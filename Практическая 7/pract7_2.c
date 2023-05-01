#include <pthread.h>
#include <locale.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <mqueue.h>
#include <sys/mman.h>

#define MQ_MAXMSG 10
#define MQ_MSGSIZE 255

const void close_data();

typedef struct
{
    char flag;
    mqd_t mq;
}th_args;


void *process(void *args) 
{
    char *flag = &((th_args *) args)->flag;
    mqd_t mq = ((th_args *) args)->mq;
    char buffer[MQ_MSGSIZE];
    gethostname(buffer, MQ_MSGSIZE);


    while ((int) (*flag) != 10) 
    {
        ssize_t res = mq_receive(mq, buffer, MQ_MSGSIZE, 0);

        sleep(1);

        if (res < 0) 
        {
            perror("Ошибка mq_receive");
        } else 
        {
            printf("Берётся из очереди: %s\n", buffer);
        }
    }
    pthread_exit((void*)1);
}


const void close_data(void *args) 
{
    mq_close(((th_args *) args)->mq);
    mq_unlink("/queue");
}


int main() 
{
    errno = 0;
    struct mq_attr attr = {0, MQ_MAXMSG, MQ_MSGSIZE, 0, {0}};
    th_args params;
    pthread_t id;
    int *exit;

    params.mq = mq_open("/queue", O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attr);

    int func_val = pthread_create(&id, NULL, process, (void *) (&params));
    if (func_val==0)
    {
        printf("Функция pthread_create для потока №1 завершилась успешно\n");
    }
    else
    {
        printf("Ошибка функции: %s\n, код ошибки %d\n", strerror(func_val), func_val);
        close_data((void *) &params);
        return 1;
    }

    params.flag = getchar();

    void *status;
    func_val = pthread_join(id, (void**)&exit);
    if (func_val==0)
    {
        printf("Функция pthread_join для потока №1 завершилась успешно\n");
    }
    else
    {
        perror("pthread_join");
        close_data((void *) &params);
        return 2;
    }
    printf("Программа завершила работу\n");

    close_data((void *) &params);
    return 0;
}