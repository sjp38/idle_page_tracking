#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BIT_AT(val, x)	(((val) & (1ull << x)) != 0)
#define SET_BIT(val, x) ((val) | (1ull << x))

void setidle(int start_pfn, int end_pfn)
{
	int fd;
	int pfn;
	unsigned long long entry;

	fd = open("/sys/kernel/mm/page_idle/bitmap", O_RDWR);
	if (fd < 0)
		err(2, "open bitmap");

	for (pfn = start_pfn; pfn <= end_pfn; pfn++) {
		entry = 0;
		if (pread(fd, &entry, sizeof(entry), pfn / 64 * 8)
				!= sizeof(entry))
			err(2, "%s: read bitmap", __func__);
		entry = SET_BIT(entry, pfn % 64);
		if (pwrite(fd, &entry, sizeof(entry), pfn / 64 * 8)
				!= sizeof(entry))
			err(2, "%s: write bitmap", __func__);
	}

	close(fd);
}

void getidle(int start_pfn, int end_pfn)
{
	int fd;
	unsigned long long entry;
	unsigned long long pfn;

	fd = open("/sys/kernel/mm/page_idle/bitmap", O_RDONLY);
	if (fd < 0)
		err(2, "open bitmap");

	for (pfn = start_pfn; pfn <= end_pfn; pfn++) {
		entry = 0;
		if (pread(fd, &entry, sizeof(entry), pfn / 64 * 8)
				!= sizeof(entry))
			err(2, "%s: read bitmap", __func__);
		printf("the bit: %d\n", (int)BIT_AT(entry, pfn % 64));
	}

	close(fd);
}

int main(int argc, char *argv[])
{
	if (argc < 4)
		errx(1, "Usage: %s <set|get> <start pfn> <end pfn>\n",
				argv[0]);

	if (strncmp(argv[1], "set", strlen("set")) == 0)
		setidle(atoi(argv[2]), atoi(argv[3]));
	else if (strncmp(argv[1], "get", strlen("get")) == 0)
		getidle(atoi(argv[2]), atoi(argv[3]));
	else
		errx(1, "Usage: %s <set|get> <start pfn> <end pfn>\n",
				argv[0]);

	return 0;
}
