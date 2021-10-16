#include "file_listing.h"
#include "misc.h"
#include "parameters.h"

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

typedef struct display_format_setting
{
	int numberOfEntries;
	int totalLines;
	int ColumnsOfTerminal;
	// from file
	unsigned int maxHardLinks;
	unsigned int maxUserLen;
	unsigned int maxGroupLen;
	unsigned int maxSizeLen;
	int maxFilenameLen;
} FORM_SETTING;

char *
getGroupName(gid_t gid);

char *
getUserName(uid_t uid);
void
initialize_setting(PENTRY root);
void
print_entries(PENTRY root, const POPTIONS options);

#endif