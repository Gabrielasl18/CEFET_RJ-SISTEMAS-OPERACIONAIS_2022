#include <stdio.h>
#include <stdlib.h>

// typedef struct _Registro Registro;

// struct _Registro {
//     char address[72];
//     char neighborhood[72];
//     char city[72];
//     char state[72];
//     char abbreviation[2];
//     char postal_code[8];
//     char trash[2];
// };


int main(int argc, char *argv) {

    
    FILE *arquivo; 
    char *buffer;
    long tamanho_arquivo;

    arquivo = fopen("numeros_ordenados.txt", "rb");
    
    fseek(arquivo, 0, SEEK_END);
    tamanho_arquivo = ftell(arquivo);
    /* faz com que o ponteiro de posição do arquivo do programa indicando 
    o prox byte do arquivo a ser lido ou gravado seja reposicionado no início do arquivo (byte 0)
    */
    rewind(arquivo); 

    buffer = (char *)malloc(tamanho_arquivo * sizeof(char));

    if(buffer == NULL) {
        perror("Erro ao alocar memória");
        return 1;
    }

    fread(buffer, sizeof(char), tamanho_arquivo ,arquivo);

    fclose(arquivo);
        
    printf("%s", buffer);

    free(buffer);

    return 0;
}