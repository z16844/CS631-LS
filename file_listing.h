#include <sys/stat.h>

#include <dirent.h>
#include <limits.h>
#include <stdbool.h>

#include "parameters.h"

#ifndef _LISTING_H_
#define _LISTING_H_

enum filetype
{
	Directory,		   // d
	Symbolic_link,		   // l
	Socket_link,		   // s
	FIFO,			   // p
	Whiteout,		   // w
	Regular_file,		   // −
	Archive_state_1,	   // a
	Archive_state_2,	   // A
	Block_special_file,	   // b
	Character_special_file,	   // c
};

typedef struct ENTRY
{
	struct stat info;
	char *filename;
	enum filetype type;
	struct ENTRY **children;    // PENTRY[]
	struct ENTRY *next;	    // PENTRY
	struct ENTRY *prev;	    // PENTRY
} ENTRY, *PENTRY;

typedef struct metadata
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

PENTRY
travel_directory(const POPTIONS options);

bool
is_visible(const POPTIONS options, struct dirent *entry);

void
update_metadata(PENTRY entry, const POPTIONS options);

FORM_SETTING *
get_metadata();

#endif /* !_LISTING_H_ */