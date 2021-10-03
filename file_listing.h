#include <sys/stat.h>

#include <limits.h>

#include "parameters.h"

#ifndef _LISTING_H_
#define _LISTING_H_

typedef struct ENTRY
{
	struct stat info;
	char path[PATH_MAX];
	struct ENTRY *subentries;
} ENTRY, *PENTRY;

void
travel_directory(const POPTIONS options);

#endif /* !_LISTING_H_ */