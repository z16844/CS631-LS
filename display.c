#include "display.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

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
	/****sequence columns in -l with other options
	 * inode (-i)
	 * blocks (-s)
	 * mode
	 * # of hardlinks
	 * owner name
	 * group name
	 * bytes of the file
	 * last modified date/time
	 * pathname
	 */
	int len_blank = 0;
	if (options->ShowInodeNumber) {	   // -i
		char *inode = itoa(entry->info.st_ino);
		int lenInode = strlen(inode);
		len_blank = setting->maxInodeLen - lenInode;
		for (; len_blank > 0; len_blank--)
			printf(" ");
		printf("%s ", inode);
		free(inode);
		len_blank = 0;
	}
	if (options->DisplayByBlockSize) {    // -s
		char *blocks = itoa(entry->info.st_blocks);
		int len_blocks = strlen(blocks);

		len_blank = setting->maxBlockSizeLen - len_blocks;
		for (; len_blank > 0; len_blank--)
			printf(" ");
		printf("%s ", blocks);
		free(blocks);
		len_blank = 0;
	}

	char mode[11] = { 0 };
	strmode(entry->info.st_mode, mode);
	printf("%s ", mode);

	char *hardlinks = itoa(entry->info.st_nlink);
	len_blank = setting->maxHardLinks - strlen(hardlinks);
	for (; len_blank > 0; len_blank--)
		printf(" ");
	printf("%s ", hardlinks);
	free(hardlinks);

	char *username = options->ShowAsUidAndGid
			     ? itoa(entry->info.st_uid)
			     : getUserName(entry->info.st_uid);
	len_blank = setting->maxUserLen - strlen(username);
	for (; len_blank > 0; len_blank--)
		printf(" ");
	printf("%s ", username);

	char *group_name = options->ShowAsUidAndGid
			       ? itoa(entry->info.st_gid)
			       : getGroupName(entry->info.st_gid);
	len_blank = setting->maxGroupLen - strlen(group_name);
	for (; len_blank > 0; len_blank--)
		printf(" ");
	printf("%s ", group_name);

	char *size = options->HumanReadableFormat
			 ? asHumanReadable(entry->info.st_size)
			 : itoa(entry->info.st_size);
	len_blank = setting->maxSizeLen - strlen(size);
	for (; len_blank > 0; len_blank--)
		printf(" ");
	printf("%s ", size);
	free(size);

	char *timestamp = parse_timestamp(&entry->info.st_mtime);
	printf("%s ", timestamp);
	free(timestamp);

	printf("%s ", entry->filename);
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
}