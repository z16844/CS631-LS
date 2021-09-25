#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "parameters.h"

extern int errno;
int
resolve_path(const char *target_path, char *resolved)
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

	resolved = calloc(strlen(abs_path) + 1, sizeof(char));
	strcpy(resolved, abs_path);

	return EXIT_SUCCESS;
};

struct Flags
_get_empty_container()
{
	struct Flags NewContainer = {
		NULL,
	};

	resolve_path("./", NewContainer.SpecifiedPath);
	return NewContainer;
}
struct Flags
parse_arguments(int argc, char *argv[])
{
	struct Flags Result = _get_empty_container();
	while (argc-- > 1) {
		TO_STDOUT("Argc: %d => %s\n", argc, argv[argc]);
	}
	TO_STDOUT("TargetPath: %s\n", Result.SpecifiedPath);
	return Result;
}