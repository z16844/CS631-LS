#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include "misc.h"

extern int errno;
FORM_SETTING *setting = NULL;

/*
 * the sequences of indicators are based on "enum filetype"
 * in file_listing.h
 */
char *filetype_indicator = "dlspw-aAbc";
char *filetype_symbols = "/@=|%%\0\0\0\0\0";

void
print_long_format(PENTRY entry, const POPTIONS options)
{
	int offset = -1;
	int len_DateTime = 11;
	char line_buf[10			/* filetype and permission*/
		      + 1			/* blank */
		      + setting->maxHardLinks	/* hardlink field */
		      + 1			/* blank */
		      + setting->maxUserLen	/* owner field */
		      + 1			/* blank */
		      + setting->maxGroupLen	/* group field */
		      + 1			/* blank */
		      + setting->maxSizeLen	/* file size field */
		      + 1			/* blank */
		      + len_DateTime		/* DateTime field */
		      + 1			/* blank before filename*/
		      + strlen(entry->filename) /* Filename field */
	];
	memset(line_buf, '\x20', sizeof(line_buf));
	line_buf[sizeof(line_buf) - 1] = '\x00';

	/* TODO: Sticky bits */
	/* FileType */
	line_buf[0] = filetype_indicator[entry->type];
	/* permission indicator */
	int i;
	int umask = entry->info.st_mode & 0777;
	for (i = 1; i < 10; i++) {
		bool bit = ((0400 >> (i - 1)) & umask) != 0;
		if (!bit) {
			line_buf[i] = '-';
			continue;
		}
		switch (i % 3) {
		case 1:
			line_buf[i] = 'r';
			break;
		case 2:
			line_buf[i] = 'w';
			break;
		case 3:
			line_buf[i] = 'x';
			break;
		}
	}
	/* TODO: handle blanks */
	offset = 10;
	line_buf[offset] = '\x20';

	char *hardlinks = itoa(entry->info.st_nlink);
	offset += 1 + setting->maxHardLinks - strlen(hardlinks);
	strncpy(&(line_buf[offset]), hardlinks, setting->maxHardLinks);
	free(hardlinks);

	char *username = getUserName(entry->info.st_uid);
	offset +=
	    strlen(hardlinks) + 1 + setting->maxUserLen - strlen(username);
	strncpy(&(line_buf[offset]), username, strlen(username));

	char *group_name = getGroupName(entry->info.st_gid);
	offset +=
	    strlen(username) + 1 + setting->maxGroupLen - strlen(group_name);
	strncpy(&(line_buf[offset]), group_name, strlen(group_name));

	if (options->HumanReadableFormat) {
		/* TODO: -h options (options->HumanReadableFormat) */
	}
	char *size = itoa(entry->info.st_size);
	offset += strlen(group_name) + 1 + setting->maxSizeLen - strlen(size);
	strncpy(&(line_buf[offset]), size, strlen(size));
	free(size);

	char *timestamp = parse_timestamp(&entry->info.st_mtime);
	offset += strlen(size) + 1;
	strncpy(&(line_buf[offset]), timestamp, strlen(timestamp));
	free(timestamp);

	offset += strlen(timestamp) + 1;
	int lenFilename = strlen(entry->filename);
	strncpy(&(line_buf[offset]), entry->filename, lenFilename);
	offset += lenFilename;
	line_buf[offset] = '\x00';

	printf(line_buf);
	printf("\n");
}
void
add_indicators(PENTRY root)
{
	char *new_name = NULL;
	PENTRY cursor = root;
	while (cursor != NULL) {
		new_name = NULL;

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
		if (new_name != NULL && strlen(new_name) > 0)
			strncpy(cursor->filename, new_name, strlen(new_name));
		// free(new_name);
		cursor = cursor->next;
	}
}

void
print_entries(PENTRY root, const POPTIONS options)
{
	setting = get_metadata();
	if (options->WithTypeSymbols) {	   // -F
		add_indicators(root);
	}
	PENTRY cursor = root;
	while (cursor != NULL) {
		if (options->ListInLongFormat) {    // -l
			print_long_format(cursor, options);
		} else {
			printf("%s\t", cursor->filename);
		}
		cursor = cursor->next;
	}

	printf("\n");
}