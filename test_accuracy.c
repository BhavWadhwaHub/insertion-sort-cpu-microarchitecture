#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include"Project.h"

int is_sorted(uint64_t*arr,uint64_t n)
{
for(uint64_t i=0;i<n-1;i++)
{
    if(arr[i]>arr[i+1])
    {
        return 0;
    }
}
return 1;
}

void test_sort(const char*name,uint64_t*arr,uint64_t n)
{
    uint64_t* copy = (uint64_t*)malloc(n * sizeof(uint64_t));
    for (uint64_t i = 0; i < n; i++)
        copy[i] = arr[i];
    if(strcmp(name,"personal")==0)
    {
        insertion_sort_personal(copy,n);
    }
    else if(strcmp(name,"optimized")==0)
    {
        insertion_sort_optimized(copy,n);
    }
    else if(strcmp(name,"asm")==0)
    {
        insertion_sort_asm(copy,n);
    }
    else if(strcmp(name,"simd_c")==0)
    {
        insertion_sort_simd_c(copy,n);
    }
    else if(strcmp(name,"simd_asm")==0)
    {
        insertion_sort_simd_asm(copy,n);
    }
    if (is_sorted(copy, n))
        printf("%s: PASS\n", name);
    else
        printf("%s: FAIL\n", name);
    free(copy);
}

int main(){
uint64_t arr[]={5,10,42,453,22,3,52,32,3};
uint64_t size=9;
printf("Testing with array size %lu\n",size);
test_sort("personal",arr,size);
test_sort("optimized",arr,size);
test_sort("asm",arr,size);
test_sort("simd_c",arr,size);
test_sort("simd_asm",arr,size);
return 0;
}

