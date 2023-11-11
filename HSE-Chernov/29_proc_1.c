#include <unistd.h>

/* либо -D_GNU_SOURCE */
extern char *environ[];

/* cat > a.txt
 * chmod +x a.txt 
 * теперь a.txt ядром будет интерпретироваться как shell script
 * или в начале можно указать интерпретатор:
 * cat > b.txt
 * #! /bin/python3
 * print('python')
 * chmod +x b.txt
 * ./b.txt 
 * cat /proc/sys/?/ -- посмотреть поддерживаемые исполняемые форматы */

/* ./29_proc_1 /bin/echo 
 * ./29_proc_1 ./b.txt */
int main(int argc, char **argv)
{
    //char *args[] = { argv[1], "1", "2", NULL };
    //execve(argv[1], args, environ);
    
    execlp(argv[1], argv[1], "1", "2", NULL);
}