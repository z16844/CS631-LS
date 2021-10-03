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
		fprintf(stderr, "Invalid target_path: %s\n", target_path);
		return EXIT_FAILURE;
	}
	if (realpath(target_path, abs_path) == NULL) {
		fprintf(stderr,
			"Failed to resolve target path '%s': 0x%x (%s)\n",
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
		fprintf(stdout, "Argc: %d => %s\n", argc, argv[argc]);
		switch (argv[argc][0]) {
		case '-':
			map_options(Result, argv[argc]);

			/* To make sure dependent options */
			map_options(Result, argv[argc]);
			break;
		default:
			if (resolve_path(argv[argc], Result) == EXIT_FAILURE) {
				fprintf(stderr, "Invalid parameter: %s\n",
					argv[argc]);
				exit(EXIT_FAILURE);
			}
			break;
		}
	}
	fprintf(stdout, "TargetPath: %s\n", Result->SpecifiedPath);
	return *Result;
}
int
map_options(const POPTIONS container, char *argv)
{
	if (container == NULL) {
		fprintf(stderr, "Failed to map options\n");
		return EXIT_FAILURE;
	}

	if (argv[0] == '-')
		argv = &argv[1];

	int len = strlen(argv);
	while (len-- > 0) {
		switch (argv[len]) {
		case 'A':
			container->ListAllEntries = true;
			fprintf(stdout, "[MAP] ListAllEntries(-A): %d\n",
				container->ListAllEntries);
			break;
		case 'a':
			container->IncludeDirectoryEntries = true;
			fprintf(stdout,
				"[MAP] IncludeDirectoryEntries(-a): %d\n",
				container->IncludeDirectoryEntries);
			break;
		case 'c':    // override: -c / -u
			if (container->SortByLastAccess == false)
				container->UseLastFileStatusChangeTime = true;
			fprintf(stdout,
				"[MAP] UseLastFileStatusChangeTime(-c): %d\n",
				container->UseLastFileStatusChangeTime);
			break;
		case 'd':
			container->DirectoriesAsPlainFiles = true;
			fprintf(stdout,
				"[MAP] DirectoriesAsPlainFiles(-d): %d\n",
				container->DirectoriesAsPlainFiles);
			break;
		case 'F':
			container->WithTypeSymbols = true;
			fprintf(stdout, "[MAP] WithTypeSymbols(-F): %d\n",
				container->WithTypeSymbols);
			break;
		case 'f':
			container->NotSortedOutput = true;
			fprintf(stdout, "[MAP] NotSortedOutput(-f): %d\n",
				container->NotSortedOutput);
			break;
		case 'h':    // Dependency: -s / -l | override: -k
			if (container->ListInLongFormat ||
			    container->OrderBySize)
				if (container->SizeFormatAsKb == false)
					container->HumanReadableFormat = true;
			fprintf(stdout, "[MAP] HumanReadableFormat(-h): %d\n",
				container->HumanReadableFormat);
			break;
		case 'i':
			container->ShowInodeNumber = true;
			fprintf(stdout, "[MAP] ShowInodeNumber(-i): %d\n",
				container->ShowInodeNumber);
			break;
		case 'k':    // Dependency: -s | override: -h
			if (container->DisplayByBlockSize &&
			    container->HumanReadableFormat == false) {
				container->SizeFormatAsKb = true;
			}
			fprintf(stdout, "[MAP] SizeFormatAsKb(-k): %d\n",
				container->SizeFormatAsKb);
			break;
		case 'l':    // override: -l / -n
			if (container->ShowAsUidAndGid == false)
				container->ListInLongFormat = true;
			fprintf(stdout, "[MAP] ListInLongFormat(-l): %d\n",
				container->ListInLongFormat);
			break;
		case 'n':    // override: -l / -n
			if (container->ListInLongFormat == false)
				container->ShowAsUidAndGid = true;
			fprintf(stdout, "[MAP] ShowAsUidAndGid(-n): %d\n",
				container->ShowAsUidAndGid);
			break;
		case 'q':    // override: -q / -w
			if (container->ForceRawPrintingOfNonPrintable == false)
				container->ForceDisplayingNonPrintableLetters =
				    true;
			fprintf(stdout,
				"[MAP] ForceDisplayingNonPrintableLetters(-q): "
				"%d\n",
				container->ForceDisplayingNonPrintableLetters);
			break;
		case 'R':
			container->ListRecursivelySubDirectoriesEncountered =
			    true;
			break;
		case 'r':
			container->ReverseTheLexicographicalOrder = true;
			fprintf(
			    stdout,
			    "[MAP] ReverseTheLexicographicalOrder(-r): %d\n",
			    container->ReverseTheLexicographicalOrder);
			break;
		case 'S':
			container->OrderBySize = true;
			fprintf(stdout, "[MAP] OrderBySize(-S): %d\n",
				container->OrderBySize);
			break;
		case 's':
			container->DisplayByBlockSize = true;
			fprintf(stdout, "[MAP] DisplayByBlockSize(-s): %d\n",
				container->DisplayByBlockSize);
			break;
		case 't':
			container->OrderByLastModifiedAscending = true;
			fprintf(stdout,
				"[MAP] OrderByLastModifiedAscending(-t): %d\n",
				container->OrderByLastModifiedAscending);
			break;
		case 'u':    // override: -c / -u
			if (container->UseLastFileStatusChangeTime == false)
				container->SortByLastAccess = true;
			fprintf(stdout, "[MAP] SortByLastAccess(-u): %d\n",
				container->SortByLastAccess);
			break;
		case 'w':    // override: -q / -w
			if (container->ForceDisplayingNonPrintableLetters ==
			    false)
				container->ForceRawPrintingOfNonPrintable =
				    true;
			fprintf(
			    stdout,
			    "[MAP] ForceRawPrintingOfNonPrintable(-w): %d\n",
			    container->ForceRawPrintingOfNonPrintable);
			break;
		}
	}
	return EXIT_SUCCESS;
}
