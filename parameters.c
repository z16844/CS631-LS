#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "parameters.h"

extern int errno;

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

	result = calloc(strlen(abs_path) + 1, sizeof(char));
	strcpy(result, abs_path);

	return EXIT_SUCCESS;
}
