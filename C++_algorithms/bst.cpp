#include <iostream>
#include "bst.h"
#include <algorithm>


void bst::insert(int new_value)
{
    if (value == MIN)
    {
        value = new_value;
        return;
    }
    else if (new_value > value)
    {
        if (right_child == nullptr)
        {
            bst *new_bst = new bst();
            right_child = new_bst;
            new_bst->value = new_value;
            return;
        }
        else
        {
            right_child->insert(new_value);
        }
    }
    else if (new_value < value)
    {
        if (left_child == nullptr)
        {
            bst *new_bst = new bst();
            left_child = new_bst;
            new_bst->value = new_value;
            return;
        }
        else
        {
            left_child->insert(new_value);
        }
    }

}

void bst::print()
{
    if (value != MIN)
    {
        if (left_child != nullptr)
        {
            left_child->print();
        }
        if (deleted != true)
        {
            printf("%i\n", value);
        }
        if (right_child != nullptr)
        {
            right_child->print();
        }

    }
}



void bst::FREE()
{
    if (right_child != nullptr)
    {
        right_child->FREE();
        delete right_child;
    }
    if (left_child != nullptr)
    {
        left_child->FREE();
        delete left_child;
    }
}


void bst::remove_from_tree(int rm_value)
{
    if (rm_value == value)
    {
        deleted = true;
        return;
    }
    if (rm_value > value && right_child != nullptr)
    {
        right_child->remove_from_tree(rm_value);
    }
    else if (rm_value < value && left_child != nullptr)
    {
        left_child->remove_from_tree(rm_value);
    }

}
int bst::count = 0;

void bst::balance_tree()
{
    int array[MAX_SIZE];
    count = 0;
    this->tree_to_array(array);
    std::cout<<"\n";
    int size = count;
    bst *new_bst;
    new_bst = this->array_to_tree(array, count);
    value = new_bst->value;
    right_child = new_bst->right_child;
    left_child = new_bst->left_child;
    return;

}


void bst::tree_to_array(int array[])
{
    if (left_child != nullptr)
    {
        left_child->tree_to_array(array);
    }
    if (deleted != true)
    {
        array[count] = value;
        count++;
    }
    if (right_child != nullptr)
    {
        right_child->tree_to_array(array);
    }
}


bst * bst::array_to_tree(int array[], int size)
{
    bst * mid_node = new bst();
    if (size > 0)
    {
        int half_size = (size % 2 == 0 ? size/2 : (size-1)/2);

        // mid is pointer to right sub tree
        int *mid = &(array[half_size+1]);

        bst *left_tree = new bst();
        left_tree = left_tree->array_to_tree(array, half_size);
        mid_node->value = array[half_size];
        mid_node->left_child = left_tree;
        mid_node->deleted = false;
        bst *right_tree = new bst();
        right_tree = right_tree->array_to_tree(mid, (size-half_size-1));
        mid_node->right_child = right_tree;
    }
    return mid_node;
}

int bst::tree_depth()
{
    int left = 0;
    int right = 0;
    if (left_child != nullptr)
    {
        left = left_child->tree_depth();
    }
    if (right_child != nullptr)
    {
        right = right_child->tree_depth();
    }

    int a = 1+std::max(left, right);
    return a;
}


void bst::check_print()
{
    if (right_child != nullptr)
    {
        std::cout << "right: " << value << "\n";
        right_child->check_print();
    }
}