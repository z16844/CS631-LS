#include <stdbool.h>

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

typedef struct _OPTIONS
{
	bool ListAllEntries;				  // -A***
	bool AllEntriesIncludeDirectory;		  // -a***
	bool OrderByLastChanged;			  // -c***
	bool DirectoriesAsPlainFiles;			  // -d
	bool WithTypeSymbols;				  // -F***
	bool NotSortedOutput;				  // -f***
	bool HumanReadableFormat;			  // -h***
	bool ShowInodeNumber;				  // -i***
	bool SizeFormatAsKb;				  // -k
	bool ListInLongFormat;				  // -l***
	bool ShowAsUidAndGid;				  // -n***
	bool ForceDisplayingNonPrintableLetters;	  // -q
	bool ListRecursivelySubDirectoriesEncountered;	  // -R
	bool OrderReversal;				  // -r***
	bool OrderDesceningBySize;			  //-S***
	bool DisplayByBlockSize;			  // -s***
	bool OrderByLastModified;			  // -t***
	bool OrderByLastAccess;				  // -u***
	bool ForceRawPrintingOfNonPrintable;		  // -w
	char **Paths;
	int CountPaths;
} OPTIONS, *POPTIONS;

POPTIONS
get_empty_container();
int
resolve_path(const char *target_path, char *result);
void
parse_paths(POPTIONS container, char *path);
POPTIONS
parse_options(int argc, char **argv);
#endif /* !_PARAMETERS_H_ */