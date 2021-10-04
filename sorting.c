#include "sorting.h"

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
	if (options->ReverseTheLexicographicalOrder)
		return order_reversal(root);
	return root;
}