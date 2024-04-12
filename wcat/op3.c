#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

// registroCEP = struct.Struct("72s72s72s72s2s8s2s")

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2]; // Ao Espaço no final da linha + quebra de linha
};

int main(int argc, char**argv)
{
	FILE *f;
	Endereco e;
	int qt;
	int c;
	long tamanhoArquivo, primeiro, ultimo, meio;

	if(argc != 2)
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
	}
	c = 0;
	f = fopen("cep_ordenado.dat","rb");

	qt = fread(&e,sizeof(Endereco),1,f);
	fseek(f, 0, SEEK_END);
	tamanhoArquivo = ftell(f);
	rewind(f);


	primeiro = 0;
	ultimo = (tamanhoArquivo/sizeof(e));
	
	while(primeiro <= ultimo)
	{
		meio = (primeiro+ultimo)/2;
		c++;
		e.logradouro[71] = '\0';
		if(strncmp(argv[2],e.cep,8)==0)
		{
			printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
			break;
		} else if(strcmp(argv[1],e.cep) < 0) {
			primeiro = meio + 1;
		} else {
			ultimo = meio - 1;
		}
	}


	printf("Total Lido: %d\n", c);
	fclose(f);
}