#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "parameters.h"

extern int errno;

int
resolve_path(const char *target_path, POPTIONS container)
{
	char abs_path[PATH_MAX] = {
		'\x00',
	};
	if (target_path == NULL || strlen(target_path) < 1) {
		TO_STDERR("Invalid target_path: %s\n", target_path);
		return EXIT_FAILURE;
	}
	if (realpath(target_path, abs_path) == NULL) {
		TO_STDERR("Failed to resolve target path '%s': 0x%x (%s)\n",
			  target_path, errno, strerror(errno));
		return EXIT_FAILURE;
	}

	container->SpecifiedPath = calloc(strlen(abs_path) + 1, sizeof(char));
	strcpy(container->SpecifiedPath, abs_path);

	return EXIT_SUCCESS;
}
POPTIONS
_get_empty_container()
{
	POPTIONS NewContainer = (POPTIONS)calloc(1, sizeof(OPTIONS));

	resolve_path("./", NewContainer);
	return NewContainer;
}
OPTIONS
parse_arguments(int argc, char *argv[])
{
	POPTIONS Result = _get_empty_container();
	while (argc-- > 1) {
		TO_STDOUT("Argc: %d => %s\n", argc, argv[argc]);
		switch (argv[argc][0]) {
		case '-':
			map_options(Result, argv[argc]);
			break;
		default:
			if (resolve_path(argv[argc], Result) == EXIT_FAILURE) {
				TO_STDERR("Invalid parameter: %s\n",
					  argv[argc]);
				exit(EXIT_FAILURE);
			}
			break;
		}
	}
	TO_STDOUT("TargetPath: %s\n", Result->SpecifiedPath);
	return *Result;
}
int
map_options(const POPTIONS container, char *argv)
{
	if (container == NULL) {
		TO_STDERR("Failed to mapp options%s\n", "");
		return EXIT_FAILURE;
	}

	if (argv[0] == '-')
		argv = &argv[1];

	int len = strlen(argv);
	while (len-- > 0) {
		switch (argv[len]) {
		case 'A':
			container->ListAllEntries = true;
			break;
		case 'a':
			container->IncludeDirectoryEntries = true;
			break;
		case 'c':
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
		case 'h':
			container->HumanReadableFormat = true;
			break;
		case 'i':
			container->ShowInodeNumber = true;
			break;
		case 'k':
			container->OverrideSizeFormatAsKb = true;
			break;
		case 'l':
			container->ListInLongFormat = true;
			break;
		case 'n':
			container->ShowAsUidAndGid = true;
			break;
		case 'q':
			container->ForceDisplayingNonPrintableLetters = true;
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
		case 'u':
			container->SortByLastAccess = true;
			break;
		case 'w':
			container->ForceRawPrintingOfNonPrintable = true;
			break;
		}
	}
	return EXIT_SUCCESS;
}
