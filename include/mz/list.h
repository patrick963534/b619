#ifndef __MZ_LIST_H__
#define __MZ_LIST_H__

/**
 Modified verdion of linux kernl 'include/linux/list.h'
*/

/*
* Simple doubly linked list implementation.
*
* Some of the internal functions ("__xxx") are useful when
* manipulating whole lists rather than single entries, as
* sometimes we already know the next/prev entries and we can
* generate better code by using them directly rather than
* using the generic single-entry routines.
*/

typedef struct list_s list_t;

struct list_s {
    list_t *next, *prev;
};


#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    list_t name = LIST_HEAD_INIT(name)

static __inline void INIT_LIST_HEAD(list_t *list)
{
    list->next = list;
    list->prev = list;
}

/*
* Insert a new entry between two known consecutive entries.
*
* This is only for internal list manipulation where we know
* the prev/next entries already!
*/
static __inline void __list_add(list_t *_new,
                                list_t *prev,
                                list_t *next)
{
    next->prev = _new;
    _new->next = next;
    _new->prev = prev;
    prev->next = _new;
}

/**
* list_add - add a new entry
* @_new: new entry to be added
* @head: list head to add it after
*
* Insert a new entry after the specified head.
* This is good for implementing stacks.
*/
static __inline void list_add(list_t *_new, list_t *head)
{
    __list_add(_new, head, head->next);
}


/**
* list_add_tail - add a new entry
* @_new: new entry to be added
* @head: list head to add it before
*
* Insert a new entry before the specified head.
* This is useful for implementing queues.
*/
static __inline void list_add_tail(list_t *_new, list_t *head)
{
    __list_add(_new, head->prev, head);
}

/*
* Delete a list entry by making the prev/next entries
* point to each other.
*
* This is only for internal list manipulation where we know
* the prev/next entries already!
*/
static __inline void __list_del(list_t * prev, list_t * next)
{
    next->prev = prev;
    prev->next = next;
}

/**
* list_del - deletes entry from list.
* @entry: the element to delete from the list.
* Note: list_empty() on entry does not return true after this, the entry is
* in an undefined state.
*/
static __inline void list_del(list_t *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = (list_t *) 0;
    entry->prev = (list_t *) 0;
}

/**
* list_replace - replace old entry by new one
* @old : the element to be replaced
* @_new : the new element to insert
*
* If @old was empty, it will be overwritten.
*/
static __inline void list_replace(list_t *old, list_t *_new)
{
    _new->next = old->next;
    _new->next->prev = _new;
    _new->prev = old->prev;
    _new->prev->next = _new;
}

static __inline void list_replace_init(list_t *old, list_t *_new)
{
    list_replace(old, _new);
    INIT_LIST_HEAD(old);
}

