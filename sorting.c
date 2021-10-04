#include <stdbool.h>

#include "misc.h"
#include "sorting.h"

void
order_lexicographical(PENTRY root)
{
	PENTRY t = NULL;
	PENTRY cur = root;
	bool hasChanged = true;
	while (hasChanged) {
		hasChanged = false;
		while (cur->next != NULL) {
			if (strcmp(cur->filename, cur->next->filename) > 0) {
				hasChanged = true;
				t = cur
			}
		}
		// Still working on it
		cur = cur->next;
	}
	return cursor;
}

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

PENTRY
sort(PENTRY root, const POPTIONS options)
{
	// order_lexicographical(root);
	if (options->ReverseTheLexicographicalOrder)
		return order_reversal(root);
	return root;
}