#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>


int main(int size, char* list[])
{
    setlocale(LC_ALL, "Rus");
    printf("Дочерний процесс начал работу\n");
    printf("Идентификатор текущего процесса: %d\n", getpid());
    printf("Идентификатор родительского процесса: %d\n", getppid());
    for(int i=0; i<size; i++)
    {
        printf("%s ", list[i]);
        sleep(1);
    }
    printf("Дочерний процесс завершил работу\n");
    exit(14);
}
