#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2]; 
};

int main(int argc, char** argv) {
    FILE *f;
    Endereco e;
    int c = 0;

    if(argc != 2) {
        fprintf(stderr, "USO: %s [CEP]\n", argv[0]);
        return 1;
    }

    f = fopen("cep_ordenado.dat", "rb"); // "rb" para garantir leitura binária correta
    if(!f) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    // 1. Definir os limites baseados em ÍNDICES (0 até N-1)
    int inicio = 0;
    fseek(f, 0, SEEK_END);
    int final = (ftell(f) / sizeof(Endereco)) - 1;

    printf("Tamanho da Estrutura: %ld\n", sizeof(Endereco));
    printf("Total de registros no arquivo: %d\n\n", final + 1);

    // 2. A condição correta: enquanto o intervalo de busca for válido
    while(inicio <= final) {
        c++;
        int meio = (inicio + final) / 2;
        
        // 3. Posiciona e lê o registro do meio
        fseek(f, meio * sizeof(Endereco), SEEK_SET);
        if (fread(&e, sizeof(Endereco), 1, f) != 1) break;

        printf("Tentativa %d - Meio: %d (CEP: %.8s)\n", c, meio, e.cep);

        int cmp = strncmp(argv[1], e.cep, 8);

        if(cmp == 0) { // ACHOU
            printf("\n--- Encontrado! ---\n");
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", 
                   e.logradouro, e.bairro, e.cidade, e.uf, e.sigla, e.cep);
            printf("Total de registros lidos: %d\n", c);
            fclose(f);
            return 0;
        } 
        else if(cmp > 0) { // O que eu quero é MAIOR que o atual, olha pra direita
            inicio = meio + 1;
        } 
        else { // O que eu quero é MENOR que o atual, olha pra esquerda
            final = meio - 1;
        }
    }

    printf("\nCEP não encontrado. Total de registros lidos: %d\n", c);
    fclose(f);
    return 0;
}