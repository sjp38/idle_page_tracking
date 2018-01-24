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

typedef unsigned long long u8;

void setidle(u8 nr_pfns, u8 pfns[])
{
	int fd;
	u8 pfn, entry;
	u8 i;

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

/*
 * pfn to index of idle page file bitmap
 *
 * The bitmap should be read in 8 bytes (64 pages) stride.
 */
#define PFN_TO_IPF_IDX(pfn) pfn >> 6 << 3

void getidle(u8 nr_pfns, u8 pfns[])
{
	int fd;
	u8 entry, pfn;
	u8 i;

	fd = open("/sys/kernel/mm/page_idle/bitmap", O_RDONLY);
	if (fd < 0)
		err(2, "open bitmap");

	for (i = 0; i < nr_pfns; i++) {
		pfn = pfns[i];
		entry = 0;
		if (pread(fd, &entry, sizeof(entry), PFN_TO_IPF_IDX(pfn))
				!= sizeof(entry))
			err(2, "%s: read bitmap", __func__);
		printf("%d ", (int)BIT_AT(entry, pfn % 64));
	}
	printf("\n");

	close(fd);
}

u8 nr_active(u8 nr_pfns, u8 pfns[])
{
	int fd;
	u8 entry, pfn;
	u8 i;
	u8 nr_activepages = 0;

	fd = open("/sys/kernel/mm/page_idle/bitmap", O_RDONLY);
	if (fd < 0)
		err(2, "open bitmap");

	for (i = 0; i < nr_pfns; i++) {
		pfn = pfns[i];
		entry = 0;
		if (pread(fd, &entry, sizeof(entry), PFN_TO_IPF_IDX(pfn))
				!= sizeof(entry))
			err(2, "%s: read bitmap", __func__);
		if (BIT_AT(entry, pfn % 64))
			nr_activepages++;
	}
	close(fd);

	return nr_activepages;
}

void err_usage(const char *cmd)
{
	errx(1, "Usage: %s <set|get|count> <pfns file>\n", cmd);
}

int main(int argc, char *argv[])
{
	u8 *pfns;
	u8 nr_pfns;
	u8 i;
	char *pfns_file;
	int f, nr_read;
	char do_get = 1;
	char do_count_active = 0;

	if (argc < 3)
		err_usage(argv[0]);

	if (strncmp(argv[1], "count", strlen("count")) == 0)
		do_count_active = 1;
	else if (strncmp(argv[1], "set", strlen("set")) == 0)
		do_get = 0;
	else if (strncmp(argv[1], "get", strlen("get")) == 0)
		do_get = 1;
	else
		err_usage(argv[0]);

	pfns_file = argv[2];
	f = open(pfns_file, O_RDONLY);

	nr_read = read(f, &nr_pfns, sizeof(nr_pfns));
	if (nr_read != sizeof(nr_pfns))
		err(1, "number of pfns reading");

	pfns = (u8 *)malloc(sizeof(u8) * nr_pfns);
	for (i = 0; i < nr_pfns; i++) {
		nr_read = read(f, &pfns[i], sizeof(pfns[i]));
		if (nr_read != sizeof(pfns[i]))
			err(1, "reading %lluth pfn", i);
	}

	if (do_count_active)
		printf("%llu\n", nr_active(nr_pfns, pfns));
	else if (do_get)
		getidle(nr_pfns, pfns);
	else
		setidle(nr_pfns, pfns);

	return 0;
}
