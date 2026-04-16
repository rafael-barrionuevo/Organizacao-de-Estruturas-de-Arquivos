#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
typedef struct _Endereco Endereco;
 
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
 
 
 
typedef struct _indiceEndereco indiceEndereco;
 
struct _indiceEndereco
{
    char cep[8];
    unsigned int posicao;
};

int compara(const void *a, const void *b) {
    indiceEndereco *x = (indiceEndereco *)a;
    indiceEndereco *y = (indiceEndereco *)b;

    // return strcmp(x->cep, y->cep);
    return strncmp(x->cep, y->cep, 8);
}
 
int main(int argc, char**argv)
{
    FILE *f, *s;
    Endereco e;
    int qt;
    int c;
 
    c = 0;
    // printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));
    f = fopen("cep.dat","rb");
   
    fseek(f,0,SEEK_END);
 
    long tamanhoBytes = ftell(f);
    long tamanhoRegistros = tamanhoBytes/sizeof(Endereco);
 
    indiceEndereco *a;
    a = malloc (tamanhoRegistros * sizeof (indiceEndereco));
 
    rewind(f);
 
    // fseek(f,meio*sizeof(Endereco),SEEK_SET);
    qt = fread(&e,sizeof(Endereco),1,f);
 
    s = fopen("indice.dat", "wb");
    int i =0;
 
    while(qt > 0){
        // strncpy(a[i].cep, e.cep, 8); estou lidando com bytes e não strings
        memcpy(a[i].cep, e.cep, 8);//memcpy lida com byte

        a[i].posicao = i;
        
        qt = fread(&e,sizeof(Endereco),1,f);
        i++;

    }
    qsort(a, tamanhoRegistros, sizeof(indiceEndereco),compara);

    /* for(int i = 0; i < 10; i++) {
        printf("%.8s | %d\n", a[i].cep, a[i].posicao);
    } */

    fwrite(a, sizeof(indiceEndereco), tamanhoRegistros, s);
    // printf("Total Lido: %d\n", i);
    printf("Criacao e ordenacao do indice completo");
    fclose(s);
    fclose(f);
}