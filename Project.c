#include "Project.h"


void insertion_sort_personal(uint64_t *arr, uint64_t size)
{
    for (uint64_t i = 1; i < size; i++) {
        uint64_t j = i;
        while (j > 0 && arr[j] < arr[j - 1]) { 
            uint64_t temp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = temp;
            j--;
        }
    }
}

void insertion_sort_optimized(uint64_t *arr,uint64_t size)
{
    if(size<=1)
    {
        return;
    }
    for(uint64_t i=1;i<size;i++)
    {
        uint64_t value=arr[i];
        uint64_t j=i;
        while(j>0)  
        {
            uint64_t previous=arr[j-1];
            if(previous <= value)
            {
              break;
            } 
            arr[j]=previous;
            j--;
        }
        arr[j] = value;
    }
}

