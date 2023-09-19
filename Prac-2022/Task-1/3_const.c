#include <stdio.h>

int main(void) {
    const static int i_1 = 10, i_2 = 10u, i_3 = 10l, i_4 = 10ul, i_5 = 013, i_6 = 0xA;
    const double d_1 = 2.3, d_2 = 12e2, d_3 = .1f, d_4 = 20.34l; 
    const char c_1 = 'b', c_2 = '\x30', c_3 = '\n'; /* 'b', '0', '\n' */
    const char s_1[10] = "string", s_2[10] = "c++";
    /* const int i_7 = 293., i_8 = 3.e2; */

    s_1 = "sjfdkj";
    printf("%d\n", i_1);
    printf("%f\n", d_4);
    printf("%c\n", c_3);
    printf("%s\n", s_2);
}