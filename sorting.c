#include "sorting.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "misc.h"

PENTRY
order_reversal(PENTRY root)
{
	PENTRY last = root;
	PENTRY t = NULL;
	/* Find last node */
	while (last->next != NULL)
		last = last->next;
	root = last;
	/* arrange in reversal order */
	while (last != NULL) {
		t = last->next;
		last->next = last->prev;
		last->prev = t;
		last = last->next;
	}
	return root;
}
void
swap_node(PENTRY *current)
{
	PENTRY tmp = *current;
	*current = (*current)->next;

	tmp->next = (*current)->next;
	(*current)->next = tmp;

	(*current)->prev = tmp->prev;
	tmp->prev = *current;

	if (tmp->next != NULL)
		tmp->next->prev = tmp;
}
int
comparer_ctime(const PENTRY a, const PENTRY b)
{
	return (a->info.st_ctime) - (b->info.st_ctime);
}
int
comparer_atime(const PENTRY a, const PENTRY b)
{
	return (a->info.st_atime) - (b->info.st_atime);
}
int
comparer_mtime(const PENTRY a, const PENTRY b)
{
	return (a->info.st_mtime) - (b->info.st_mtime);
}
int
comparer_file_size(const PENTRY a, const PENTRY b)
{
	/* -S is to order descending by file size*/
	return (b->info.st_size) - (a->info.st_size);
}
int
comparer_lexicographical(const PENTRY a, const PENTRY b)
{
	return strcmp(a->filename, b->filename);
}
void
order_by_comparer(PENTRY *rootPtr, int comparer(const PENTRY, const PENTRY))
{
	for (PENTRY *first = rootPtr, sorted = NULL, last = NULL;
	     (*first)->next != last; last = sorted) {
		PENTRY *current = first;
		sorted = (*first)->next;
		for (; (*current)->next != last; current = &(*current)->next) {
			if (comparer(*current, (*current)->next) > 0) {
				swap_node(current);
				sorted = (*current)->next;
			}
		}
	}
}
PENTRY
sort(PENTRY root, const POPTIONS options)
{
	if (options->NotSortedOutput) {	   // -f
		return root;
	} else {
		order_by_comparer(&root, comparer_lexicographical);
	}

	if (options->OrderByLastModified) {    // -t
		order_by_comparer(&root, comparer_mtime);
	} else if (options->OrderByLastChanged) {    // -c
		order_by_comparer(&root, comparer_ctime);
	} else if (options->OrderByLastAccess) {    // -u
		order_by_comparer(&root, comparer_atime);
	} else if (options->OrderDesceningBySize) {    // -S
		order_by_comparer(&root, comparer_file_size);
	}
	if (options->OrderReversal)    // -r
		return order_reversal(root);
	return root;
}