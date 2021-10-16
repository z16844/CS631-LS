#include "misc.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

void *
calloc_checked(int count, size_t type_size)
{
	if (count < 1) {
		fprintf(stderr,
			"Invalid parameter: calloc_checked (count = %d)\n",
			count);
		exit(EXIT_FAILURE);
	}

	void *allocated = calloc(count, type_size);
	if (allocated == NULL) {
		fprintf(stderr, "Failed to allocate memory: 0x%x (%s)\n", errno,
			strerror(errno));
		exit(EXIT_FAILURE);
	}

	if ((count * type_size) > SIZE_MAX) {
		fprintf(stderr,
			"Overflow during memory allocation: 0x%x (%s)\n",
			EOVERFLOW, strerror(EOVERFLOW));
		exit(EXIT_FAILURE);
	}

	bzero(allocated, count * type_size);
	return allocated;
}
char *
itoa(int value)
{
	int len = (value / 10) + 1;    // without NULL-terminator
	char *buf = (char *)calloc_checked(len + 1, sizeof(char));
	if (sprintf(buf, "%d", value) == -1) {
		fprintf(stderr, "Invalid Parameter to itoa()");
		exit(EXIT_FAILURE);
	}
	return buf;
}
char *
parse_timestamp(time_t *time)
{
	char *result = (char *)calloc_checked(13, sizeof(char));
	struct tm *t;
	t = localtime(time);

	strftime(result, 13, "%b %e %R", t);
	return result;
}