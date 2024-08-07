### Задание 2.1
Вычисляется квадратный корень из числа с заданной точностью Е.
Реализовано с помощью метода Ньютона.
Подаются вещественное Е, далее вещественные числа, для которых нужно вычислить корень.

### Задание 2.2
Считается значение многочлена с коэфициентами a_i в точке x.
Реализовано по схеме Горнера.
Подаются вещественные числа: x, a_n, a_n-1, ..., a_0.

### Задание 2.2.2
К 2.2 добавлено вычисление производной.
Реализовано по схеме Горнера.
Подаются вещественные числа: x, a_n, a_n-1, ..., a_0.

### Задание 2.2.3
Вычисление определенного интеграла от многочлена степени n с коэфициентами a_i в пределах от о до х.
Реализовано по схеме Горнера.
Подаются целая степень многочлена n и вещественные числа: x, a_n, a_n-1, ..., a_0.

### Задание 2.3
Вычисление i-го члена последовательности Фибоначчи.
Реализованы итеративная и рекурсивная версии.
Используется библиотека time.h для того, чтобы отследить разницу во времени вычисления.
Подается целое число i.

### Задание 2.4
Реализована функция str2double, которая преобразует строку в вещественное число.
Реализовано в соответствии с синтаксической диаграммой вещественной константы.
Исключен суффикс L (long double), добавлен знак перед всем числом.
Вводится вещественное число, но считывается как строка.

### Задание 2.5
Реализован список слов (слово - список букв), базовая библиотека для работы со списками в данной задаче.
Вводится строка, состоящая из непустых слов, разделенных пробелами и табуляциями. 
По ней формируется список, в котором впоследствии удаляются все слова, совпадающие с последним (кроме него самого).
Модифицированный список печатается.

### Задание 2.6
Вводятся последовательность '+', '-', '?' и чисел.
+<неотрицательное целое> -> элемент добавляется в дерево поиска, если его нет.
-<неотрицательное целое> -> элемент удаляется из дп, если он есть.
?<неотрицательное целое> -> печатается элемент с новой строки.
и через пробел печатается "yes", если данный элемент присутствует, "no" - отсутствует.

### Задание 2.6
Исходный калькулятор, вычисляющий выражения, состоящие из цифр '0'-'9', операций сложения, умножения и скобок,
дополнен операциями вычитания, деления с отбрасыванием остатка, возведения в степень.
