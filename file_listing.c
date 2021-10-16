#include "file_listing.h"

#include <sys/param.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "parameters.h"

char filetype_symbol[] = "aAbcdlspw-";

extern int errno;
void
parse_type(PENTRY result, const mode_t st_mode)
{
	switch (st_mode) {
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
}
PENTRY
convert_file(const char *path)
{
	struct stat f_stat = { 0 };
	if (stat(path, &f_stat) != EXIT_SUCCESS) {
		fprintf(
		    stderr,
		    "Failed to get stat from directory stream: %s - 0x%x(%s)\n",
		    path, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	PENTRY result = (PENTRY)calloc_checked(1, sizeof(ENTRY));
	strncpy(result->filename, path, strlen(path));

	// result->info = f_stat;
	memcpy(&result->info, &f_stat, sizeof(f_stat));
	parse_type(result, f_stat.st_mode);
	return result;
}

PENTRY
convert_directory(struct dirent *entry, DIR *dir_stream)
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
		fprintf(
		    stderr,
		    "Failed to get stat from directory stream: %s - 0x%x(%s)\n",
		    entry->d_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	// result->info = f_stat;
	memcpy(&result->info, &f_stat, sizeof(f_stat));
	closedir(dir_stream);
	parse_type(result, entry->d_type);

	return result;
}
PENTRY
travel_directory(const POPTIONS options)
{
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
					newNode = convert_directory(dir, d);
					if (root != NULL)
						root->next = newNode;
					newNode->prev = root;
					root = newNode;
				}
				closedir(d);
			}
		} else {
			newNode = convert_file(path);
			if (root != NULL)
				root->next = newNode;
			newNode->prev = root;
			root = newNode;
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