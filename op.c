#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Record Record;
typedef struct _Index Index;

struct _Record {
    char address[72];
    char neighborhood[72];
    char city[72];
    char state[72];
    char abbreviation[2];
    char postal_code[8];
    char trash[2];
};

struct _Index {
    char postal_code[8];
    int position; 
};

int compare(const void *e1, const void *e2) {
    return strncmp(((Index*)e1)->postal_code, ((Index*)e2)->postal_code, 8); 
}

// binary_search(argv[1], index_array, quantity);
// recebe o ponteiro apontando para o codigo postal, o ponteiro apontando para struct com os indices e o tamanho do arquivo 
int binary_search(const char* postal_code, long quantity) {
    /*é aberto novamente porque esta função está realizando uma busca binária nos registros desse arquivo, utilizando um 
    arquivo de índice (index_array) apenas para obter as posições dos registros no arquivo cep_ordenado.dat.*/
    FILE *file = fopen("cep_ordenado.dat","rb");
    /*nova struct de endereco*/
    Record record;

    int start = 0;
    int end = quantity - 1;

    while (start <= end) {
        int middle = (start + end) / 2;

        fseek(file, middle * sizeof(Record), SEEK_SET);
        fread(&record, sizeof(Record), 1, file);

        if (strncmp(postal_code, record.postal_code, 8) == 0) {
            printf("Encontrado na posição %d\n", middle);
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", record.address, record.neighborhood, record.city, record.state, record.abbreviation, record.postal_code);
            fclose(file);
            return 0;
        } else if (strncmp(postal_code, record.postal_code, 8) < 0) {
            end = middle - 1;
        } else {
            start = middle + 1;
        }
    }

    printf("CEP não encontrado.\n");
    fclose(file);
    return 1;
} 

int main(int argc, char** argv) {
    Record record;
    Index index;
    long last_byte, quantity;

    // le o arquivo binario com os enderecos
    FILE *file_addresses = fopen("cep_ordenado.dat", "rb");
    // cria um novo arquivo .txt para escrita de binarios
    FILE *file_indices = fopen("index.dat", "wb");
  
    // pergunta se tem mais/menos de um argumento sendo passado na chamada o programa
    if (argc != 2) {
        fprintf(stderr, "Uso: %s [CEP]\n", argv[0]);
        return 1;
    }

    //declara um contador
    int count = 0; 

    // le o arquivo com os enderecos 
    while (fread(&record, sizeof(Record), 1, file_addresses) == 1) {
        // copia o codigo postal da struct endereco para struct index
        strcpy(index.postal_code, record.postal_code); 
        // contador pra saber a posicao e coloca na struct index
        index.position = count;
        // escreve no arquivo binario apontando para a nova struct que foi formada, o 1 é pra dizer que vai ler só 1 struct que é index
        fwrite(&index, sizeof(Index), 1, file_indices); 
        count++;
    }
    printf("O índice foi criado!\n");

    // fecha os arquivos
    fclose(file_addresses);
    fclose(file_indices);

    // abre arquivo index
    file_indices = fopen("index.dat", "rb");


    // coloca o ponteiro no ultimo indice do arquivo
    fseek(file_indices, 0, SEEK_END);
    // retorna a posição atual do ponteiro de arquivo em relação ao início do arquivo
    last_byte = ftell(file_indices); 
    // determinar a quantidade de registros (ou entradas) em um arquivo em bytes, com o sizeof que determina a quantidade de bytes da struct index
    quantity = last_byte / sizeof(Index);

    // posiciona o ponteiro no inicio do arquivo
    rewind(file_indices); 

    // cria um ponteiro apontando para uma nova estrutura index
    Index* index_array; 
    /*
    Determinação do tamanho necessário: O objetivo é alocar memória 
    para armazenar um array de estruturas Index, onde o número total de elementos é quantity.

    Tamanho de cada elemento: Para calcular o tamanho total necessário da memória, precisamos
    saber o tamanho de cada elemento individual. Isso é obtido usando sizeof(Index), que retorna
    o tamanho, em bytes, de uma estrutura Index.

    Multiplicação: Multiplicamos o número total de elementos (quantity) pelo tamanho de cada
    elemento (sizeof(Index)) que é da Struct Index. Isso nos dá o tamanho total necessário 
    para alocar memória para
    o array inteiro.
    
    */
    index_array = (Index*) malloc(quantity * sizeof(Index)); 
    
    /*
    faz a leitura dos arquivos endereco para index_array q é struct Index
    index_array = ponteiro q vai apontar pro arquivo de struct index
    sizeof(Index) = quantidade de bytes dentro da struct Index
    quantity = tamanho do arquivo file_indices (com todos os indices dos enderecos)
    */
    fread(index_array, sizeof(Index), quantity, file_indices);

    /*
    chama a funcao qsort para ordenar a struct com os indices passando
    index_array = a struct index_array com todos os indices disponiveis que foram trazidos de file_indices
    quantity = tamanho o arquivo file_indices q contem todos os indices dos enderecos
    sizeof(Index) = quantidade de bytes dentro da struct Index
    compare = chama a função existente no programa
    */
    qsort(index_array, quantity, sizeof(Index), compare);
    // fecha o arquivo com os indices
    fclose(file_indices);

    // abre o arquivo com os indices para escrita
    file_indices = fopen("index.dat", "wb");

    //escreve a struct ordenada no arquivo, passando q quantidade de bytes da struct Index e o tamanho do arquivo
    fwrite(index_array, sizeof(Index), quantity, file_indices);

    // faz a busca binaria na struct ordenada 
    binary_search(argv[1], quantity);

    // libera o ponteiro
    free(index_array);

    // fecha o arquivo
    fclose(file_indices);

    return 0;
}
