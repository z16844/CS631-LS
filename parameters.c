#include "parameters.h"

#include <sys/stat.h>

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "misc.h"

extern int errno;
extern int optind;

int
resolve_path(const char *target_path, char *result)
{
	char abs_path[PATH_MAX] = {
		'\x00',
	};
	if (target_path == NULL || strlen(target_path) < 1) {
		fprintf(stderr, "Invalid target_path: %s\n", target_path);
		return EXIT_FAILURE;
	}
	if (realpath(target_path, abs_path) == NULL) {
		fprintf(stderr,
			"Failed to resolve target path '%s': 0x%x (%s)\n",
			target_path, errno, strerror(errno));
		return EXIT_FAILURE;
	}

	result = calloc_checked(strlen(abs_path) + 1, sizeof(char));
	strcpy(result, abs_path);

	return EXIT_SUCCESS;
}
POPTIONS
get_empty_container()
{
	POPTIONS NewContainer = (POPTIONS)calloc_checked(1, sizeof(OPTIONS));

	/*
	 * According to C Standards, the maximum count of argv is 127. Due to
	 * option parameters, available argv space would be less than 127,
	 * however, just set 127 to avoid more complexity for now.
	 */
	NewContainer->Paths = (char **)calloc_checked(127, sizeof(char *));
	NewContainer->CountPaths = -1;

	return NewContainer;
}
void
parse_paths(POPTIONS container, char *path)
{
	struct stat stat_path = { 0 };
	if (stat(path, &stat_path) != EXIT_SUCCESS) {
		fprintf(stderr, "Invalid Path: %s\n", path);
		exit(EXIT_FAILURE);
	}
	container->CountPaths++;
	container->Paths[container->CountPaths] = path;
}
POPTIONS
parse_options(int argc, char **argv)
{
	char parsed = -1;
	POPTIONS container = get_empty_container();
	while ((parsed = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
		switch (parsed) {
		case 'A':
			container->ListAllEntries = true;
			break;
		case 'a':
			container->AllEntriesIncludeDirectory = true;
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
	return container;
}