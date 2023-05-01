#include <stdio.h>
#include <fcntl.h>  
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <sys/select.h>



int keypresed()
{
    return 0;
}


int main()
{
    setlocale(LC_ALL, "Rus");
    printf("Программа 1 начала работу\n");
    struct timeval tv;
    fd_set rfds;
    sem_t *sem = sem_open("/semaphore", O_CREAT, 0644, 1);
    FILE *f;
    char *message = "1";
    int ret_val;
    if((f=fopen("file", "a"))==NULL) 
    {
        printf ("Не удалось открыть файл\n");
        exit(1);
    }
    printf("Программа ждёт нажатия клавиши\n");
    while(1)
    {
        sem_wait(sem);
        for(int i = 0; i<10; i++)
        {
            fputs(message, f);
            fflush(f);
            printf("%d", 1);
            fflush(stdout);
            sleep(1);
        }
        sem_post(sem);
        sleep(1);
        tv.tv_sec=0;
        tv.tv_usec=1;
        FD_ZERO(&rfds);
        FD_SET(0,&rfds);
        if(select(1,&rfds,NULL,NULL,&tv))
        {
            printf("\nКлавиша нажата\n");
            break;
        }
    }
    sem_close(sem);
    sem_unlink("/semaphore");
    printf("Программа завершила работу\n");
    return 0;
}