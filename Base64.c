#include <stdio.h>
#include <string.h>

char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void encodeBlock(unsigned char *input, int len) {
    unsigned int buffer = 0;
    char output[4];
    int i;

    for (i = 0; i < len; i++) {
        buffer |= input[i] << (16 - 8 * i);
    }

    for (i = 0; i < 4; i++) {
        if (i <= (len + 1)) {
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

void encodeBase64() {
    char input[256]; 
    unsigned char block[3];
    int len;
    int j;

    printf("Digite a string para codificar: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; 

    printf("\nBase64: ");

    int i = 0;
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
    printf("\n");
}

int getBase64Value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

void decodeBase64() {
    char input[256];
    unsigned char output[3];
    int buffer;
    int i, j;
    
    printf("Digite a string Base64 para decodificar: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    
    int inputLen = strlen(input);
    
    printf("\nDecodificado: ");
    for (i = 0; i < inputLen; i += 4) {
        buffer = 0;
        int padding = 0;
        
        for (j = 0; j < 4; j++) {
            if (input[i + j] == '=') {
                padding++;
            } else {
                buffer = (buffer << 6) | getBase64Value(input[i + j]);
            }
        }
        
        for (j = 0; j < 3 - padding; j++) {
            output[j] = (buffer >> (16 - 8 * j)) & 0xFF;
            printf("%c", output[j]);
        }
    }
    printf("\n");
}

int main() {
    int escolha;
    
    do {
        printf("\nMenu:\n");
        printf("1. Codificar para Base64\n");
        printf("2. Decodificar Base64\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        getchar();
        
        switch (escolha) {
            case 1:
                encodeBase64();
                break;
            case 2:
                decodeBase64();
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (escolha != 3);
    
    return 0;
}
