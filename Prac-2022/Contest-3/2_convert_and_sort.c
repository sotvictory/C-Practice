#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) 
{
    return *(int*)a - *(int*)b;
}

int main(int argc, char **argv) 
{
    int i, n;
    int *nums;

    n = argc - 1;
    nums = malloc(n * sizeof(int));

    for (i = 1; i < argc; i++) 
        nums[i-1] = strtol(argv[i], NULL, 17);

    qsort(nums, n, sizeof(int), compare);

    for (i = 0; i < n; i++)
        printf("%d ", nums[i]);
    printf("\n");

    free(nums);

    return 0;
}