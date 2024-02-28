#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_node = malloc(sizeof(struct list_head));
    if (!new_node)
        return NULL;
    INIT_LIST_HEAD(new_node);
    return new_node;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *next = head->next;
    while (next != head) {
        element_t *node = list_entry(next, element_t, list);
        next = next->next;
        q_release_element(node);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node)
        return false;

    char *tmp = strdup(s);
    if (!tmp) {
        free(new_node);
        return false;
    }

    new_node->value = tmp;
    list_add(&new_node->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node)
        return false;

    char *tmp = strdup(s);
    if (!tmp) {
        free(new_node);
        return false;
    }

    new_node->value = tmp;
    list_add_tail(&new_node->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *node = list_first_entry(head, element_t, list);
    list_del(&node->list);

    if (sp) {
        memcpy(sp, node->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *node = list_last_entry(head, element_t, list);
    list_del(&node->list);

    if (sp) {
        memcpy(sp, node->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int count = 0;
    struct list_head *node;
    list_for_each (node, head)
        count++;
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    struct list_head **indir = &head->next;
    for (struct list_head *fast = head->next;
         fast != head && fast->next != head; fast = fast->next->next)
        indir = &(*indir)->next;
    element_t *node = list_entry(*indir, element_t, list);
    list_del(*indir);
    q_release_element(node);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *node = head->next;
    element_t *e1;
    struct list_head *safe;
    while (node && node != head && node->next != head) {
        e1 = list_entry(node, element_t, list);
        element_t *e2 = list_entry(node->next, element_t, list);
        safe = node->next;
        if (!strcmp(e1->value, e2->value)) {
            do {
                list_del(&(e2->list));
                safe = safe->next;
                q_release_element(e2);
                if (safe == head)
                    break;
                e2 = list_entry(safe, element_t, list);
            } while (!strcmp(e1->value, e2->value));
            list_del(&e1->list);
            q_release_element(e1);
        }
        node = safe;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head))
        return;

    struct list_head *cur = head->next;
    while (cur != head && cur->next != head) {
        list_move(cur, cur->next);
        cur = cur->next;
    }
}

void q_print(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    element_t *it = NULL;
    list_for_each_entry (it, head, list) {
        printf("%s ", it->value);
    }
    printf("\n");
}
/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *node;
    struct list_head *safe;

    list_for_each_safe (node, safe, head) {
        list_move(node, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void quick_sort(struct list_head *head, struct list_head *end);
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return;
    quick_sort(head, head);
}

void quick_sort(struct list_head *head, struct list_head *end)
{
    struct list_head *safe;
    struct list_head *pivot = head->next;
    char *value = list_entry(pivot, element_t, list)->value;
    struct list_head *node = pivot->next;
    while (node != end) {
        element_t *elem = list_entry(node, element_t, list);
        safe = node->next;
        if (strcmp(elem->value, value) < 0) {
            list_move_tail(node, pivot);
        } else {
            list_move(node, pivot);
        }
        node = safe;
    }
    if (head->next != pivot && head->next->next != pivot)
        quick_sort(head, pivot);
    if (pivot->next != end && pivot->next->next != end)
        quick_sort(pivot, end);
}
/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
