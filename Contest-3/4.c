#include <stdio.h>
#include <stdint.h>

void replace(uint32_t *num, int pos, int len) {
    uint32_t mask = ((1 << len) - 1) << pos;
    *num |= mask;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", argv[1]);
        return 1;
    }

    uint32_t num;
    fread(&num, sizeof(num), 1, fp);
    fclose(fp);

    int max_len = 0, max_pos = -1, cur_len = 0, cur_pos = -1;
    for (int i = 0; i < 32; i++) {
        if ((num & (1 << i)) != 0) {
            if (cur_pos == -1) {
                cur_pos = i;
            }
            cur_len++;
        } else {
            if (cur_len > max_len) {
                max_len = cur_len;
                max_pos = cur_pos;
            }
            cur_len = 0;
            cur_pos = -1;
        }
    }

    if (cur_len > max_len) {
        max_len = cur_len;
        max_pos = cur_pos;
    }

    printf("%d %d\n", max_pos, max_pos + max_len - 1);
    
    replace(&num, max_pos, max_len);

    printf("%08x\n", num);

    return 0;
}