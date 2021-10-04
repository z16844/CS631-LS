#include <sys/stat.h>

#include <dirent.h>
#include <limits.h>
#include <stdbool.h>

#include "parameters.h"

#ifndef _LISTING_H_
#define _LISTING_H_

enum filetype
{
	Archive_state_1,	   // a
	Archive_state_2,	   // A
	Block_special_file,	   // b
	Character_special_file,	   // c
	Directory,		   // d
	Symbolic_link,		   // l
	Socket_link,		   // s
	FIFO,			   // p
	Whiteout,		   // w
	Regular_file,		   // −
};

typedef struct ENTRY
{
	struct stat info;
	char filename[PATH_MAX];
	enum filetype type;
	struct ENTRY **children;
} ENTRY, *PENTRY;

PENTRY
travel_directory(const POPTIONS options);

bool
is_visible(const POPTIONS options, struct dirent *entry);

#endif /* !_LISTING_H_ */