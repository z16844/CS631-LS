#include "display.h"

#include <stdio.h>

void
print_entries(PENTRY root, const POPTIONS options)
{
	if (options->WithTypeSymbols) {
	}
	PENTRY cursor = root;
	while (cursor != NULL) {
		printf("%s\t", cursor->filename);
		cursor = cursor->next;
	}

	printf("\n");
}