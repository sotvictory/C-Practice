#include <stdio.h>
#include <limits.h>

int charrepresentation1(void){
    if (CHAR_MIN == 0)
        printf("Char is unsigned by default\n");
    else
        printf("Char is signed by default\n");
    return 0;
}

int charrepresentation2(void){
    char c = 126;
    c += 2;
    if (c > 126)
        printf("Char is unsigned by default\n");
    else
        printf("Char is signed by default\n");
    return 0;
}

int main(void) {
    charrepresentation1();
    charrepresentation2();
    return 0;
}