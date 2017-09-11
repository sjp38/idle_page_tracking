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

void setidle(int nr_pfns, int pfns[])
{
	int fd;
	int pfn;
	unsigned long long entry;
	int i;

	fd = open("/sys/kernel/mm/page_idle/bitmap", O_RDWR);
	if (fd < 0)
		err(2, "open bitmap");

	for (i = 0; i < nr_pfns; i++) {
		pfn = pfns[i];
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

void getidle(int nr_pfns, int pfns[])
{
	int fd;
	unsigned long long entry;
	unsigned long long pfn;
	int i;

	fd = open("/sys/kernel/mm/page_idle/bitmap", O_RDONLY);
	if (fd < 0)
		err(2, "open bitmap");

	for (i = 0; i < nr_pfns; i++) {
		pfn = pfns[i];
		entry = 0;
		if (pread(fd, &entry, sizeof(entry), pfn / 64 * 8)
				!= sizeof(entry))
			err(2, "%s: read bitmap", __func__);
		printf("%d ", (int)BIT_AT(entry, pfn % 64));
	}
	printf("\n");

	close(fd);
}

int main(int argc, char *argv[])
{
	int *pfns;
	int nr_pfns;
	int i;

	if (argc < 4)
		errx(1, "Usage: %s <set|get> <pfn> [pfn 2]...\n",
				argv[0]);

	nr_pfns = argc - 2;
	pfns = (int *)malloc(sizeof(int) * nr_pfns);
	for (i = 0; i < nr_pfns; i++)
		pfns[i] = atoi(argv[i + 2]);

	if (strncmp(argv[1], "set", strlen("set")) == 0)
		setidle(nr_pfns, pfns);
	else if (strncmp(argv[1], "get", strlen("get")) == 0)
		getidle(nr_pfns, pfns);
	else
		errx(1, "Usage: %s <set|get> <start pfn> <end pfn>\n",
				argv[0]);

	return 0;
}