/**
* list_del_init - deletes entry from list and reinitialize it.
* @entry: the element to delete from the list.
*/
static __inline void list_del_init(list_t *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

/**
* list_move - delete from one list and add as another's head
* @list: the entry to move
* @head: the head that will precede our entry
*/
static __inline void list_move(list_t *list, list_t *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}

/**
* list_move_tail - delete from one list and add as another's tail
* @list: the entry to move
* @head: the head that will follow our entry
*/
static __inline void list_move_tail(list_t *list, list_t *head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
}

/**
* list_is_last - tests whether @list is the last entry in list @head
* @list: the entry to test
* @head: the head of the list
*/
static __inline int list_is_last(const list_t *list, const list_t *head)
{
    return list->next == head;
}

/**
* list_empty - tests whether a list is empty
* @head: the list to test.
*/
static __inline int list_empty(const list_t *head)
{
    return head->next == head;
}


/**
* list_is_singular - tests whether a list has just one entry.
* @head: the list to test.
*/
static __inline int list_is_singular(const list_t *head)
{
    return !list_empty(head) && (head->next == head->prev);
}

static __inline void __list_cut_position(list_t *list, list_t *head, list_t *entry)
{
    list_t *new_first = entry->next;
    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    head->next = new_first;
    new_first->prev = head;
}

/**
* list_cut_position - cut a list into two
* @list: a new list to add all removed entries
* @head: a list with entries
* @entry: an entry within head, could be the head itself
*   and if so we won't cut the list
*
* This helper moves the initial part of @head, up to and
* including @entry, from @head to @list. You should
* pass on @entry an element you know is on @head. @list
* should be an empty list or a list you do not care about
* losing its data.
*
*/
static __inline void list_cut_position(list_t *list,
                                       list_t *head, list_t *entry)
{
    if (list_empty(head))
        return;
    if (list_is_singular(head) &&
        (head->next != entry && head != entry))
        return;
    if (entry == head)
        INIT_LIST_HEAD(list);
    else
        __list_cut_position(list, head, entry);
}

static __inline void __list_splice(const list_t *list,
                                   list_t *prev,
                                   list_t *next)
{
    list_t *first = list->next;
    list_t *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

/**
* list_splice - join two lists, this is designed for stacks
* @list: the new list to add.
* @head: the place to add it in the first list.
*/
static __inline void list_splice(const list_t *list, list_t *head)
{
    if (!list_empty(list))
        __list_splice(list, head, head->next);
}

/**
* list_splice_tail - join two lists, each list being a queue
* @list: the new list to add.
* @head: the place to add it in the first list.
*/
static __inline void list_splice_tail(list_t *list, list_t *head)
{
    if (!list_empty(list))
        __list_splice(list, head->prev, head);
}

/**
* list_splice_init - join two lists and reinitialise the emptied list.
* @list: the new list to add.
* @head: the place to add it in the first list.
*
* The list at @list is reinitialised
*/
static __inline void list_splice_init(list_t *list, list_t *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head, head->next);
        INIT_LIST_HEAD(list);
    }
}

/**
* list_splice_tail_init - join two lists and reinitialise the emptied list
* @list: the new list to add.
* @head: the place to add it in the first list.
*
* Each of the lists is a queue.
* The list at @list is reinitialised
*/
static __inline void list_splice_tail_init(list_t *list, list_t *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head->prev, head);
        INIT_LIST_HEAD(list);
    }
}

/**
* list_entry - get the struct for this entry
* @ptr: the &list_t pointer.
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*/
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
* list_first_entry - get the first element from a list
* @ptr: the list head to take the element from.
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*
* Note, that list is expected to be not empty.
*/
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)


#define list_last_entry(ptr, type, member) \
    list_entry((ptr)->prev, type, member)

/**
* list_for_each -   iterate over a list
* @pos: the &list_t to use as a loop cursor.
* @head:    the head for your list.
*/
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
* list_for_each_prev    -   iterate over a list backwards
* @pos: the &list_t to use as a loop cursor.
* @head:    the head for your list.
*/
#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
* list_for_each_safe - iterate over a list safe against removal of list entry
* @pos: the &list_t to use as a loop cursor.
* @n:       another &list_t to use as temporary storage
* @head:    the head for your list.
*/
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
    pos = n, n = pos->next)

/**
* list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
* @pos: the &list_t to use as a loop cursor.
* @n:       another &list_t to use as temporary storage
* @head:    the head for your list.
*/
#define list_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev; \
    pos != (head); \
    pos = n, n = pos->prev)

/**
* list_for_each_entry   -   iterate over list of given type
* @pos: the type * to use as a loop cursor.
* @head:    the head for your list.
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*/
#define list_for_each_entry(pos, head, type, member)    \
    for (pos = list_entry((head)->next, type, member);  \
    &pos->member != (head);     \
    pos = list_entry(pos->member.next, type, member))

/**
* list_for_each_entry_reverse - iterate backwards over list of given type.
* @pos: the type * to use as a loop cursor.
* @head:    the head for your list.
* @member:  the name of the list_struct within the struct.
*/
#define list_for_each_entry_reverse(pos, head, type, member)            \
    for (pos = list_entry((head)->prev, type, member);  \
    &pos->member != (head);     \
    pos = list_entry(pos->member.prev, type, member))

