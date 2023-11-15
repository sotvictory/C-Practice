# Команды

Все скомпилировать: `make` <br>
Все почистить: `make clean` <br>

Проверить на утечку файловых дескрипторов можно, например, так:
``` shell
cat > test.txt
sleep 10000
chmod +x test.txt
./prog ./test.txt
ps ax
ls -l /proc/<PID>/fd
```

Проверить дедлоки (посмотреть current call stack):
```
gdb <program> <PID>
bt
```

# Руденко:

`ioredirect.c` (8.18 d):
``` shell
pr1 < f.dat > f.res
```

`subshell.c` (8.18 h):
``` shell
((pr1 | pr2); pr3) | pr4
```

Проверить, например, на:
``` shell
# ((ls | sort); pwd) | sort
./subshell ls sort pwd sort
```

# Казачук (синяя книга): 

 `cmd.c` (11.1 a):
 ``` shell
pr1 arg1 arg2 | pr2; pr3 >> f1.dat
 ```

  Проверить, например, на:
 ``` shell
 # ls -l -h | sort; pwd >> f1.dat
 ./cmd ls -l -h sort pwd f1.dat
 ```

 `conveyor.c` (11.1 c):
 ``` shell
 pr1 | pr2 | ... | prn
 ```

 Проверить, например, на:
 ``` shell
 # ls | cat | wc
 ./conveyor ls cat wc
 ```

`str_search.c` (11.4): <br>
Написать программу параллельного поиска подстроки в файлах, имена которых заданы в командной строке. Поиск подстроки в каждом из заданных файлов организовывать в процессе-потомке, создав для каждого файла свой процесс. Программа завершается, как только подстрока будет найдена в одном из файлов. Все незавершенные процессы должны быть прерваны.