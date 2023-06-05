# Задание 1:

``` C
unsigned u;
unsigned char uc;
int i;
float f;
unsigned short us;
long double ld;
double d;
short s;
```

| Выражение                          	| Тип выражения 	|
|------------------------------------	|---------------	|
| `(uc + d) * ld ? us : f`           	| `float`       	|
| `(s + '1') / u - us * i`           	| `unsigned`    	|
| `uc - '0' + (signed char) '2'`     	| `int`         	|
| `(102.5L / s * 3.14f) * (3.f + .3)` 	| `long double` 	|
| `(unsigned) i - (11 - 2) * 5`      	| `unsigned`    	|

# Задание 2:

| Выражение                         	| Значение выражения 	|
|-----------------------------------	|--------------------	|
| `b / a / c`                       	| `0`                	|
| `d / a % c`                       	| `2`                	|
| `c % d - e`                       	| `9`                	|
| `-e % a + b / a * -5 + 5`         	| `-9`               	|
| `sizeof((int)(unsigned int))(-1)` 	| `error`            	|

# Задание 3:

``` C
# include <stdio.h>

int main ()
{
    char str[] = "TTTARAMEPYAM1\1\11W\1AYYM1";
    int i, c;

    for (i = 2; (c = str[i]) != '\0'; i++)
    {
        switch (c)
        {
            case 'a':
                putchar('i');
                continue;
            case '1':
                break;
            case 1:
                while ((c = str[++i]) != '\1' && c != '\0');
            case 9:
                putchar('P');
            case 'E':
            case 'Y':
                continue;
            default:
                putchar(c);
                continue;
        }
        putchar(' ');
    }
    putchar('\n');
}
```

Программа напечатает `TARAMPAM PAM \n`

# Задание 4:
Описать функцию, которая меняет местами главную и побочную диагонали матрицы размера NxN. Диагонали рассматриваются сверху вниз.

Решение:
``` C
# define N 3

void change(int m[][N])
{
    int i, tmp;

    for (i = 0, i < N; i++)
    {
        tmp = a[i][i];
        a[i][i] = a[i][N-i-1];
        a[i][N-i-1] = tmp;
    }
}
```

```
Sample:
1 2 3    3 2 1
4 5 6 -> 4 5 6
7 8 9    9 8 7
```

# Задание 5: 
Напишите программу, которая подсчитывает, сколько всего в файлах строк, оканчивающихся заданным символом. Программа интерпретирует `argv[1]` как слово из одного символа, остальные — как имена файлов. 

Использовать высокоуровневый ввод-вывод.

# Задание 6:
Напишите функцию, которая преобразует список символов: удаляет каждое нечетное вхождение символа a.

```
Input: aaabbacda
Output: aabbcda
```