#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define SZ_PAGE (4*1024)

#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))

char **pages;

int alloc_pages(int nr_pages)
{
	int i;

	pages = (char **)malloc(sizeof(char *) * nr_pages);
	for (i = 0; i < nr_pages; i++) {
		pages[i] = (char *)malloc(SZ_PAGE);
		ACCESS_ONCE(pages[i][0]) = 1;
	}

	return 0;
}

int access_pages(int nr_pages)
{
	int i;

	for (i = 0; i < nr_pages; i++)
		ACCESS_ONCE(pages[i][0]) = 1;

	return 0;
}

int main(int argc, char *argv[])
{
	int n;
	int nr_pages;
	int nr_accesses;

	if (argc < 2)
		errx(1, "Usage: %s <number of pages to utilize>\n", argv[0]);

	nr_pages = atoi(argv[1]);
	if (nr_pages < 1)
		errx(1, "Argument '%s' is wrong", argv[1]);

	alloc_pages(nr_pages);

	while (1) {
		printf("How many pages you want to access? ");
		n = scanf("%d", &nr_accesses);
		if (errno != 0)
			err(1, "number of pages to access");
		if (n != 1)
			errx(1, "Wrong input");
		if (nr_accesses < 0) {
			printf("Bye\n");
			return 0;
		}
		printf("will access %d pages\n", nr_accesses);
		access_pages(nr_accesses);
	}

	return 0;
}
