#include <stdio.h>
#include <stdlib.h>

#include "file_listing.h"
#include "parameters.h"

int
main(int argc, char **argv)
{
	POPTIONS options = parse_options(argc, argv);
	travel_directory(options);
	printf("\n");
	return EXIT_SUCCESS;
}