#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>

sem_t sem_id;

typedef struct
{
    int flag;
    int symbol;
}targs;


void* process1(void *arg)
{
    printf("Поток №1 начал работу\n");
    struct timespec t;
    clock_gettime(0, &t);
    targs *args = (targs*) arg;
    int ret_val;
    while (args->flag==0)
    {
        t.tv_sec++;
        ret_val = sem_timedwait(&sem_id, &t);
        if (ret_val==-1) continue;
        for(int i = 0; i<10;i++)
        {
            printf("%d", args->symbol);
            fflush(stdout);
            sleep(1);
        }
        ret_val = sem_post(&sem_id);
        if(ret_val==0)
        {
            printf("\nФункция sem_post для потока №1 завершилась успешно\n");
        }
        else
        {
            perror("sem_post");
        }
        sleep(1);
    }
    printf("Поток №1 закончил работу\n");
    pthread_exit((void*)1);
}

void* process2(void *arg)
{ 
    printf("Поток №2 начал работу\n");
    struct timespec t;
    clock_gettime(0, &t);
    targs *args = (targs*) arg;
    int ret_val;
    while (args->flag==0)
    {
        t.tv_sec++;
        ret_val = sem_timedwait(&sem_id, &t);
        if (ret_val==-1) continue;
        for(int i = 0; i<10;i++)
        {
            printf("%d", args->symbol);
            fflush(stdout);
            sleep(1);
        }
        ret_val = sem_post(&sem_id);
        if(ret_val==0)
        {
            printf("\nФункция sem_post для потока №2 завершилась успешно\n");
        }
        else
        {
            perror("sem_post");
        }
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

    sem_init(&sem_id, 0, 1);

    int func_val = pthread_create(&id1, NULL, process1, &arg1);
    if (func_val==0)
    {
        printf("Функция pthread_create для потока №1 завершилась успешно\n");
    }
    else
    {
        printf("Ошибка функции: %s\n, код ошибки %d\n", strerror(func_val), func_val);
    }

    func_val = pthread_create(&id2, NULL, process2, &arg2);
    if (func_val==0)
    {
        printf("Функция pthread_create для потока №2 завершилась успешно\n");
    }
    else
    {
        printf("Ошибка функции: %s\n, код ошибки %d\n", strerror(func_val), func_val);
    }
    printf("Программа ждёт нажатия клавиши\n");
    getchar();
    printf("Клавиша нажата\n");

    arg1.flag = 1;
    arg2.flag = 1;

    func_val = pthread_join(id1, (void**)&exit);
    printf("\nВозвращённое значение по завершении потока №1: %p\n", exit);
    if (func_val==0)
    {
        printf("Функция pthread_join для потока №1 завершилась успешно\n");
    }
    else
    {
        perror("pthread_join");
    }
    func_val = pthread_join(id2, (void**)&exit);
    
    printf("\nВозвращённое значение по завершении потока №2: %p\n", exit);
    if (func_val==0)
    {
        printf("Функция pthread_join для потока №2 завершилась успешно\n");
    }
    else
    {
        perror("pthread_join");
    }

    sem_destroy(&sem_id);
    printf("Программа завершила работу\n");
    return 0;
}