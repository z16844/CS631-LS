#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "misc.h"
#include "parameters.h"

int
main(int argc, char **argv)
{
	printf("Hello World!\n");
	parse_arguments(argc, argv);
	return EXIT_SUCCESS;
}