#include "file_listing.h"
#include "misc.h"
#include "parameters.h"

#ifndef _DISPLAY_H_
#define _DISPLAY_H_
/*
 * the sequences of indicators are based on "enum filetype"
 * in file_listing.h
 */

void
print_entries(PENTRY root, const POPTIONS options);

#endif