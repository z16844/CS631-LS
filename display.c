#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

char *filetype_indicator = "dlspw-aAbc";
char *filetype_symbols = "/@=|%%\0\0\0\0\0";
// void
// print_long_format(PENTRY root, const POPTIONS options)
// {}
void
add_indicators(PENTRY root)
{
	char *new_name = NULL;
	PENTRY cursor = root;
	while (cursor != NULL) {
		new_name = (char *)calloc_checked(strlen(cursor->filename) + 2,
						  sizeof(char));
		bzero(new_name, sizeof(new_name));

		switch (cursor->type) {
		case Directory:
		case Symbolic_link:
		case Socket_link:
		case FIFO:
		case Whiteout:
			new_name =
			    strncat(cursor->filename,
				    &(filetype_symbols[cursor->type]), 1);
			break;
		default:
			if ((cursor->info.st_mode & S_IEXEC)) {
				new_name = strncat(cursor->filename, "*", 1);
			}
			break;
		}
		if (strlen(new_name) > 0)
			strncpy(cursor->filename, new_name, strlen(new_name));
		free(new_name);
		cursor = cursor->next;
	}
}
void
print_entries(PENTRY root, const POPTIONS options)
{
	if (options->WithTypeSymbols) {
		add_indicators(root);
	}
	PENTRY cursor = root;
	while (cursor != NULL) {
		printf("%s\t", cursor->filename);
		cursor = cursor->next;
	}

	printf("\n");
}