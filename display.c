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
		char *blocks;
		if (options->SizeFormatAsKb)
			blocks = itoa((entry->info.st_blocks) / 2);
		else
			blocks = itoa(entry->info.st_blocks);

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
size_t
print_padding(size_t size)
{
	size_t written = 0;
	while (size-- > 0)
		printf(" ");
	return written;
}
void
print_grid(PENTRY root, const POPTIONS options)
{

	PENTRY *result =
	    (PENTRY *)calloc_checked(setting->numberOfEntries, sizeof(PENTRY));
	PENTRY cursor = root;

	int columnsPerItem = 1;
	if (options->ShowInodeNumber)
		columnsPerItem++;
	if (options->DisplayByBlockSize)
		columnsPerItem++;

	int columns =
	    setting->ColumnsOfTerminal / (setting->maxFilenameLen + 1);
	columns /= columnsPerItem;

	int rows = setting->numberOfEntries / columns;
	rows += setting->numberOfEntries % columns > 0 ? 1 : 0;

	// re-align as grid
	for (int c = 0; c < columns; c++) {
		for (int r = 0; r < rows; r++) {
			if ((columns * r) + c >= setting->numberOfEntries)
				break;

			result[(columns * r) + c] = cursor;

			if (!cursor->next)
				break;
			cursor = cursor->next;
		}
	}
	char *buf = NULL;
	size_t len = 0;
	for (int i = 0; i < setting->numberOfEntries; i++) {
		cursor = result[i];
		if (options->ShowInodeNumber) {
			buf = itoa(cursor->info.st_ino);
			len = strlen(buf);
			print_padding(setting->maxInodeLen - len);
			printf("%s ", buf);
			free(buf);
		}

		if (options->DisplayByBlockSize) {
			if (options->SizeFormatAsKb)
				buf = itoa(cursor->info.st_blocks / 2);
			else
				buf = itoa(cursor->info.st_blocks);
			len = strlen(buf);
			print_padding(setting->maxBlockSizeLen - len);
			printf("%s ", buf);
			free(buf);
		}

		buf = NULL;
		len = strlen(cursor->filename);
		printf("%s ", cursor->filename);
		print_padding(setting->maxFilenameLen - len);
		if ((i + 1) % columns == 0)
			printf("\n");
	}
	free(result);
}
void
print_entries(PENTRY root, const POPTIONS options)
{
	setting = get_metadata();
	if (options->WithTypeSymbols) {	   // -F
		add_indicators(root);
	}
	PENTRY cursor = root;
	if (options->DisplayByBlockSize) {	// -s
		if (options->SizeFormatAsKb)	// -k
			printf("total %d\n", (setting->sumBlocks / 2));
		else
			printf("total %d\n", (setting->sumBlocks));
	} else if (options->ListInLongFormat) {	   // -l
		printf("total %d\n", setting->sumBlocks);
	}

	if (options->ListInLongFormat) {
		while (cursor != NULL) {
			print_long_format(cursor, options);
			cursor = cursor->next;
		}
	} else {
		print_grid(root, options);
	}
	printf("\n");
}