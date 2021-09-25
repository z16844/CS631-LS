#include <stdbool.h>

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

struct _OPTIONS
{
	bool ListAllEntries;				  // -A
	bool IncludeDirectoryEntries;			  // -a
	bool UseLastFileStatusChangeTime;		  // -c
	bool DirectoriesAsPlainFiles;			  // -d
	bool WithTypeSymbols;				  // -F
	bool NotSortedOutput;				  // -f
	bool HumanReadableFormat;			  // -h
	bool ShowInodeNumber;				  // -i
	bool OverrideSizeFormatAsKb;			  // -k
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
};
typedef struct _OPTIONS OPTIONS;
typedef struct _OPTIONS *POPTIONS;

POPTIONS
_get_empty_container();

int
resolve_path(const char *input_path, POPTIONS resolved);

OPTIONS
parse_arguments(int argc, char *argv[]);

int
map_options(const POPTIONS container, char *argv);

#endif /* !_PARAMETERS_H_ */