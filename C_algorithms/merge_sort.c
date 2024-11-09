#include <stdio.h>
#include <stdlib.h>

void merge_sort(int list[], int left_index, int right_index);
void merge(int list[], int left_index, int mid, int right_index);
void print_array(int list[], int size);

int main(void)
{
    const int LENGTH = 1047499;
    int list[LENGTH];
    for (int i=0; i<LENGTH; i++)
    {
        list[i] = rand();
    }
    int arr_size = sizeof(list) / sizeof(list[0]);
    //printf("Old list: ");
    //print_array(list, arr_size);
    merge_sort(list, 0, arr_size-1);
    //printf("Sorted list: ");
    //print_array(list, arr_size);
}

void print_array(int list[], int size)
{
    for (int i=0; i<size; i++)
    {
        printf("%i ", list[i]);
    }
    printf("\n");
}

void merge_sort(int list[], int left_index, int right_index)
{
    if (left_index < right_index)
    {
        int mid = (right_index + left_index)/2;
        merge_sort(list, left_index, mid);
        merge_sort(list, mid+1, right_index);
        merge(list, left_index, mid, right_index);
    }

}

void merge(int list[], int left_index, int mid, int right_index)
{
    //steps: create 2 temporary arrays, Left and Right, compare first numbers in each, and put the sorted version back into list
    int left_len = mid - left_index + 1;
    int right_len = right_index - mid;
    int left_arr[left_len], right_arr[right_len];
    for (int i=0; i<left_len; i++)
    {
        left_arr[i] = list[left_index + i];
    }
    for (int j=0; j<right_len; j++)
    {
        right_arr[j] = list[mid + j + 1];
    }

    int m = left_index;
    int k = 0;
    int l = 0;
    while (k<left_len && l<right_len)
    {
        if (left_arr[k] < right_arr[l])
        {
            list[m] = left_arr[k];
            k++;
        }
        else
        {
            list[m] = right_arr[l];
            l++;
        }
        m++;
    }

    while (k<left_len)
    {
        list[m] = left_arr[k];
        m++;
        k++;
    }
    while (l<right_len)
    {
        list[m] = right_arr[l];
        m++;
        l++;
    }

}