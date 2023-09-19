#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main(int argc, char **argv) 
{
    FILE *file = 0;
    int matrix_size, i, bit_offset, file_offset, file_size;
    unsigned char byte;

    file = fopen(argv[1], "rb+");

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    fprintf(stderr, "File size: %d bytes\n", file_size);

    matrix_size = (int)sqrt(file_size * 8);
    fprintf(stderr, "Matrix size: %dx%d bits\n\n", matrix_size, matrix_size);
    
    if (matrix_size == 4) {
        fread(&byte, 1, sizeof(char), file);
        fprintf(stderr, "Byte before: %d\n", byte);
        byte = byte | (1 << 7) | (1 << 2);
        fprintf(stderr, "Byte after: %d\n", byte);
        fseek(file, -sizeof(char), SEEK_CUR);
        fwrite(&byte, 1, sizeof(char), file);

        fread(&byte, 1, sizeof(char), file);
        fprintf(stderr, "Byte before: %d\n", byte);
        byte = byte | (1 << 5) | 1;
        fprintf(stderr, "Byte after: %d\n", byte);
        fseek(file, -sizeof(char), SEEK_CUR);
        fwrite(&byte, 1, sizeof(char), file);
    } else {
        for (i = 0; i < matrix_size; i++) {
            file_offset = (i * matrix_size) / 8 + i / 8;
            bit_offset = 7 - i % 8;
            fprintf(stderr, "File offset: %d bits\n", file_offset);
            fprintf(stderr, "Bit offset: %d bits\n", bit_offset);
            fseek(file, file_offset, SEEK_SET);
            fread(&byte, 1, sizeof(char), file);
            fprintf(stderr, "Byte before: %d\n", byte);
            byte |= (1 << bit_offset);
            fprintf(stderr, "Byte after: %d\n", byte);
            fseek(file, -sizeof(char), SEEK_CUR);
            fwrite(&byte, 1, sizeof(char), file);
            fprintf(stderr, "\n");
        }
    }

    fseek(file, 0, SEEK_SET);
    for (i = 0; i < file_size; i++) {
        fread(&byte, 1, sizeof(byte), file);
        printf("%d ", byte);
    }
    printf("\n");
    
    fclose(file);

    return 0;
}