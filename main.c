#include <stdio.h>
#include <stdlib.h>

#include "parameters.h"

int
main(int argc, char **argv)
{
	POPTIONS options = parse_options(argc, argv);
	printf("[TEST] %s\n", options->Paths[0]);
	return EXIT_SUCCESS;
}