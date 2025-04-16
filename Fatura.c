#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Fatura {
    char data[100];
    char status[100];
    float valor;
    int numero;
} Fatura;

typedef struct Node {
    Fatura fatura;
    struct Node* left;
    struct Node* right;
} Node;

Node* criarFatura(char data[], char status[], float valor, int numero) {
    Node* novo = (Node*)malloc(sizeof(Node));
    if (novo == NULL) {
        printf("Erro ao alocar mem?ria!\n");
        exit(1);
    }
    strcpy(novo->fatura.data, data);
    strcpy(novo->fatura.status, status);
    novo->fatura.valor = valor;
    novo->fatura.numero = numero;
    novo->left = novo->right = NULL;
    return novo;
}

Node* inserirFatura(Node* root, char data[], char status[], float valor, int numero) {
    if (root == NULL) {
        return criarFatura(data, status, valor, numero);
    }
    if (strcmp(data, root->fatura.data) < 0) {
        root->left = inserirFatura(root->left, data, status, valor, numero);
    } else if (strcmp(data, root->fatura.data) > 0) {
        root->right = inserirFatura(root->right, data, status, valor, numero);
    } else {
        printf("Fatura ja cadastrada!\n");
    }
    return root;
}


void menu() {
    printf("\n---- Sistema de Faturas ----\n");
    printf("1 - Inserir fatura\n");
    printf("2 - Exibir faturas\n");
    printf("3 - Buscar fatura\n");
    printf("4 - Atualizacao de status de pagamento\n");
    printf("5 - Remocao de fatura\n");
    printf("0 - Sair\n");
}

int main(){

    Node* root = NULL;
    int opcao;
    char data[100], status[100];
    float valor;
    int numero;

    do{
        menu();
        printf("Escolha uma op??o: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer

        switch (opcao)
        {
        case 1:
            printf("Numero da fatura: ");
            scanf("%d", &numero);
            data[strcspn(data, "\n")] = 0;
            printf("Status da fatura: ");
            fgets(status, 100, stdin);
            status[strcspn(status, "\n")] = 0;
            printf("Data de vencimento: ");
            fgets(data, 100, stdin);
            printf("Valor: ");
            scanf("%f", &valor);
            root = inserirFatura(root, data, status, valor, numero);
            break;
    
        }
    }while (opcao != 0);
    
    return 0;
}

