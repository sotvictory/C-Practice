void del_nechet(list *lst)
{
    int counter = 0;
    link p = *lst, q;

    if (p == NULL)
        return;

    /* first element processing */
    if (p->elem == 'a') {
        ++counter;
        q = p;
        p = p->next;
        *lst = p;
        free(q);
    }

    if (p == NULL)
        return;

    if (p->elem == 'a')
        ++counter;

    /* next elements processing */
    while (p != NULL && p->next != NULL) {
        if (p->next->elem == 'a') {
            ++counter;
            if (counter % 2 == 1) {
                q = p->next;
                p->next = q->next;
                free(q);
            }
        }
        p = p->next;
    }
    
    return;
}