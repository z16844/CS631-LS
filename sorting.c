#include "sorting.h"

PENTRY
order_reversal(PENTRY root)
{
	PENTRY last = root;
	PENTRY tmp = NULL;

	/* Find last node */
	while (last->next != NULL)
		last = last->next;
	root = last;
	/* arrange in reversal order */
	while (last->prev != NULL) {
		tmp = last->prev;
		last->prev = last->next;
		last = last->next = tmp;
		last = tmp;
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