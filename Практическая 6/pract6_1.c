#include <stdio.h>
#include <fcntl.h>  
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <sys/select.h>
# include <sys/types.h>
# include <sys/ipc.h>
#include <sys/shm.h> 

int keypresed()
{
    struct timeval tv;
    fd_set rfds;
    tv.tv_sec=0;
    tv.tv_usec=1;
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    if(select(1,&rfds,NULL,NULL,&tv))
    {
        printf("\nКлавиша нажата\n");
        return 1;
    }
    return 0;
}


int main()
{
    setlocale(LC_ALL, "Rus");
    printf("Программа 1 начала работу\n");

    key_t key = ftok("file.txt", 1);
	int memory = shmget(key, 4096, IPC_CREAT|0666); 
	char *p = (char*)shmat(memory, NULL,0);

    sem_t *sem_read = sem_open("/sem_read", O_CREAT, 0644, 0);
    sem_t *sem_write = sem_open("/sem_write", O_CREAT, 0644, 0);
    FILE *f = fopen("file.txt", "a");

    char buffer[255];

    printf("Программа ждёт нажатия клавиши\n");
    while(1)
    {
        gethostname(buffer, 255);
        printf("Передаётся: %s\n", buffer);
        memcpy(p, &buffer, 255);
        sem_post(sem_read);
        sem_wait(sem_write);
        sleep(1);
        if(keypresed()==1)
        {
            break;
        }
    }

    fclose(f);
	sem_close(sem_read);
	sem_unlink("/sem_read");
	sem_close(sem_write);
	sem_unlink("/sem_write");
	shmdt(p);
	shmctl(memory, IPC_RMID, (shmid_ds*) p);
    printf("Программа завершила работу\n");
    return 0;
}