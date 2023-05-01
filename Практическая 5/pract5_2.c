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
#include <signal.h>


void* func_write(void* args);
void sig_handler(int descript);

int flag;                                                                                                                                                                                                                                                                                                                                                              
int descript; 


void sig_handler(int descript)
{
    printf("Получен SIGPIPE\n");
}


void* func_open(void* args)
{
    errno = 0;
    
    while(flag == 0)
    {
        descript = open("/tmp/pipe", O_CREAT|O_WRONLY|O_NONBLOCK);
        pthread_t id_write;
        
        if (descript == -1) 
        {
            perror("Ошибка открытия");
            sleep(1);
        }
        else
        {
            if(pthread_create(&id_write, NULL, func_write, args))
            {
                printf("Ошибка создания потока передачи");
                close(descript);
                unlink("/tmp/pipe");
            }

            pthread_join(id_write, NULL);            
        }
    }
}


void* func_write(void* args)
{
    char buffer[255];
    while(flag == 0)
    {
        gethostname(buffer, 255);
        write(descript, &buffer, 255);
        printf("Передаётся: %s\n", buffer);
        sleep(1);
    }     
}




int main()
{
    setlocale(LC_ALL, "Rus");
    printf("Программа начала работу\n");
    signal(SIGPIPE, sig_handler);
    errno = 0;
    int *exit;
    pthread_t id;

    mkfifo("/tmp/pipe", 0644);
    
    int func_val = pthread_create(&id, NULL, func_open, NULL);
    if (func_val==0)
    {
        printf("Функция pthread_create для потока №1 завершилась успешно\n");
    }
    else
    {
        printf("Ошибка функции: %s\n, код ошибки %d\n", strerror(func_val), func_val);
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