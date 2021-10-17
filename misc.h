
#include <math.h>
#include <stdio.h>
#include <time.h>

#ifndef _MISC_H_
#define _MISC_H_

void *
calloc_checked(int count, size_t type_size);

char *
dtoa(double value);

char *
itoa(int value);

char *
parse_timestamp(time_t *tm);

char *
getUserName(uid_t uid);

char *
getGroupName(gid_t gid);

#endif /* !_MISC_H_ */