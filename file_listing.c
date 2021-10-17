#include "file_listing.h"

#include <sys/param.h>
#include <sys/ioctl.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "misc.h"
#include "parameters.h"

extern int errno;

char filetype_symbol[] = "aAbcdlspw-";
FORM_SETTING *settings = NULL;

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
convert_file(const char *path, int fd_dir, const POPTIONS options)
{
	struct stat f_stat = { 0 };

	int ret = fd_dir > 0 ? fstatat(fd_dir, path, &f_stat, 0)
			     : stat(path, &f_stat);
	if (ret != EXIT_SUCCESS) {
		fprintf(stderr, "Failed to get stat: %s - 0x%x(%s)\n", path,
			errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	PENTRY result = (PENTRY)calloc_checked(1, sizeof(ENTRY));

	int read_length = MAX(PATH_MAX, strlen(path));
	result->filename =
	    (char *)calloc_checked(read_length + 1, sizeof(char));
	strncpy(result->filename, path, read_length);

	memcpy(&result->info, &f_stat, sizeof(f_stat));
	parse_type(result, f_stat.st_mode);
	update_metadata(result, options);
	return result;
}

PENTRY
convert_directory(struct dirent *entry, DIR *dir_stream, const POPTIONS options)
{
	int fd_dir = dirfd(dir_stream);
	if (fd_dir == EXIT_FAILURE) {
		fprintf(stderr, "Failed to get fd from directory stream\n");
		exit(EXIT_FAILURE);
	}

	PENTRY result = convert_file(entry->d_name, fd_dir, options);
	parse_type(result, entry->d_type);

	return result;
}
PENTRY
travel_directory(const POPTIONS options)
{
	/* TODO: Symlinks */
	/* TODO: recursive directories(-R) */
	/* TODO: listed directories (-d) */
	/* TODO: Total blocks on the top */

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
					newNode =
					    convert_directory(dir, d, options);
					if (root != NULL)
						root->next = newNode;
					newNode->prev = root;
					root = newNode;
				}
			}
			closedir(d);
		} else {
			newNode = convert_file(path, 0, options);
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

void
update_metadata(PENTRY entry, const POPTIONS options)
{
	if (settings == NULL)
		settings =
		    (FORM_SETTING *)calloc_checked(1, sizeof(FORM_SETTING));

	int name_len = strlen(entry->filename);
	if (name_len > settings->maxFilenameLen)
		settings->maxFilenameLen = name_len;

	char *inode = itoa(entry->info.st_ino);
	size_t len_inode = strlen(inode);
	if (len_inode > settings->maxInodeLen)
		settings->maxInodeLen = len_inode;
	free(inode);

	char *size;
	if (options->HumanReadableFormat)
		size = asHumanReadable(entry->info.st_size);
	else
		size = itoa(entry->info.st_size);

	size_t len_size = strlen(size);
	if (len_size > settings->maxSizeLen)
		settings->maxSizeLen = len_size;
	free(size);

	char *hardlinks = itoa(entry->info.st_nlink);
	size_t len_hardlinks = strlen(hardlinks);
	if (len_hardlinks > settings->maxHardLinks)
		settings->maxHardLinks = len_hardlinks;
	free(hardlinks);

	char *username = options->ShowAsUidAndGid
			     ? itoa(entry->info.st_uid)
			     : getUserName(entry->info.st_uid);
	size_t len_username = strlen(username);
	if (len_username > settings->maxUserLen)
		settings->maxUserLen = len_username;

	char *group_name = options->ShowAsUidAndGid
			       ? itoa(entry->info.st_gid)
			       : getGroupName(entry->info.st_gid);
	size_t len_group_name = strlen(group_name);
	if (len_group_name > settings->maxGroupLen)
		settings->maxGroupLen = len_group_name;

	settings->numberOfEntries++;
}
FORM_SETTING *
get_metadata()
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	settings->ColumnsOfTerminal = w.ws_col;

	return settings;
}