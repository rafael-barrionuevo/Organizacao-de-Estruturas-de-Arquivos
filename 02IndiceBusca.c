#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
} Endereco;

typedef struct {
    char cep[8];
    unsigned int posicao;
} indiceEndereco;

int main(int argc, char **argv) {

    if(argc < 2) {
        printf("Uso: %s CEP\n", argv[0]);
        return 1;
    }

    FILE *fi = fopen("indice.dat", "rb");
    if(fi == NULL) {
        printf("Erro ao abrir indice.dat\n");
        return 1;
    }

    // Descobrir quantidade de registros no índice
    fseek(fi, 0, SEEK_END);
    long tamanhoBytes = ftell(fi);
    long n = tamanhoBytes / sizeof(indiceEndereco);
    rewind(fi);

    // CEP buscado
    char cepBuscado[8];
    memcpy(cepBuscado, argv[1], 8);

    // Busca binária
    int esquerda = 0;
    int direita = n - 1;
    int meio;
    indiceEndereco temp;
    int achou = 0;

    //qsort(

    while(esquerda <= direita) {
        meio = (esquerda + direita) / 2;

        fseek(fi, meio * sizeof(indiceEndereco), SEEK_SET);
        fread(&temp, sizeof(indiceEndereco), 1, fi);

        int cmp = strncmp(cepBuscado, temp.cep, 8);

        if(cmp == 0) {
            achou = 1;
            break;
        }
        else if(cmp < 0) {
            direita = meio - 1;
        }
        else {
            esquerda = meio + 1;
        }
    }

    fclose(fi);

    if(!achou) {
        printf("CEP nao encontrado\n");
        return 0;
    }

    // Agora acessa o arquivo original
    FILE *fd = fopen("cep.dat", "rb");
    if(fd == NULL) {
        printf("Erro ao abrir cep.dat\n");
        return 1;
    }

    // Vai direto na posição
    fseek(fd, temp.posicao * sizeof(Endereco), SEEK_SET);

    Endereco e;
    fread(&e, sizeof(Endereco), 1, fd);

    fclose(fd);

    // Imprime
    printf("Logradouro: %.72s\n", e.logradouro);
    printf("Bairro: %.72s\n", e.bairro);
    printf("Cidade: %.72s\n", e.cidade);
    printf("UF: %.72s\n", e.uf);
    printf("Sigla: %.2s\n", e.sigla);
    printf("CEP: %.8s\n", e.cep);

    return 0;
}