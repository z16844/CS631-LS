#include "file_listing.h"
#ifndef _SORTING_H_
#define _SORTING_H_

typedef struct ENTRY_LIST
{
	PENTRY head;
	PENTRY tail;
} LIST;

PENTRY
sort(PENTRY root, const POPTIONS options);

#endif /* !_SORTING_H_ */