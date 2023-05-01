#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int flag; 
int descript;                                                                                                                                                                                                                                                                                                                                                               


void* func_read(void* args)
{
    errno = 0;
    char buffer[255];

    while(flag == 0)
    {
        //buffer = '\0';
        strcpy(buffer,"\0");
        if (read(descript, &buffer, 255) > 0)
        {
            printf("Считывание: %s\n", buffer);
        }
        else
        {
            printf("Файл пуст\n");
        }

        sleep(1);
    }   
    pthread_exit((void*)1);  
}


int main()
{
    setlocale(LC_ALL, "Rus");
    printf("Программа начала работу\n");
    pthread_t id;
    int *exit;
    errno = 0;


    mkfifo("/tmp/pipe",0644);
    descript = open("/tmp/pipe", O_CREAT|O_RDONLY|O_NONBLOCK);

    int func_val = pthread_create(&id, NULL, func_read, NULL);
    if (func_val==0)
    {
        printf("Функция pthread_create для потока №1 завершилась успешно\n");
    }
    else
    {
        printf("Ошибка функции: %s\n, код ошибки %d\n", strerror(func_val), func_val);
        close(descript);
        unlink("/tmp/pipe");
    }


    printf("Программа ждёт нажатия клавиши\n");
    getchar();
    printf("Клавиша нажата\n");
    flag = 1;

    func_val = pthread_join(id, (void**)&exit);
    if (func_val==0)
    {
        printf("Функция pthread_join для потока №1 завершилась успешно\n");
    }
    else
    {
        perror("pthread_join");
        close(descript);
        unlink("/tmp/pipe");
    }
    printf("Программа завершила работу\n");
    close(descript);
    unlink("/tmp/pipe");
    return 0;
}