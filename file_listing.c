#include <sys/param.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_listing.h"
#include "misc.h"
#include "parameters.h"

char filetype_symbol[] = "aAbcdlspw-";

PENTRY
convert(struct dirent *entry, DIR *dir_stream)
{
	PENTRY result = (PENTRY)calloc_checked(1, sizeof(ENTRY));
	bzero(result, sizeof(ENTRY));

	int read_length = MAX(PATH_MAX, strlen(entry->d_name));
	strncpy(result->filename, entry->d_name, read_length);

	int fd_dir = dirfd(dir_stream);
	if (fd_dir == EXIT_FAILURE) {
		fprintf(stderr, "Failed to get fd from directory stream\n");
		exit(EXIT_FAILURE);
	}

	struct stat f_stat = { 0 };
	if (fstatat(fd_dir, entry->d_name, &f_stat, 0) == EXIT_FAILURE) {
		fprintf(stderr,
			"Failed to get stat from directory stream: %s\n",
			entry->d_name);
		exit(EXIT_FAILURE);
	}
	result->info = f_stat;
	closedir(dir_stream);

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
	/* PENTRY[] */
	PENTRY root = NULL, newNode = NULL;

	DIR *d;
	struct dirent *dir;
	int index = options->CountPaths;
	while (index > -1) {
		char *path = options->Paths[index];
		d = opendir(path);
		if (d != NULL) {
			while ((dir = readdir(d)) != NULL) {
				if (is_visible(options, dir)) {
					newNode = convert(dir, d);
					if (root != NULL)
						root->next = newNode;
					newNode->prev = root;
					root = newNode;
				}
			}
			closedir(d);
		}
		index--;
	}
	while (root->prev != NULL)
		root = root->prev;
	return root;
}
bool
is_visible(const POPTIONS options, struct dirent *entry)
{
	if (options->AllEntriesIncludeDirectory) {    // -a
		return true;
	}

	if (!options->ListAllEntries) {	   // -A
		return entry->d_name[0] != '.';
	} else if (entry->d_type == DT_DIR) {
		return !(strcmp(".", entry->d_name) == 0 ||
			 strcmp("..", entry->d_name) == 0);
	}

	return true;
}