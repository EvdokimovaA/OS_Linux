#include <stdio.h>
#include <locale.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/capability.h>
#define filename "pract9_1"

int main()
{
    setlocale(LC_ALL, "Rus");
    cap_t cp1, cp2;
    cp1 = cap_get_file((const char*)filename);
    char *st = cap_to_text(cp1, NULL);
    printf("Возможности:\n%s\n", st);

    cp2 = cap_from_text("cap_fowner=+eip");
    int rv = cap_set_file(filename, cp2);
    if(rv == -1)
        perror("Ошибка cap_set_file");
    else printf("\ncap_set_file 'file' выполнена успешно\n");

    cp1 = cap_get_file((const char*)filename);
    st = cap_to_text(cp1, NULL);
    printf("\nВозможности:\n%s\n", st);

    return 0;
}

