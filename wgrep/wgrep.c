#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;

    //ve se tem argumentos de linha de comando
    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }
    
    //se tiver só 1 argumento, ele vai ler a entrada padrão
    if (argc == 1) {
        fp = stdin;
    } else{
        //se tiver mais de 1 argumento, vai abrir o arquivo pedido
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            printf("wgrep: cannot open file\n");
            exit(1);
        }
    }

    //loop para processar cada linha do arquivo ou entrada padrão
    while (getline(&line, &len, fp) != -1) {
        for (int i = 1; i < argc; i++) {
            if (strstr(line, argv[i]) != NULL) {
                printf("%s", line);
                break; //interrompe a verificação se a palavra for encontrada, só assim nao procura várias vezes
            }
        }
    }

    if (line != NULL)
        free(line); 

    if (fp != stdin)
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