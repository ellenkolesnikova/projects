// Implements a list of numbers with linked list

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <sys/stat.h>

// Represents a node

typedef struct node
{
    int number;
    struct node *next;
}
node;

node * insert_at_beginning(node *list, int new_number);
node * insert_at_end(node *list, int new_number);
node * insert_at_index(node *list, int new_number, int index);
node * remove_at_index(node *list, int index);
node * remove_by_value(node *list, int value);
node * remove_ith_from_end(node *list, int index);
node * recursive_reverse(node *list);
bool has_loop(node *list);
int get_ith(node *list, int index);
int get_length(node *list);
int print(node *list);
int FREE(node *list);



int main(void)
{
    // List of size 0
    node *list = NULL;


    /*list = insert_at_beginning(list, 0);
    list = insert_at_end(list, 4);
    list = insert_at_end(list, 5);
    list = insert_at_end(list, 6);
    list = insert_at_end(list, 7);
    list = insert_at_end(list, 8);*/
    print(list);
    printf("\n");
    list = recursive_reverse(list);
    print(list);

    FREE(list);


    return 0;
}

// Print function

int print(node *list)
{
    for (node *tmp = list; tmp != NULL; tmp = tmp->next)
    {
        printf("%i\n", tmp->number);
    }
    return 0;
}

// Free function

int FREE(node *list)
{
    while (list != NULL)
    {
        node *tmp = list->next;
        free(list);
        list = tmp;
    }
    return 0;
}

// Insert at beginning function

node * insert_at_beginning(node *list, int new_number)
{
    node *new_node = malloc(sizeof(new_node));
    node *tmp = list;
    list = new_node;
    new_node->number = new_number;
    new_node->next = tmp;
    return list;
}


// Insert at end function

node * insert_at_end(node *list, int new_number)
{
    node *itr = list;
    node *new_node = malloc(sizeof(new_node));
    while (itr->next != NULL)
    {
        itr = itr->next;
    }
    itr->next = new_node;
    new_node->number = new_number;
    return list;
}

// Insert at index function

node * insert_at_index(node *list, int new_number, int index)
{
    if (index == 0)
    {
        list = insert_at_beginning(list, new_number);
        return list;
    }
    node *itr = list;
    node *new_node = malloc(sizeof(new_node));
    int count = 0;
    while (count != index-1)
    {
        itr = itr->next;
        count ++;
        if (itr == NULL)
        {
            printf("Indexing error - index out of range\n");
            return list;
        }

    }
    node *tmp = itr->next;
    itr->next = new_node;
    new_node->number = new_number;
    new_node->next = tmp;
    return list;
}


// Get length function

int get_length(node *list)
{
    int count = 0;
    node *itr = list;
    while (itr != NULL)
    {
        count ++;
        itr = itr->next;
    }
    return count;
}

// Remove at index function

node * remove_at_index(node *list, int index)
{
    node *itr = list;
    if (index == 0)
    {
        list = list->next;
        return list;
    }
    int count = 0;
    while (count < index-1)
    {
        itr = itr->next;
        count ++;
        if (itr == NULL)
        {
            printf("Indexing error - index out of range\n");
            break;
        }
    }
    node *n = itr->next;// n now points to the node being deleted
    itr->next = n->next;
    //free(n);
    return list;
}


// Get ith element of the list function

int get_ith(node *list, int index)
{
    if (index<0 || index >= get_length(list))
    {
        printf("Indexing error - index out of range\n");
        return 0;
    }
    int count = 0;
    node *itr = list;
    while (count < index)
    {
        itr = itr->next;
        count ++;
    }
    return itr->number;
}

// Remove by value function

node * remove_by_value(node *list, int value)
{
    node *itr = list;
    if (value == list->number)
    {
        list = itr->next;
        return list;
    }
    while (itr->next->number != value)
    {
        itr = itr->next;
    }
    itr->next = itr->next->next;
    return list;
}


// Has loop function

bool has_loop(node *list)
{
    node *pointer_a = list;
    node *pointer_b = list;
    while (pointer_a->next->next != NULL)
    {
        pointer_a = pointer_a->next->next;
        pointer_b = pointer_b->next;
    }
    if (pointer_a == pointer_b)
    {
        return true;
    }
    return false;
}

// Remove ith element from the end function

node * remove_ith_from_end(node *list, int index)
{
    int length = get_length(list);
    index = length - 1 - index;
    list = remove_at_index(list, index);
    return list;
}

// Reverse the list recursively

node * recursive_reverse(node *list)
{
    node *ll2;


    node *list_beginning = list;
    list = list->next;
    if (list->next != NULL)
    {
        ll2 = recursive_reverse(list);
    }
    else
    {
        ll2 = list;
    }
    list->next = list_beginning;
    list_beginning->next = NULL;
    return ll2;


    //ll2 = insert_at_end(list, list_beginning->number);


}
