#include <stdio.h>	

#define n 250

/* считает степень 10*/
double tenDeg(int signDeg, int degree) {
    int i = 0;
    double result = 1;
    /* если отрицательная степень */
    if (signDeg == 1)
        while (i < degree) {
            result /= 10;
            ++i;
        }
    else 
    /* если положительная степень */
        while (i < degree) {
            result *= 10;
            ++i;
        }
    return result;
}

double str2double(char str[]) {
    double result = 0, part = 0;
    int i = 0, j = 0, sign = 0, signDeg = 0;
    /* учитываем знак */
    if (str[0] == '-') {
        ++i;
        sign = 1;
    } else 
        if (str[0] == '+')
            ++i;
    /* сначала считываем число до точки или e/E по схеме Горнера*/
    while (str[i] != '.' &&  str[i] != 'e' && str[i] != 'E' && str[i] != 'f' && str[i] != 'F') {
        result *= 10;
        result += (str[i] - '0');
        ++i;
    }
    /* остановились на точке; далее [число], [e/E], [+/-], [число, !после е/Е обязательно], [f/F] */
    if (str[i] == '.') {
        /* далее просматриваем, есть ли число */
        while (str[i] != '\0' && str[i] != 'e' && str[i] != 'E' && str[i] != 'f' && str[i] != 'F')
            ++i;
        j = i; /* запомнили куда возвращаться */
        --i; /* смещаем индекс на последнюю цифру */
        /* начинаем считывать это число по схеме Горнера*/
        while (str[i] != '.') {
			part /= 10;
            part += (str[i] - '0');
			--i;
		}
        part /= 10;
        result += part;
        i = j;
    }
    /* теперь идем до e/Е, если оно встретится и накапливаем степень 10 */
	if (str[i] == 'e' || str[i] == 'E') {
        i += 2;
        j = 0;
        /* учитываем знак степени */
		if (str[i-1] == '-')
            signDeg = 1;
        else if (str[i-1] != '+')
            --i;
        /* подсчитываем саму степень, i указывает на первую цифру */
        while (str[i] != 0 && str[i] != 'f' && str[i] != 'F') {
            j *= 10;
            j += (str[i] - '0');
            ++i;
        }
        result *= tenDeg(signDeg, j);
	}
    if (sign == 1)
        return -result;
    else
	    return result;
}

int main(void) {
	char str[n];
	while (scanf("%s", str) != EOF) {
		printf("%.10g\n", str2double(str));
	}	
}