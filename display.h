#include "file_listing.h"
#include "misc.h"
#include "parameters.h"

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

char *
getGroupName(gid_t gid);

char *
getUserName(uid_t uid);

void
print_entries(PENTRY root, const POPTIONS options);

#endif