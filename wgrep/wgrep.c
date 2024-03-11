#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    if (argc == 2) {
        //se apenas dois argumentos foram fornecidos, verifica se o segundo argumento é um arquivo válido
        fp = fopen(argv[2], "r");
        if (fp == NULL) {
            fp = stdin;
            while (getline(&line, &len, fp) != -1) {
                printf("%s", line);
            }
        } 
    } 

    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }
    
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("wgrep: cannot open file\n");
        exit(1);
    }

    // le a linha do arquivo apontado por fp e armazena essa linha em line, também atualiza len com o tamanho da linha lida, -1 é retornado quando atinge o final do arquivo ou se ocorre erro na leitura
    while (getline(&line, &len, fp) != -1) {
        for(int i=0; i < argc; i++) {
            if (strstr(line, argv[i]) != NULL) {
                printf("%s", line);
                break;
            }
        }
    }
    printf("\n");
    free(line); 
    fclose(fp);

    return 0;
}
