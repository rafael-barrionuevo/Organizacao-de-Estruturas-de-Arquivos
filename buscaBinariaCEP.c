#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2]; 
};

int main(int argc, char**argv)
{
	FILE *f;
	Endereco e;
	int qt;
	int c;

	if(argc != 2)
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
	}

	c = 0;
	printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));
	f = fopen("cep_ordenado.dat","rb");
	fseek(f,0,SEEK_END);
	long tamanhoBytes = ftell(f);
	long tamanhoRegistros = tamanhoBytes/sizeof(Endereco);
	long inicio = 0;
	long fim = tamanhoRegistros-1;	
	
	while(inicio <= fim){
		c += 1;
		long meio = (inicio+fim)/2;
		fseek(f,meio*sizeof(Endereco),SEEK_SET);
		fread(&e,sizeof(Endereco),1,f);

		if(strncmp(e.cep,argv[1],8)==0){

			break;
		}
		else if (strncmp(argv[1], e.cep, 8) > 0) {
			inicio = meio +1;
		}
		else {
			fim = meio - 1;
		}
	}

    printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
	printf("Total Lido: %d\n", c);
	fclose(f);
}

