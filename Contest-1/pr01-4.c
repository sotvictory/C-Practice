#include <stdlib.h>
#include <string.h>

struct Node {
    struct Node *prev, *next;
    char *elem;
};

struct List {
    struct Node *first, *last;
};

void process(struct List *pl, const char *str)
{
    struct Node *cur_node = pl->first, *last_node = pl->last, *buf_node = NULL;

	if (pl == NULL)
	    return;

	while (cur_node != last_node) {
        /* node with a string equal to str: remove */
		if (strcmp(cur_node->elem, str) == 0) {
			/* first node: change start + free up memory */
			if (cur_node == pl->first) {
				pl->first = cur_node->next;
				cur_node->next->prev = NULL;
				free(cur_node->elem);
				free(cur_node);
				cur_node = pl->first;
			/* another node: edit connections + free up memory */
			} else {
				buf_node = cur_node->next;
				cur_node->prev->next = cur_node->next;
				cur_node->next->prev = cur_node->prev;
				free(cur_node->elem);
				free(cur_node);
				cur_node = buf_node;
			}
		} else {
            /* node with a string lexicographically greater than str: move it to the end of the list */
		    if (strcmp(cur_node->elem, str) > 0) {
				/* first node */
				if (cur_node == pl->first) {
					/* move left cyclically */
					pl->first = cur_node->next;
					cur_node->next->prev = NULL;
					pl->last->next = cur_node;
					cur_node->prev = pl->last;
					cur_node->next = NULL;
					pl->last = cur_node;
					cur_node = pl->first;
                /* another node */
				} else {
                    /* cut */
					buf_node = cur_node->next;
                    cur_node->next->prev = cur_node->prev;
					cur_node->prev->next = buf_node;
                    /* insert at the end */
					pl->last->next = cur_node;
					cur_node->prev = pl->last;
					cur_node->next = NULL;
					pl->last = cur_node;
					cur_node = buf_node;
				}
			} else 
                /* if there is no need to remove or move to the end, consider the next link */
				cur_node = cur_node->next;
		}
	}

    /* process the original last node*/
    /* remove */
    if ((pl->last != NULL) && (strcmp(cur_node->elem, str) == 0)) {
        /* first node */
		if (cur_node == pl->first) {
			pl->first = cur_node->next;
			if (cur_node == pl->last)
				pl->last = NULL;
			else
				cur_node->next->prev = NULL;
        /* another node */
		} else {
			cur_node->prev->next = cur_node->next;
			if (cur_node == pl->last)
				pl->last = cur_node->prev;
			else
				cur_node->next->prev = cur_node->prev;
		}
		free(cur_node->elem);
		free(cur_node);
    /* move to the end */
	} else if ((pl->last != NULL) && (strcmp(cur_node->elem, str) > 0)) {
        /* first node */
	    if ((cur_node == pl->first) && (cur_node != pl->last)) {
			pl->first = cur_node->next;
			pl->first->prev = NULL;
        /* another node */
		} else if ((cur_node != pl->first) && (cur_node != pl->last)) {
			cur_node->prev->next = cur_node->next;
			cur_node->next->prev = cur_node->prev;
		}
        /* general restructuring */
		pl->last->next = cur_node;
		cur_node->prev = pl->last;
		cur_node->next = NULL;
		pl->last = cur_node;
	}
}