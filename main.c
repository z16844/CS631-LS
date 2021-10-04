#include <stdio.h>
#include <stdlib.h>

#include "file_listing.h"
#include "parameters.h"
#include "sorting.h"

int
main(int argc, char **argv)
{
	POPTIONS options = parse_options(argc, argv);
	PENTRY result = travel_directory(options);
	result = sort(result, options);

	PENTRY cursor = result;
	while (cursor != NULL) {
		printf("%s\t", cursor->filename);
		cursor = cursor->next;
	}

	printf("\n");
	return EXIT_SUCCESS;
}