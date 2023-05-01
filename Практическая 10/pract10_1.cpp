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
#include <ev.h>


#define MQ_MAXMSG 10
#define MQ_MSGSIZE 255

const void close_data();

char flag;
mqd_t mq;
ev_io enter_watcher;
ev_timer write_watcher;

static void enter_cb(EV_P_ ev_io *w, int revents) 
{
    printf("Клавиша нажата\n");
    ev_io_stop(EV_A_ w);
    ev_break(EV_A_ EVBREAK_ALL);
}

static void write_cb(EV_P_ ev_timer *w, int revents) 
{
    char buffer[MQ_MSGSIZE];
    gethostname(buffer, MQ_MSGSIZE);
    int res = mq_send(mq, buffer, MQ_MSGSIZE, 0);
    sleep(1);
    if (res == -1) 
    {
        perror("Ошибка mq_send");
    } else 
    {
        printf("Отправка в очередь: %s\n", buffer);
    }
    
}

const void close_data() 
{
    mq_close(mq);
    mq_unlink("/queue");
}


int main() 
{
    struct ev_loop *loop = EV_DEFAULT;
    errno = 0;
    struct mq_attr attr = {0, MQ_MAXMSG, MQ_MSGSIZE, 0, {0}};
    mq = mq_open("/queue", O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &attr);

    ev_io_init(&enter_watcher, enter_cb, STDIN_FILENO, EV_READ);
    ev_timer_init(&write_watcher, write_cb, 1., 1.);

    ev_io_start(loop, &enter_watcher);
    ev_timer_start(loop, &write_watcher);

    ev_run(EV_DEFAULT_ 0);

    printf("Программа завершила работу\n");
    close_data();
    return 0;
}