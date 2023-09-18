#include <stdio.h>

int main(void) 
{
    unsigned char matrix[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    FILE *file = fopen("matrix.bin", "wb");
    fwrite(matrix, sizeof(char), 8, file);
    fclose(file);

    return 0;
}