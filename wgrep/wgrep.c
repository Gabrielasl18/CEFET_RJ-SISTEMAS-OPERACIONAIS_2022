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

/*

Seu programa wgrep sempre recebe um termo de pesquisa e zero ou mais arquivos para pesquisar 
(portanto, mais de um é possível).Ele deve passar por cada linha e verificar se o termo de pesquisa 
está nela; se estiver, a linha deve ser impressa, e se não estiver,
a linha deve ser ignorada. 
 
_____________________________
 
A correspondência é sensível a maiúsculas e minúsculas. Assim, se estiver procurando por "foo", linhas 
com "Foo" não corresponderão.
 	
_____________________________
     
As linhas podem ser arbitrariamente longas (ou seja, você pode ver muitos caracteres antes de encontrar 
um caractere de nova linha, \n).wgrep deve funcionar como esperado mesmo com linhas muito longas. Para 
isso, você pode querer olhar para a chamada de biblioteca getline()(em vez de fgets()), ou criar sua própria.

_____________________________
 
Se wgrep não receber argumentos na linha de comando, ele deve imprimir "wgrep: searchterm [file ...]" 
(seguido de uma nova linha) e sair com status 1.

_____________________________
 
Se wgrep encontrar um arquivo que não pode abrir, ele deve imprimir "wgrep: cannot open file"
(seguido de uma nova linha) e sair com status 1. Em todos os outros casos, wgrep deve sair com código de retorno 0.
_____________________________
 
Se um termo de pesquisa, mas nenhum arquivo, for especificado, wgrep deve funcionar, mas em vez de ler de um arquivo,
wgrep deve ler da entrada padrão. Fazer isso é fácil, porque o fluxo de arquivo stdin já está aberto; você pode usar 
fgets() (ou rotinas similares) para ler dele.

_____________________________
 
Para simplificar, se passar a string vazia como termo de pesquisa, wgrep pode ou não corresponder a nenhuma linha ou
corresponder a todas as linhas, ambas são aceitáveis.

*/