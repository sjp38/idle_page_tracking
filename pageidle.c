#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setidle(int start_pfn, int end_pfn)
{
}

void getidle(int start_pfn, int end_pfn)
{
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
