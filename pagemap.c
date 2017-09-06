#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_SHIFT 12
#define SZ_PAGE (1ull << PAGE_SHIFT)

/* bit 63 */
#define PAGEMAP_PRESENT(ent)	(((ent) & (1ull << 63)) != 0)
/* bits 0-54 */
#define PAGEMAP_PFN(ent)	((ent) & ((1ull << 55) - 1))

typedef unsigned long uintptr_t;

int main(int argc, char *argv[])
{
	char *pid;
	char path[256];
	int f;
	unsigned long long ent;

	unsigned long long vaddr, start_vaddr, end_vaddr;

	if (argc < 4)
		errx(1, "Usage: %s <pid of process> <start vaddr> <end vaddr>\n",
				argv[0]);

	pid = argv[1];
	start_vaddr = (atoi(argv[2]));
	end_vaddr = (atoi(argv[3]));

	snprintf(path, 256, "/proc/%s/pagemap", pid);

	f = open(path, O_RDONLY);

	for (vaddr = start_vaddr; vaddr < end_vaddr; vaddr += SZ_PAGE) {
		if (pread(f, &ent, sizeof(ent),
					(uintptr_t)vaddr >> (PAGE_SHIFT - 3)) != sizeof(ent))
			err(2, "pread pagemap");
		printf("map for vaddr %p: %llx\n", (void *)vaddr, ent);
	}

	return 0;
}
