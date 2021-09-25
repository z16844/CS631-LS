#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#ifndef _MISC_H_
#define _MISC_H_

#define TO_STDOUT(format, ...) fprintf(stdout, format, __VA_ARGS__)
#define TO_STDERR(format, ...) fprintf(stderr, format, __VA_ARGS__)

#define FILENAME(path) basename(path)

#endif /* !_MISC_H_ */