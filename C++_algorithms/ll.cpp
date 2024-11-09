#include <iostream>
#include "ll.h"


void ll::insert_beg(int v)
{
    node *new_node = (node*) malloc(sizeof(node));
    new_node->value = v;
    new_node->next = head;
    head = new_node;
    return;
}

void ll::print()
{
    node *tmp = head;
    while (tmp != nullptr)
    {
        printf("%i\n", tmp->value);
        tmp = tmp->next;
    }
}

int ll::operator [](int index)
{
    int return_value = 0;
    node *itr = head;
    for (int i=0; i<=index; i++)
    {
        return_value = itr->value;
        itr = itr->next;

    }
    return return_value;
}

void ll::operator +(ll list_2)
{
    node *itr = head;
    while (itr->next != nullptr)
    {
        itr = itr->next;
    }
    itr->next = list_2.head;

}