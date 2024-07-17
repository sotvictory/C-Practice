#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const double MONEY_REQUIRED = 42.47;

typedef struct UserData {
    char comment[58];
    double money;
    long id;
    char username[13];
} UserData;

int main() {
    UserData userdata = {
        .username = "student",
        .id = 5,
        .money = 0,
        .comment = "no comments...",
    };

    printf("Ok, so what do you think about this program? Leave your comment right here:\n");
    scanf("%s", &userdata.comment);

    if (strcmp(userdata.username, "admin")) {
        printf("HEY YOU'RE NOT ADMIN GET AWAY RIGHT NOW!\n");
        exit(1);
    }

    printf("Oh hi admin, what's up? Lost your super-duper universal key again?\n");
    printf("Fine, I can tell you the key provided you have enough *money* (you know what I mean...)\n");

    if (userdata.money > MONEY_REQUIRED) {
        printf("Hell NO, you're too rich for admin! Get out!\n");
        exit(1);
    } else if (userdata.money < MONEY_REQUIRED) {
        printf("Oh dear, you're so poor... Don't come any closer, sorry.\n");
        exit(1);
    } else {
        printf("WOW so hacker!\n");
    }

    return 0;
}