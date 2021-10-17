#include "misc.h"

#include <errno.h>
#include <grp.h>
#include <pwd.h>
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
dtoa(double value)
{
	char *result;
	char buf[15] = { 0 };
	size_t len = snprintf(buf, 10, "%.01f", value);

	/* snprintf doesn't count NULL-termination */
	result = (char *)calloc_checked(len + 1, sizeof(char));
	strncpy(result, buf, len);
	return result;
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

char *
getGroupName(gid_t gid)
{
	struct group *g = NULL;
	if ((g = getgrgid(gid)) != NULL) {
		return g->gr_name;
	}
	if (errno != 0) {
		fprintf(stderr,
			"Failed to get group name from gid: %d - 0x%x(%s)\n",
			gid, errno, strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		return itoa(gid);
	}
}
char *
getUserName(uid_t uid)
{
	struct passwd *pw = NULL;
	if ((pw = getpwuid(uid)) != NULL) {
		return pw->pw_name;
	}
	if (errno != 0) {
		fprintf(stderr,
			"Failed to get user name from uid: %d - 0x%x(%s)\n",
			uid, errno, strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		return itoa(uid);
	}
}