#include <stdio.h>
#include <stdlib.h>

int main() {
    char nomeArquivo[256];
    FILE *arquivo;
    unsigned char *buffer;
    long tamanho;

    // Solicita o nome do arquivo
    printf("Digite o nome do arquivo: ");
    scanf("%255s", nomeArquivo);

    // Abre o arquivo em modo binário
    arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Move o cursor para o fim e descobre o tamanho
    fseek(arquivo, 0, SEEK_END);
    tamanho = ftell(arquivo);
    rewind(arquivo); // volta ao início do arquivo

    // Aloca memória para armazenar o conteúdo do arquivo
    buffer = (unsigned char*) malloc(tamanho);
    if (buffer == NULL) {
        perror("Erro de memória");
        fclose(arquivo);
        return 1;
    }

    // Lê os bytes do arquivo
    size_t lidos = fread(buffer, 1, tamanho, arquivo);
    if (lidos != tamanho) {
        perror("Erro ao ler o arquivo");
        free(buffer);
        fclose(arquivo);
        return 1;
    }

    // Exibe os bytes lidos (em hexadecimal)
    printf("Bytes do arquivo:\n");
    for (long i = 0; i < tamanho; i++) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }

    // Libera recursos
    free(buffer);
    fclose(arquivo);

    return 0;
}
