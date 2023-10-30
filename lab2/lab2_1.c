#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid;
    pid = fork();
    printf("%d\n",pid);
    return 0;
}

// fork() создает дочерний процесс, возвращает его ID, дочерний процесс берет начало со строки, где вызван fork();
// число, отличное от нуля - родительский процесс
// 0 - данные, выведенные дочерним процессом
