#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Tabela Base64
char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Função para codificar um bloco de até 3 bytes em Base64
void encodeBlock(unsigned char *input, int len) {
    unsigned int buffer = 0;
    char output[4];
    int i;

    for (i = 0; i < len; i++) {
        buffer |= input[i] << (16 - 8 * i);
    }

    for (i = 0; i < 4; i++) {
        if (i <= len) {
            int index = (buffer >> (18 - 6 * i)) & 0x3F;
            output[i] = base64_table[index];
        } else {
            output[i] = '=';
        }
    }

    for (i = 0; i < 4; i++) {
        printf("%c", output[i]);
    }
}

// Codificação completa Base64
void encodeBase64(const char *input) {
    unsigned char block[3];
    int len, i = 0, j;
    int inputLen = strlen(input);

    while (i < inputLen) {
        len = 0;
        for (j = 0; j < 3; j++) {
            if (i < inputLen) {
                block[j] = input[i++];
                len++;
            } else {
                block[j] = 0;
            }
        }
        encodeBlock(block, len);
    }
}

// Decodificação Base64
int getBase64Value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

void decodeBase64(const char *input, char *output) {
    unsigned char buffer[3];
    int len = strlen(input);
    int i, j, index = 0, val;
    uint32_t temp;

    for (i = 0; i < len; i += 4) {
        temp = 0;
        int padding = 0;

        for (j = 0; j < 4; j++) {
            temp <<= 6;
            if (input[i + j] == '=') {
                padding++;
            } else {
                val = getBase64Value(input[i + j]);
                if (val >= 0) temp |= val;
            }
        }

        for (j = 0; j < 3 - padding; j++) {
            buffer[j] = (temp >> (16 - 8 * j)) & 0xFF;
            output[index++] = buffer[j];
        }
    }
    output[index] = '\0'; // Null-terminator
}

// SHA-256

#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define CH(x, y, z) ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define EP0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define EP1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SIG0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))
#define SIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))

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

void sha256_transform(uint32_t state[8], const uint8_t data[]) {
    uint32_t w[64], a, b, c, d, e, f, g, h, t1, t2;
    for (int i = 0; i < 16; i++)
        w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | (data[i * 4 + 3]);
    for (int i = 16; i < 64; i++)
        w[i] = SIG1(w[i - 2]) + w[i - 7] + SIG0(w[i - 15]) + w[i - 16];
    
    a = state[0]; b = state[1]; c = state[2]; d = state[3];
    e = state[4]; f = state[5]; g = state[6]; h = state[7];
    
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
    
    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    state[4] += e; state[5] += f; state[6] += g; state[7] += h;
}

void sha256(const uint8_t *data, size_t len, uint8_t hash[32]) {
    uint32_t state[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    uint64_t bit_len = len * 8;
    size_t total_len = len + 1 + 8;
    size_t padded_len = ((total_len + 63) / 64) * 64;

    uint8_t *padded = calloc(padded_len, 1);
    memcpy(padded, data, len);
    padded[len] = 0x80;

    padded[padded_len - 8] = (bit_len >> 56) & 0xff;
    padded[padded_len - 7] = (bit_len >> 48) & 0xff;
    padded[padded_len - 6] = (bit_len >> 40) & 0xff;
    padded[padded_len - 5] = (bit_len >> 32) & 0xff;
    padded[padded_len - 4] = (bit_len >> 24) & 0xff;
    padded[padded_len - 3] = (bit_len >> 16) & 0xff;
    padded[padded_len - 2] = (bit_len >> 8)  & 0xff;
    padded[padded_len - 1] = (bit_len)       & 0xff;

    for (size_t i = 0; i < padded_len; i += 64)
        sha256_transform(state, padded + i);

    free(padded);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++)
            hash[i * 4 + j] = (state[i] >> (24 - j * 8)) & 0xff;
}

// Utilitários
void print_hash(uint8_t hash[32]) {
    for (int i = 0; i < 32; i++)
        printf("%02x", hash[i]);
}

// Principal
int main() {
    int opcao;
    char mensagem[256], base64[400], mensagem_decodificada[256];
    uint8_t hash[32], hashCalculado[32];
    char hash_input[65];

    do {
        printf("\nMenu:\n");
        printf("1. Codificar mensagem e gerar hash\n");
        printf("2. Decodificar e verificar autenticidade\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            printf("Digite a mensagem: ");
            fgets(mensagem, sizeof(mensagem), stdin);
            mensagem[strcspn(mensagem, "\n")] = '\0';

            printf("\nMensagem codificada em Base64: ");
            encodeBase64(mensagem);
            printf("\n");

            sha256((uint8_t*)mensagem, strlen(mensagem), hash);
            printf("Hash SHA-256 da mensagem: ");
            print_hash(hash);
            printf("\n");

        } else if (opcao == 2) {
            printf("Digite a string Base64: ");
            fgets(base64, sizeof(base64), stdin);
            base64[strcspn(base64, "\n")] = '\0';

            decodeBase64(base64, mensagem_decodificada);
            printf("Mensagem decodificada: %s\n", mensagem_decodificada);

            printf("Digite o hash SHA-256 esperado: ");
            fgets(hash_input, sizeof(hash_input), stdin);
            hash_input[strcspn(hash_input, "\n")] = '\0';

            sha256((uint8_t*)mensagem_decodificada, strlen(mensagem_decodificada), hashCalculado);

            char hashHex[65];
            for (int i = 0; i < 32; i++) sprintf(&hashHex[i * 2], "%02x", hashCalculado[i]);

            if (strcmp(hashHex, hash_input) == 0) {
                printf("Mensagem AUTÊNTICA!\n");
            } else {
                printf("Mensagem ALTERADA ou Hash incorreto!\n");
                printf("Hash calculado: %s\n", hashHex);
            }
        }

    } while (opcao != 3);

    return 0;
}
