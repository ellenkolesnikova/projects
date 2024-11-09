#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <sys/stat.h>
#include <math.h>

typedef struct node
{
    int value;
    struct node *left_child;
    struct node *right_child;
    bool deleted;
}
node;

#define MAX(a, b) ((a) > (b) ? (a) : (b));

int insert(node *cur_node, int new_value);
void print(node *cur_node);
node* create_BST(int value);
void insert_random_numbers(int amount, int highest, node *tree);
void FREE(node *cur_node);
void remove_from_tree(node *cur_node, int value);
void tree_to_array(node *cur_node, int array[]);
node * array_to_tree(int array[], int size);
node * balance_tree(node *tree);
int depth(node *cur_node);


const int MAX_SIZE = 1000;
int count = 0;


int main(void)
{
    node *tree = create_BST(10);
    insert(tree, 0);
    insert(tree, 1);
    insert(tree, 2);
    insert(tree, 3);
    insert(tree, 4);
    insert(tree, 5);
    //insert_random_numbers(5, 100000, tree);
    print(tree);
    printf("depth is %i\n", depth(tree));
    //remove_from_tree(tree, 4);
    tree = balance_tree(tree);
    printf("after balancing, depth is %i\n", depth(tree));

    //print(tree);
    FREE(tree);
}

node* create_BST(int value)
{
    node *tree = malloc(sizeof(node));
    tree->value = value;
    return tree;
}

int insert(node *cur_node, int new_value)
{
    if (new_value < cur_node->value)
    {
        if (cur_node->left_child == NULL)
        {
            node *new_node = malloc(sizeof(node));
            cur_node->left_child = new_node;
            new_node->value = new_value;
            new_node->deleted = false;
        }
        else
        {
            insert(cur_node->left_child, new_value);
        }
        return 0;
    }
    else if (new_value > cur_node->value)
    {
        if (cur_node->right_child == NULL)
        {
            node *new_node = malloc(sizeof(node));
            cur_node->right_child = new_node;
            new_node->value = new_value;
        }
        else
        {
            insert(cur_node->right_child, new_value);
        }
        return 0;
    }
    else
    {
        return 1;
    }
}


void print(node *cur_node)
{
    if (cur_node != NULL)
    {
        print(cur_node->left_child);
        printf("%i\n", cur_node->value);
        print(cur_node->right_child);
    }
}

void insert_random_numbers(int amount, int highest, node *tree)
{
    int error;
    int i = 0;
    while (i < amount+1)
    {
        int j = rand() % (highest+1);
        error = insert(tree, j);
        printf("%i\n", error);
        if (error == 1)
        {
            i--;
        }
        i++;
    }
}

void FREE(node *cur_node)
{
    if (cur_node == NULL)
    {
        return;
    }
    FREE(cur_node->left_child);
    FREE(cur_node->right_child);
    free(cur_node);
}



void remove_from_tree(node *cur_node, int value)
{
   if (cur_node->value == value)
   {
        cur_node->deleted = true;
   }
   else if (cur_node->value < value)
   {
        remove_from_tree(cur_node->right_child, value);
   }
   else
   {
        remove_from_tree(cur_node->left_child, value);
   }
}




node * balance_tree(node *tree)
{
    int array[MAX_SIZE];
    count = 0;
    tree_to_array(tree, array);
    int size = count;
    return array_to_tree(array, size);
}

void tree_to_array(node *cur_node, int array[])
{
    if (cur_node != NULL)
    {
        tree_to_array(cur_node->left_child, array);
        if (cur_node->deleted != true)
        {
            array[count] = cur_node->value;

            count ++;

        }
        tree_to_array(cur_node->right_child, array);
    }
    else if (cur_node == NULL)
    {
       return;
    }


}


// size is the number of elements in the array
node * array_to_tree(int array[], int size)
{

    node * mid_node =  NULL;
    if (size > 0)
    {
        // to process were splitting the array in 3 parts, first half, middle value, and second half, half size is the size of the first half
        int half_size = (size % 2 == 0 ? size/2 : (size-1)/2);
        // mid is pointer to the array destined right subtree
        int *mid = &(array[half_size+1]);
        node *left_tree = array_to_tree(array, half_size);
        mid_node = malloc(sizeof(node));
        mid_node->value = array[half_size];
        mid_node->left_child = left_tree;
        mid_node->deleted = false;
        node *right_tree = array_to_tree(mid, (size-half_size-1));
        mid_node->right_child = right_tree;
    }
    return mid_node;
}




int depth(node *cur_node)
{
    if (cur_node != NULL)
    {
        int a= 1+MAX(depth(cur_node->left_child), depth(cur_node->right_child));
        return a;
    }
    return 0;
}
