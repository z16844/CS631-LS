#include <stdbool.h>

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

struct Flags
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

struct Flags
_get_empty_container(char *target_path);

struct Flags
parse_arguments(int argc, char *argv[]);

#endif /* !_PARAMETERS_H_ */