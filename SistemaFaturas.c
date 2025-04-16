//Avaliacao A2 Enzo Januario e Nicolas Ferraz
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar as informacoes da fatura
typedef struct Fatura {
    char data[100];
    char status[100];
    float valor;
    int numero;
} Fatura;

// Estrutura do no da arvore binaria
typedef struct Node {
    Fatura fatura;
    struct Node* left;
    struct Node* right;
} Node;

// Funcao para criar uma nova fatura
Node* criarFatura(int numero, char data[], char status[], float valor) {
    Node* novo = (Node*)malloc(sizeof(Node));
    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    novo->fatura.numero = numero;
    strcpy(novo->fatura.data, data);
    strcpy(novo->fatura.status, status);
    novo->fatura.valor = valor;
    novo->left = novo->right = NULL;
    return novo;
}

// Funcao para inserir uma fatura na arvore
Node* inserirFatura(Node* root, int numero, char data[], char status[], float valor) {
    if (root == NULL) {
        return criarFatura(numero, data, status, valor);
    }
    if (numero < root->fatura.numero) {
        root->left = inserirFatura(root->left, numero, data, status, valor);
    } else if (numero > root->fatura.numero) {
        root->right = inserirFatura(root->right, numero, data, status, valor);
    } else {
        printf("Fatura jÃ¡ cadastrada!\n");
    }
    return root;
}

// Funcao para exibir as faturas em ordem crescente de numero
void exibirFaturas(Node* root) {
    if (root != NULL) {
        exibirFaturas(root->left);
        printf("NÃºmero: %d | Data: %s | Valor: %.2f | Status: %s\n",
               root->fatura.numero, root->fatura.data, root->fatura.valor, root->fatura.status);
        exibirFaturas(root->right);
    }
}

// Funcao para buscar uma fatura pelo numero
Node* buscarFatura(Node* root, int numero) {
    if (root == NULL || root->fatura.numero == numero) {
        return root;
    }
    if (numero < root->fatura.numero) {
        return buscarFatura(root->left, numero);
    } else {
        return buscarFatura(root->right, numero);
    }
}

// Funcao para alterar o status de pagamento de uma fatura
int alterarStatus(Node* root, int numero, char novoStatus[]) {
    Node* fatura = buscarFatura(root, numero);
    if (fatura != NULL) {
        strcpy(fatura->fatura.status, novoStatus);
        return 1;
    }
    return 0;
}

Node* encontrarMinimo(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Funcao para remover uma fatura da arvore
Node* removerFatura(Node* root, int numero) {
    if (root == NULL) {
        return NULL;
    }
    if (numero < root->fatura.numero) {
        root->left = removerFatura(root->left, numero);
    } else if (numero > root->fatura.numero) {
        root->right = removerFatura(root->right, numero);
    } else {
        //Sem filhos
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        //Um filho
        else if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        //Dois filhos
        else {
            Node* temp = encontrarMinimo(root->right);
            root->fatura = temp->fatura;
            root->right = removerFatura(root->right, temp->fatura.numero);
        }
    }
    return root;
}

// Exibe o menu
void menu() {
    printf("\n---- Sistema de Faturas ----\n");
    printf("1 - Inserir fatura\n");
    printf("2 - Exibir faturas\n");
    printf("3 - Buscar fatura\n");
    printf("4 - Atualizar status de pagamento\n");
    printf("5 - Remover fatura\n");
    printf("0 - Sair\n");
}

int main() {
    Node* root = NULL;
    int opcao, numero;
    char data[100], status[100];
    float valor;

    do {
        menu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Numero da fatura: ");
                scanf("%d", &numero);
                getchar();
                printf("Data de vencimento: ");
                fgets(data, 100, stdin);
                data[strcspn(data, "\n")] = 0;
                printf("Status da fatura (pendente/paga): ");
                fgets(status, 100, stdin);
                status[strcspn(status, "\n")] = 0;
                printf("Valor: ");
                scanf("%f", &valor);
                root = inserirFatura(root, numero, data, status, valor);
                break;

            case 2:
                printf("\nLista de Faturas:\n");
                exibirFaturas(root);
                break;

            case 3:
                printf("Digite o numero da fatura para buscar: ");
                scanf("%d", &numero);
                Node* encontrada = buscarFatura(root, numero);
                if (encontrada) {
                    printf("Fatura encontrada: Numero: %d | Data: %s | Valor: %.2f | Status: %s\n",
                           encontrada->fatura.numero, encontrada->fatura.data,
                           encontrada->fatura.valor, encontrada->fatura.status);
                } else {
                    printf("Fatura nao encontrada!\n");
                }
                break;

            case 4:
                printf("Numero da fatura: ");
                scanf("%d", &numero);
                getchar();
                printf("Novo status (pendente/paga): ");
                fgets(status, 100, stdin);
                status[strcspn(status, "\n")] = 0;
                if (alterarStatus(root, numero, status)) {
                    printf("Status atualizado!\n");
                } else {
                    printf("Fatura nao encontrada!\n");
                }
                break;

            case 5:
                printf("Numero da fatura a remover: ");
                scanf("%d", &numero);
                root = removerFatura(root, numero);
                printf("Fatura removida!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}