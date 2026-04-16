#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contadorArquivos = 0;

typedef struct {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
} Endereco;

int compara(const void *a, const void *b) {
    return strncmp(((Endereco*)a)->cep, ((Endereco*)b)->cep, 8);
}

void gera_blocos(int n) {
    FILE *f = fopen("cep.dat", "rb");
    if (!f) {
        printf("Erro ao abrir cep.dat\n");
        return;
    }

    fseek(f, 0, SEEK_END);
    long total = ftell(f) / sizeof(Endereco); //total -> qtd Enderecos do arquivo
    rewind(f);

    int qtb = total / n; // qtb -> 17/8 -- qtb = 2
    int sobra = total % n;// sobra -> 17%8 -- sobra = 1

    Endereco *buf = malloc((qtb + 1) * sizeof(Endereco)); //tamanho de buf = qtb + 1, tem um chorinho
    char nome[20];

    for (int i = 0; i < n; i++) { // for de 8, ou seja, 0 até 7
        int tam = (i < sobra ? qtb + 1 : qtb);// tam = qtb + 1

        fread(buf, sizeof(Endereco), tam, f);// vamo armazenar em buf 2 + 1

        qsort(buf, tam, sizeof(Endereco), compara);//vamos ordenar o buf

        sprintf(nome, "bloco%d.dat", contadorArquivos++);// aqui a gente define o nome do arquivo e armazena na variavel nome

        FILE *out = fopen(nome, "wb");// a gente gera um arquivo com o nome de b0.dat

        fwrite(buf, sizeof(Endereco), tam, out);
        fclose(out);
    }

    free(buf);
    fclose(f);
}

void merge_arquivos(char *arq1, char *arq2, char *saidaNome) {
    FILE *a = fopen(arq1, "rb");//b0
    FILE *b = fopen(arq2, "rb");//b1
    FILE *out = fopen(saidaNome, "wb");//b9

    if (!a || !b || !out) {
        printf("Erro ao abrir arquivos para merge\n");
        return;
    }

    Endereco ea, eb;

    int leuA = fread(&ea, sizeof(Endereco), 1, a);
    int leuB = fread(&eb, sizeof(Endereco), 1, b);

    while (leuA && leuB) {
        if (strncmp(ea.cep, eb.cep, 8) < 0) {
            fwrite(&ea, sizeof(Endereco), 1, out);
            leuA = fread(&ea, sizeof(Endereco), 1, a);
        } else {
            fwrite(&eb, sizeof(Endereco), 1, out);
            leuB = fread(&eb, sizeof(Endereco), 1, b);
        }
    }

    while (leuA) {
        fwrite(&ea, sizeof(Endereco), 1, out);
        leuA = fread(&ea, sizeof(Endereco), 1, a);
    }

    while (leuB) {
        fwrite(&eb, sizeof(Endereco), 1, out);
        leuB = fread(&eb, sizeof(Endereco), 1, b);
    }

    fclose(a);
    fclose(b);
    fclose(out);
}

void merge_total(int n) {
    int atual = n;
    int novo;

    char nome1[20], nome2[20], saida[20];

    int inicio = 0;

    while (atual > 1) {// 8 < 1
        novo = 0;

        for (int i = 0; i < atual; i += 2) { // incrementa 2 no i

            sprintf(nome1, "bloco%d.dat", inicio + i);// nome 1 = b0.dat

            if (i + 1 < atual) { // 0 + 1 < 8
                sprintf(nome2, "bloco%d.dat", inicio + i + 1);// nome 2 = b1.dat

                sprintf(saida, "bloco%d.dat", contadorArquivos++);// saida = b9.dat 

                merge_arquivos(nome1, nome2, saida);
            } else {
                sprintf(saida, "bloco%d.dat", contadorArquivos++);
                rename(nome1, saida);
            }

            novo++;
        }

        inicio = contadorArquivos - novo;
        atual = novo;
    }
}

int main() {

    int n = 8;// 8 blocos

    printf("Gerando blocos ordenados...\n");
    gera_blocos(n);

    printf("Realizando merges...\n");
    merge_total(n);

    printf("Arquivo final ordenado gerado!\n");

    return 0;
}