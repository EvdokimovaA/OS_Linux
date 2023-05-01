#include <stdio.h>
#include <locale.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>

int main() 
{
    setlocale(LC_ALL, "Rus");
    char *filename = (char *) "file";
    int rv = utime(filename, NULL);
    if (rv == -1)
        perror("Ошибка utime");
    else
        printf("utime() выполнена успешно\n");
    return 0;
}