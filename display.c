#include "display.h"

#include <sys/ioctl.h>

#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

char *
itoa(int value)
{
	int len = (value / 10) + 1;
	char *buf = (char *)calloc_checked((value / 10) + 1, sizeof(char));
	if (snprintf(buf, len, "%d", value) == -1) {
		fprintf(stderr, "Invalid Parameter to itoa()");
		exit(EXIT_FAILURE);
	}
	return buf;
}
char *
getGroupName(gid_t gid)
{
	struct group *g = getgrgid(gid);
	if (g == NULL) {
		fprintf(stderr, "Failed to get group name from gid: %d\n", gid);
		exit(EXIT_FAILURE);
	}
	return g->gr_name;
}
char *
getUserName(uid_t uid)
{
	struct passwd *pw = getpwuid(uid);
	if (pw == NULL) {
		fprintf(stderr, "Failed to get user name from uid: %d\n", uid);
		exit(EXIT_FAILURE);
	}
	return pw->pw_name;
}
/*
 * the sequences of indicators are based on "enum filetype"
 * in file_listing.h
 */
char *filetype_indicator = "dlspw-aAbc";
char *filetype_symbols = "/@=|%%\0\0\0\0\0";
FORM_SETTING *setting = NULL;
void
print_long_format(PENTRY entry, const POPTIONS options)
{
	int offset = -1;
	int len_DateTime = 11;
	char line_buf[10		      /* filetype and permission*/
		      + 1		      /* blank */
		      + setting->maxHardLinks /* hardlink field */
		      + 1		      /* blank */
		      + setting->maxUserLen   /* owner field */
		      + 1		      /* blank */
		      + setting->maxGroupLen  /* group field */
		      + 1		      /* blank */
		      + setting->maxSizeLen   /* file size field */
		      + 1		      /* blank */
		      + len_DateTime	      /* DateTime field */
		      + 1		      /* blank before filename*/
	];
	memset(line_buf, '\x20', sizeof(line_buf));
	line_buf[sizeof(line_buf) - 1] = '\x00';

	/* TODO: Sticky bits */
	/* FileType */
	line_buf[0] = filetype_indicator[entry->type];
	/* permission indicator */
	int i;
	for (i = 1; i < 10; i++) {
		int bit = (entry->info.st_mode >> (i - 1)) & 0x1;
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
	offset += (10 + 1 + setting->maxHardLinks) - strlen(hardlinks);
	strncpy(&(line_buf[offset]), hardlinks, setting->maxHardLinks);

	char *username = getUserName(entry->info.st_uid);
	offset += setting->maxUserLen - strlen(username);
	strncpy(&(line_buf[offset]), username, strlen(username));

	char *group = getGroupName(entry->info.st_gid);
	offset += setting->maxGroupLen - strlen(group);
	strncpy(&(line_buf[offset]), group, strlen(group));

	/* TODO: -h options (options->HumanReadableFormat) */
	if (options->HumanReadableFormat) {
	}
	char *size = itoa(entry->info.st_size);
	offset += setting->maxSizeLen - strlen(size);
	strncpy(&(line_buf[offset]), size, strlen(size));

	/* TODO: build DateTime*/
	offset += len_DateTime + 1;
	strncat(&(line_buf[offset]), entry->filename, strlen(entry->filename));
	line_buf[offset + 1] = '\n';

	printf(line_buf);
}
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
initialize(PENTRY root)
{
	PENTRY cursor = root;
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	/* Check the width of terminal */
	if (setting == NULL)
		setting =
		    (FORM_SETTING *)calloc_checked(1, sizeof(FORM_SETTING));
	setting->ColumnsOfTerminal = w.ws_col;
	setting->numberOfEntries = 0;

	while (cursor != NULL) {
		/* Check the maximum length of file name */
		int name_len = strlen(cursor->filename);
		if (name_len > setting->maxFilenameLen)
			setting->maxFilenameLen = name_len;

		/* TODO: -n options (options->ShowAsUidAndGid) */
		char *user_name = getUserName(cursor->info.st_uid);
		if (strlen(user_name) > setting->maxUserLen)
			setting->maxUserLen = strlen(user_name);
		char *group_name = getGroupName(cursor->info.st_gid);
		if (strlen(group_name) > setting->maxUserLen)
			setting->maxGroupLen = strlen(group_name);

		/* TODO: -h options (options->HumanReadableFormat) */
		if (strlen(itoa(cursor->info.st_size)) > setting->maxSizeLen)
			setting->maxSizeLen = cursor->info.st_size;

		/* Check the number of max hard links */
		if (strlen(itoa(cursor->info.st_nlink)) > setting->maxHardLinks)
			setting->maxHardLinks = cursor->info.st_nlink;
	}
}
void
print_entries(PENTRY root, const POPTIONS options)
{
	initialize(root);
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