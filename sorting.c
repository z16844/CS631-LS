#include "sorting.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "misc.h"
// void
// order_lexicographical(PENTRY root)
// {
// 	PENTRY cur = root;
// 	PENTRY cur2 = root;
// 	bool hasChanged = true;
// 	while (hasChanged) {
// 		hasChanged = false;
// 		while (cur2) {
// 			/* strcasecmp is same as strcmp but case-insensitve */
// 			if (strcasecmp(cur2->filename, cur2->next->filename) >
// 			    0) {
// 				hasChanged = true;
// 				swap_node(cur2, cur2->next);
// 				cur2->prev = cur2;
// 			}
// 			cur2 = cur2->next;
// 		}
// 		// Still working on it
// 		cur = cur->next;
// 	}
// 	root = cur;
// }

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
comparer_filename(const PENTRY a, const PENTRY b)
{
	return strcmp(a->filename, b->filename);
}
void
order_by_comparer(LIST *_list, int comparer(const PENTRY, const PENTRY))
{
	bool hasInitial = true;
	for (PENTRY *first = &_list->head, sorted = NULL, last = NULL;
	     (*first)->next != last; last = sorted) {
		PENTRY *current = first;
		sorted = (*first)->next;
		for (; (*current)->next != last; current = &(*current)->next) {
			if (comparer(*current, (*current)->next) > 0) {
				swap_node(current);
				sorted = (*current)->next;
			}
		}
		if (hasInitial) {
			_list->tail = *current;
			hasInitial = false;
		}
	}
}
PENTRY
sort(PENTRY root, const POPTIONS options)
{
	LIST list;
	list.head = root;
	if (options->OrderByLastModifiedAscending) {
	} else {
		order_by_comparer(&list, comparer_filename);
	}
	// order_lexicographical(root);
	if (options->ReverseTheLexicographicalOrder)
		return order_reversal(root);
	return list.head;
}