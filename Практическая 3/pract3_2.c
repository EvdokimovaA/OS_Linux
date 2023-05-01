#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
    setlocale(LC_ALL, "Rus");
    printf("Родительский процесс начал работу\n");
    pid_t pid = fork();
    int status;
    char * const env [] = {"Что-нибудь = чего-нибудь", NULL};
    char * arg1 = {"привет"};
    char * arg2 = {"мир"};
    if (pid == -1)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        execle("./pract3_1", arg1, arg2, NULL, env);
    }   
    else
    {
        while (waitpid(pid, &status, WNOHANG) == 0)
        {
            printf("Ждём\n");
            usleep(500000);
        }
        printf("Идентификатор текущего процесса: %d\n", getpid());
        printf("Идентификатор родительского процесса: %d\n", getppid());
        printf("Идентификатор дочернего процесса: %d\n", pid);
        printf("Статус: %d\n", WEXITSTATUS(status));
    }
    printf("Родительский процесс завершил работу\n");
    exit(0);
}
