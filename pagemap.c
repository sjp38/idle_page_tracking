#include <err.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
		errx(1, "Usage: %s <pid of process>\n", argv[0]);
	printf("hello\n");

	return 0;
}
