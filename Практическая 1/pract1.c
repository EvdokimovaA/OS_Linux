#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>


typedef struct
{
    int flag;
    int symbol;
}targs;

void* process1(void *arg)
{
    printf("Поток №1 начал работу\n");
    targs *args = (targs*) arg;
    while (args->flag==0)
    {
        printf("%d\n", args->symbol);
        fflush(stdout);
        sleep(1);
    }
    printf("Поток №1 закончил работу\n");
    pthread_exit((void*)1);
}

void* process2(void *arg)
{ 
    printf("Поток №2 начал работу\n");
    targs *args = (targs*) arg;
    while (args->flag==0)
    {
        printf("%d\n", args->symbol);
        fflush(stdout);
        sleep(1);
    }
    printf("Поток №2 закончил работу\n");
    pthread_exit((void*)2);
}


int main()
{
    setlocale(LC_ALL, "Rus");
    printf("Программа начала работу\n");
    pthread_t id1, id2;
    targs arg1, arg2;
    int *exit;
    arg1.flag = 0;
    arg2.flag = 0;
    arg1.symbol = 1;
    arg2.symbol = 2;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, -100);

    int func_val = pthread_create(&id1, &attr, process1, &arg1);
    if (func_val==0)
    {
        printf("Функция pthread_create для потока №1 завершилась успешно\n");
    }
    else
    {
        perror("pthread_create");
    }

    func_val = pthread_create(&id2, &attr, process2, &arg2);
    if (func_val==0)
    {
        printf("Функция pthread_create для потока №2 завершилась успешно\n");
    }
    else
    {
        perror("pthread_create");
    }
    printf("Программа ждёт нажатия клавиши\n");
    getchar();
    printf("Клавиша нажата\n");

    arg1.flag = 1;
    arg2.flag = 1;

    func_val = pthread_join(id1, (void**)&exit);
    printf("\nВозвращённое значение по завершении потока №1: %d\n", *exit);
    if (func_val==0)
    {
        printf("Функция pthread_join для потока №1 завершилась успешно\n");
    }
    else
    {
        perror("pthread_join");
    }
    func_val = pthread_join(id2, (void**)&exit);
    printf("\nВозвращённое значение по завершении потока №2: %d\n", *exit);
    if (func_val==0)
    {
        printf("Функция pthread_join для потока №2 завершилась успешно\n");
    }
    else
    {
        perror("pthread_join");
    }

    printf("Программа завершила работу\n");
    return 0;
}