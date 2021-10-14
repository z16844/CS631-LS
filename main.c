#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "file_listing.h"
#include "parameters.h"
#include "sorting.h"

int
main(int argc, char **argv)
{
	POPTIONS options = parse_options(argc, argv);
	PENTRY result = travel_directory(options);
	result = sort(result, options);

	print_entries(result, options);

	return EXIT_SUCCESS;
}