#include <stdbool.h>

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

typedef struct _OPTIONS
{
	bool ListAllEntries;				  // -A
	bool IncludeDirectoryEntries;			  // -a
	bool UseLastFileStatusChangeTime;		  // -c
	bool DirectoriesAsPlainFiles;			  // -d
	bool WithTypeSymbols;				  // -F
	bool NotSortedOutput;				  // -f
	bool HumanReadableFormat;			  // -h
	bool ShowInodeNumber;				  // -i
	bool SizeFormatAsKb;				  // -k
	bool ListInLongFormat;				  // -l
	bool ShowAsUidAndGid;				  // -n
	bool ForceDisplayingNonPrintableLetters;	  // -q
	bool ListRecursivelySubDirectoriesEncountered;	  // -R
	bool ReverseTheLexicographicalOrder;		  // -r
	bool OrderBySize;				  //-S
	bool DisplayByBlockSize;			  // -s
	bool OrderByLastModifiedAscending;		  // -t
	bool SortByLastAccess;				  // -u
	bool ForceRawPrintingOfNonPrintable;		  // -w
	char *SpecifiedPath;
} OPTIONS, *POPTIONS;

POPTIONS
get_empty_container();

int
resolve_path(const char *target_path, char *result);

#endif /* !_PARAMETERS_H_ */