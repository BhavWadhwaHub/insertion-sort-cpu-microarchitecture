#include "Project.h"
#include"vectorclass.h"

void insertion_sort_simd_c(uint64_t *arr,uint64_t size)
{
    if(size<=1)
    {
        return;
    }
    for (uint64_t i=1;i<size;i++)
    {
        uint64_t key=arr[i];
        int64_t j=i-1;
        while (j>=0&&arr[j]>key)
        {
            j--;
        }
        int64_t shift_start=j+1;
        int64_t shift_count=i-shift_start;
        if (shift_count>=4)
        {
            int64_t k=i-1;
            while (k>=shift_start+3)
            {
                Vec4q block=Vec4q().load(arr+k-3); 
                block.store(arr+k-2);                  
                k-=4;
            }
            while(k>=shift_start)
            {
                arr[k+1]=arr[k];
                k--;
            }
        }
        else
        {
            for(int64_t k=i-1;k>=shift_start;k--)
            {
                arr[k+1]=arr[k];
            }
        }
        
        arr[shift_start]=key;
    }
}



