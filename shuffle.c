#include "shuffle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(struct list_head *a, struct list_head *b)
{
    struct list_head *tmp = a->prev;
    list_move(a, b);
    list_move(b, tmp);
}

void shuffle(struct list_head *head)
{
    int size = q_size(head) - 1;
    struct list_head *tail = head->prev;
    while (size) {
        int i = rand() % size;
        int count = 0;
        struct list_head *tmp;
        list_for_each (tmp, head) {
            if (count == i)
                break;
            count++;
        }
        swap(tmp, tail);
        size--;
        tail = tmp->prev;
    }
}
