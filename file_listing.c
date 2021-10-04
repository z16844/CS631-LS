#include <sys/param.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_listing.h"
#include "parameters.h"

char filetype_symbol[] = "aAbcdlspw-";

PENTRY
convert(struct dirent *entry, char *location)
{
	PENTRY result = (PENTRY)calloc(1, sizeof(ENTRY));
	bzero(result, sizeof(ENTRY));

	int read_length = MAX(PATH_MAX, strlen(entry->d_name));
	strncpy(result->filename, entry->d_name, read_length);

	location = NULL;
	location = location;
	// char *file_path =
	//     strncat(location, result->filename, strlen(result->filename));
	// struct stat f_stat = { 0 };
	// if (stat(file_path, &f_stat) != EXIT_SUCCESS) {
	// 	fprintf(stderr, "Failed to read stat: %s\n", file_path);
	// 	exit(EXIT_FAILURE);
	// }

	switch (entry->d_type) {
	case DT_BLK:
		result->type = Block_special_file;
		break;
	case DT_CHR:
		result->type = Character_special_file;
		break;
	case DT_DIR:
		result->type = Directory;
		break;
	case DT_FIFO:
		result->type = FIFO;
		break;
	case DT_SOCK:
		result->type = Socket_link;
		break;
	case DT_WHT:
		result->type = Whiteout;
		break;
	case DT_LNK:
		result->type = Symbolic_link;
		break;
	case DT_REG:
	default:
		result->type = Regular_file;
		break;
	}

	return result;
}
PENTRY
travel_directory(const POPTIONS options)
{

	PENTRY query_set = (PENTRY)calloc(1, sizeof(ENTRY));
	bzero(query_set, sizeof(ENTRY));

	DIR *d;
	struct dirent *dir;
	int index = options->CountPaths;
	while (index > -1) {
		char *path = options->Paths[index--];
		d = opendir(path);
		if (d != NULL) {
			while ((dir = readdir(d)) != NULL) {
				if (is_visible(options, dir))
					printf("%s\t", dir->d_name);
			}
			closedir(d);
		}
	}
	return query_set;
}
bool
is_visible(const POPTIONS options, struct dirent *entry)
{
	if (options->ListAllEntries && entry->d_type == DT_DIR) {
		if (strcmp(".", entry->d_name) == 0 ||
		    strcmp("..", entry->d_name) == 0) {
			return false;
		}
	}
	if (!options->IncludeDirectoryEntries) {
		if (entry->d_name[0] == '.')
			return false;
	}
	return true;
}