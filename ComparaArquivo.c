/*
    Este programa implementa o algoritmo SHA-256 para calcular o hash de dois arquivos e verificar se eles são autênticos, ou seja, se possuem o mesmo conteúdo.

    O SHA-256 (Secure Hash Algorithm 256 bits) é uma função hash criptográfica que gera um valor de 256 bits (32 bytes) único para dados de entrada de qualquer tamanho. Mesmo uma pequena alteração no arquivo resultará em um hash completamente diferente.

    O processo de verificação de autenticidade do arquivo é realizado através das seguintes etapas:

    1. **Leitura de Arquivos**:
        - O programa começa solicitando ao usuário o nome de dois arquivos. 
        - Em seguida, ele lê os dados desses arquivos em memória usando a função `ler_arquivo`. Esta função abre os arquivos, lê seu conteúdo em blocos de bytes e armazena em um buffer.

    2. **Cálculo do Hash SHA-256**:
        - Para cada arquivo lido, o programa aplica a função `sha256`, que implementa o algoritmo de hashing SHA-256.
        - O SHA-256 divide os dados em blocos de 512 bits e aplica uma série de transformações e operações lógicas sobre os dados para gerar o hash final de 256 bits.
        - O hash gerado é armazenado em um vetor de 32 bytes, que representa o valor único gerado para os dados do arquivo.

    3. **Comparação dos Hashes**:
        - Depois de calcular os hashes dos dois arquivos, o programa compara os valores resultantes byte a byte. Se os hashes forem iguais, isso significa que os arquivos possuem exatamente o mesmo conteúdo (são idênticos).
        - Caso contrário, os arquivos são considerados diferentes.

    4. **Exibição do Resultado**:
        - O programa imprime os hashes dos dois arquivos e, ao final, exibe uma mensagem indicando se os arquivos são "iguais" ou "diferentes" com base na comparação dos hashes.

    O algoritmo SHA-256 é implementado manualmente, sem o uso de bibliotecas externas, através das funções de transformação e do processo de preenchimento (padding) do SHA-256.

    Este programa tem como objetivo demonstrar a segurança do SHA-256 como ferramenta de verificação de integridade de dados, sendo útil para verificar a autenticidade de arquivos em diversas situações, como ao transferir ou armazenar dados sensíveis.

    O código a seguir implementa o funcionamento descrito acima.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Funções auxiliares para operações de SHA-256
#define ROTR(x, n) ((x >> n) | (x << (32 - n)))  // Rotação à direita de 32 bits
#define CH(x, y, z) ((x & y) ^ (~x & z))         // Função CH (condicional)
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z)) // Função MAJ (majority)
#define EP0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22)) // Função EP0 (early part 0)
#define EP1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25)) // Função EP1 (early part 1)
#define SIG0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))  // Função SIG0 (sigma 0)
#define SIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10)) // Função SIG1 (sigma 1)

// Constantes utilizadas no SHA-256
static const uint32_t k[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Função para realizar a transformação de dados (passo principal do SHA-256)
void sha256_transform(uint32_t state[8], const uint8_t data[]) {
    uint32_t w[64], a, b, c, d, e, f, g, h, t1, t2;
    
    // Prepara os 16 primeiros valores do array de palavras (w)
    for (int i = 0; i < 16; i++)
        w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | (data[i * 4 + 3]);
    
    // Preenche as próximas palavras (w[16] a w[63]) com base nas operações SIG0 e SIG1
    for (int i = 16; i < 64; i++)
        w[i] = SIG1(w[i - 2]) + w[i - 7] + SIG0(w[i - 15]) + w[i - 16];
    
    // Inicializa os valores das variáveis de estado (a, b, c, d, e, f, g, h)
    a = state[0]; b = state[1]; c = state[2]; d = state[3];
    e = state[4]; f = state[5]; g = state[6]; h = state[7];
    
    // Loop principal de 64 rodadas de transformação
    for (int i = 0; i < 64; i++) {
        t1 = h + EP1(e) + CH(e, f, g) + k[i] + w[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    
    // Atualiza os valores de estado com os resultados da transformação
    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    state[4] += e; state[5] += f; state[6] += g; state[7] += h;
}

// Função principal do SHA-256 que prepara os dados e chama a transformação
void sha256(const uint8_t *data, size_t len, uint8_t hash[32]) {
    // Inicializa os valores do estado com os valores constantes definidos no SHA-256
    uint32_t state[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    uint64_t bit_len = len * 8;  // Calcula o tamanho em bits dos dados
    size_t total_len = len + 1 + 8;  // Tamanho total, incluindo padding e comprimento
    size_t padded_len = ((total_len + 63) / 64) * 64;  // Calcula o comprimento total com padding de 512 bits

    uint8_t *padded = calloc(padded_len, 1);  // Aloca memória para os dados com padding
    memcpy(padded, data, len);  // Copia os dados originais para a memória
    padded[len] = 0x80;  // Adiciona o bit de preenchimento 0x80

    // Adiciona o comprimento dos dados no final (8 bytes)
    padded[padded_len - 8] = (bit_len >> 56) & 0xff;
    padded[padded_len - 7] = (bit_len >> 48) & 0xff;
    padded[padded_len - 6] = (bit_len >> 40) & 0xff;
    padded[padded_len - 5] = (bit_len >> 32) & 0xff;
    padded[padded_len - 4] = (bit_len >> 24) & 0xff;
    padded[padded_len - 3] = (bit_len >> 16) & 0xff;
    padded[padded_len - 2] = (bit_len >> 8)  & 0xff;
    padded[padded_len - 1] = (bit_len)       & 0xff;

    // Processa os dados em blocos de 512 bits (64 bytes)
    for (size_t i = 0; i < padded_len; i += 64)
        sha256_transform(state, padded + i);

    free(padded);  // Libera a memória utilizada para o padding

    // Converte o estado final para o valor do hash (32 bytes)
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++)
            hash[i * 4 + j] = (state[i] >> (24 - j * 8)) & 0xff;
}

// Função para ler o conteúdo de um arquivo binário
int ler_arquivo(const char *nome, uint8_t **dados, size_t *tamanho) {
    FILE *arquivo = fopen(nome, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    fseek(arquivo, 0, SEEK_END);
    *tamanho = ftell(arquivo);  // Obtém o tamanho do arquivo
    rewind(arquivo);

    *dados = malloc(*tamanho);  // Aloca memória para os dados do arquivo
    if (!*dados) {
        perror("Erro ao alocar memória");
        fclose(arquivo);
        return 0;
    }

    // Lê o conteúdo do arquivo para a memória
    if (fread(*dados, 1, *tamanho, arquivo) != *tamanho) {
        perror("Erro ao ler o arquivo");
        free(*dados);
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);  // Fecha o arquivo
    return 1;
}

// Função principal do programa que faz a verificação da autenticidade
int main() {
    char nome1[256], nome2[256];
    uint8_t *dados1, *dados2;
    size_t tam1, tam2;
    uint8_t hash1[32], hash2[32];

    // Solicita os nomes dos arquivos ao usuário
    printf("Digite o nome do primeiro arquivo: ");
    scanf("%255s", nome1);
    printf("Digite o nome do segundo arquivo: ");
    scanf("%255s", nome2);

    // Lê os arquivos
    if (!ler_arquivo(nome1, &dados1, &tam1)) return 1;
    if (!ler_arquivo(nome2, &dados2, &tam2)) {
        free(dados1);
        return 1;
    }

    // Calcula os hashes SHA-256 dos arquivos
    sha256(dados1, tam1, hash1);
    sha256(dados2, tam2, hash2);

    // Exibe os hashes calculados
    printf("\nHash do primeiro arquivo: ");
    for (int i = 0; i < 32; i++) printf("%02x", hash1[i]);

    printf("\nHash do segundo arquivo : ");
    for (int i = 0; i < 32; i++) printf("%02x", hash2[i]);

    // Compara os hashes e determina se os arquivos são iguais
    int iguais = memcmp(hash1, hash2, 32) == 0;
    printf("\n\nOs arquivos sao %s.\n", iguais ? "iguais" : "diferentes");

    free(dados1);  // Libera a memória alocada para os dados
    free(dados2);  // Libera a memória alocada para os dados
    return 0;
}
