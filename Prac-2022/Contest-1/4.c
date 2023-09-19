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
    struct Node *curNode = pl->first, *lastNode = pl->last, *buffNode = NULL;

    // тривиальный случай
	if (pl == NULL)
	    return;

    // последнее звено - особый случай
	while (curNode != lastNode) {
        // звено со строкой, равной str, удалить
		if (strcmp(curNode->elem, str) == 0) {
            // первое звено, поменять начало + освободить память
			if (curNode == pl->first) {
				pl->first = curNode->next;
				curNode->next->prev = NULL;
				free(curNode->elem);
				free(curNode);
				curNode = pl->first;
            // промежуточное звено, подредактировать связи слева и справа + освободить память
			} else {
				buffNode = curNode->next;
				curNode->prev->next = curNode->next;
				curNode->next->prev = curNode->prev;
				free(curNode->elem);
				free(curNode);
				curNode = buffNode;
			}
		} else {
            // нашли звено со строкой, лексикографически большей str, переставить в конец списка
		    if (strcmp(curNode->elem, str) > 0) {
                // первое звено, циклически сдвинуть влево
				if (curNode == pl->first) {
					pl->first = curNode->next;
					curNode->next->prev = NULL;
					pl->last->next = curNode;
					curNode->prev = pl->last;
					curNode->next = NULL;
					pl->last = curNode;
					curNode = pl->first;
                // промежуточное звено, вырезать и вставить в конец
				} else {
                    // вырезать
					buffNode = curNode->next;
                    curNode->next->prev = curNode->prev;
					curNode->prev->next = buffNode;
                    // вставить в конец
					pl->last->next = curNode;
					curNode->prev = pl->last;
					curNode->next = NULL;
					pl->last = curNode;
					curNode = buffNode;
				}
			} else 
                // если не надо удалять или перемещать в конец, рассмотреть следующее звено
				curNode = curNode->next;
		}
	}

    // обработка исходного последнего звена
    // удалить
    if ((pl->last != NULL) && (strcmp(curNode->elem, str) == 0)) {
        // первое
		if (curNode == pl->first) {
			pl->first = curNode->next;
			if (curNode == pl->last)
				pl->last = NULL;
			else
				curNode->next->prev = NULL;
        // промежуточное
		} else {
			curNode->prev->next = curNode->next;
			if (curNode == pl->last)
				pl->last = curNode->prev;
			else
				curNode->next->prev = curNode->prev;
		}
        // освободить память
		free(curNode->elem);
		free(curNode);
    // переставить
	} else if ((pl->last != NULL) && (strcmp(curNode->elem, str) > 0)) {
        // первое
	    if ((curNode == pl->first) && (curNode != pl->last)) {
			pl->first = curNode->next;
			pl->first->prev = NULL;
        // промежуточное
		} else if ((curNode != pl->first) && (curNode != pl->last)) {
			curNode->prev->next = curNode->next;
			curNode->next->prev = curNode->prev;
		}
        // общая перестройка
		pl->last->next = curNode;
		curNode->prev = pl->last;
		curNode->next = NULL;
		pl->last = curNode;
	}
}