/**
* list_prepare_entry - prepare a pos entry for use in list_for_each_entry_continue()
* @pos: the type * to use as a start point
* @head:    the head of the list
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*
* Prepares a pos entry for use as a start point in list_for_each_entry_continue().
*/
#define list_prepare_entry(pos, head, type, member) \
    ((pos) ? list_entry(head, type, member) : NULL)

/**
* list_for_each_entry_continue - continue iteration over list of given type
* @pos: the type * to use as a loop cursor.
* @head:    the head for your list.
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*
* Continue to iterate over list of given type, continuing after
* the current position.
*/
#define list_for_each_entry_continue(pos, head, type, member)       \
    for (pos = list_entry(pos->member.next, type, member);  \
    &pos->member != (head); \
    pos = list_entry(pos->member.next, type, member))

/**
* list_for_each_entry_continue_reverse - iterate backwards from the given point
* @pos: the type * to use as a loop cursor.
* @head:    the head for your list.
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*
* Start to iterate over list of given type backwards, continuing after
* the current position.
*/
#define list_for_each_entry_continue_reverse(pos, head, type, member)       \
    for (pos = list_entry(pos->member.prev, type, member); \
    &pos->member != (head); \
    pos = list_entry(pos->member.prev, type, member))

/**
* list_for_each_entry_from - iterate over list of given type from the current point
* @pos: the type * to use as a loop cursor.
* @head:    the head for your list.
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*
* Iterate over list of given type, continuing from current position.
*/
#define list_for_each_entry_from(pos, head, type, member)           \
    for (;&pos->member != (head); pos = list_entry(pos->member.next, type, member))

/**
* list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
* @pos: the type * to use as a loop cursor.
* @n:       another type * to use as temporary storage
* @head:    the head for your list.
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*/
#define list_for_each_entry_safe(pos, n, head, type, member)            \
    for (pos = list_entry((head)->next, type, member),  \
    n = list_entry(pos->member.next, type, member); \
    &pos->member != (head);                     \
    pos = n, n = list_entry(n->member.next, type, member))

/**
* list_for_each_entry_safe_continue
* @pos: the type * to use as a loop cursor.
* @n:       another type * to use as temporary storage
* @head:    the head for your list.
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*
* Iterate over list of given type, continuing after current point,
* safe against removal of list entry.
*/
#define list_for_each_entry_safe_continue(pos, n, head, type, member)       \
    for (pos = list_entry(pos->member.next, type, member),      \
    n = list_entry(pos->member.next, type, member);     \
    &pos->member != (head);                     \
    pos = n, n = list_entry(n->member.next, type, member))

/**
* list_for_each_entry_safe_from
* @pos: the type * to use as a loop cursor.
* @n:       another type * to use as temporary storage
* @head:    the head for your list.
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*
* Iterate over list of given type from current point, safe against
* removal of list entry.
*/
#define list_for_each_entry_safe_from(pos, n, head, type, member)           \
    for (n = list_entry(pos->member.next, type, member);        \
    &pos->member != (head);                     \
    pos = n, n = list_entry(n->member.next, type, member))

/**
* list_for_each_entry_safe_reverse
* @pos: the type * to use as a loop cursor.
* @n:       another type * to use as temporary storage
* @head:    the head for your list.
* @type:    the type of the struct this is embedded in.
* @member:  the name of the list_struct within the struct.
*
* Iterate backwards over list of given type, safe against removal
* of list entry.
*/
#define list_for_each_entry_safe_reverse(pos, n, head, type, member)        \
    for (pos = list_entry((head)->prev, type, member),  \
    n = list_entry(pos->member.prev, type, member); \
    &pos->member != (head);                     \
    pos = n, n = list_entry(n->member.prev, type, member))



#endif

