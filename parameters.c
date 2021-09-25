#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "parameters.h"

extern int errno;

struct Flags
_get_empty_container(char *target_path)
{
	char abs_path[PATH_MAX] = {
		'\x00',
	};
	struct Flags NewContainer = {
		NULL,
	};
	if (target_path == NULL)
		target_path = "./";

	if (realpath(target_path, abs_path) == NULL) {
		TO_STDERR("Failed to resolve target path '%s': 0x%x (%s)\n",
			  target_path, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	NewContainer.SpecifiedPath = abs_path;
	return NewContainer;
}
struct Flags
parse_arguments(int argc, char *argv[])
{
	struct Flags Result = _get_empty_container(NULL);
	while (argc-- > 1) {
		TO_STDOUT("Argc: %d => %s\n", argc, argv[argc]);
	}
	TO_STDOUT("TargetPath: %s\n", Result.SpecifiedPath);
	return Result;
}