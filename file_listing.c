#include <dirent.h>
#include <stdio.h>

#include "file_listing.h"
#include "parameters.h"

void
travel_directory(const POPTIONS options)
{
	DIR *d;
	struct dirent *dir;
	int index = options->CountPaths;
	while (index > -1) {
		char *path = options->Paths[index--];
		d = opendir(path);
		if (d != NULL) {
			while ((dir = readdir(d)) != NULL) {
				printf("%s\t\t", dir->d_name);
			}
			closedir(d);
		}
	}
}