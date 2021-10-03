#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "parameters.h"

extern int optind;

int
main(int argc, char **argv)
{
	char parsed = -1;
	POPTIONS container = get_empty_container();
	while ((parsed = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
		switch (parsed) {
		case 'A':
			container->ListAllEntries = true;
			break;
		case 'a':
			container->IncludeDirectoryEntries = true;
			break;
		case 'u':    // override: -c / -u
			container->UseLastFileStatusChangeTime = false;
			container->SortByLastAccess = true;
			break;
		case 'c':    // override: -c / -u
			container->SortByLastAccess = false;
			container->UseLastFileStatusChangeTime = true;
			break;
		case 'd':
			container->DirectoriesAsPlainFiles = true;
			break;
		case 'F':
			container->WithTypeSymbols = true;
			break;
		case 'f':
			container->NotSortedOutput = true;
			break;
		case 'i':
			container->ShowInodeNumber = true;
			break;
		case 'h':    // Dependency: -s / -l | override: -k
			container->SizeFormatAsKb = false;
			container->HumanReadableFormat = true;
			break;
		case 'k':    // Dependency: -s | override: -h
			container->HumanReadableFormat = false;
			container->SizeFormatAsKb = true;
			break;
		case 'l':    // override: -l / -n
			container->ShowAsUidAndGid = false;
			container->ListInLongFormat = true;
			break;
		case 'n':    // override: -l / -n
			container->ListInLongFormat = false;
			container->ShowAsUidAndGid = true;
			break;
		case 'R':
			container->ListRecursivelySubDirectoriesEncountered =
			    true;
			break;
		case 'r':
			container->ReverseTheLexicographicalOrder = true;
			break;
		case 'S':
			container->OrderBySize = true;
			break;
		case 's':
			container->DisplayByBlockSize = true;
			break;
		case 't':
			container->OrderByLastModifiedAscending = true;
			break;
		case 'q':    // override: -q / -w
			container->ForceRawPrintingOfNonPrintable = false;
			container->ForceDisplayingNonPrintableLetters = true;
			break;
		case 'w':    // override: -q / -w
			container->ForceDisplayingNonPrintableLetters = false;
			container->ForceRawPrintingOfNonPrintable = true;
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (argc == 0) {
		parse_paths(container, "./");
	} else {
		while (argc-- > 0) {
			parse_paths(container, argv[argc]);
		}
	}
	return EXIT_SUCCESS;
}