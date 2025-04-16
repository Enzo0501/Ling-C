#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar dados do livro
typedef struct Livro {
    char nome[100];
    char autor[100];
    float preco;
    int quantidade;
} Livro;

// Estrutura para a arvore binaria
typedef struct Node {
    Livro livro;
    struct Node* left;
    struct Node* right;
} Node;

// Fun��o para criar um novo no com um livro
Node* criarLivro(char nome[], char autor[], float preco, int quantidade) {
    Node* novo = (Node*)malloc(sizeof(Node));
    if (novo == NULL) {
        printf("Erro ao alocar mem�ria!\n");
        exit(1);
    }
    strcpy(novo->livro.nome, nome);
    strcpy(novo->livro.autor, autor);
    novo->livro.preco = preco;
    novo->livro.quantidade = quantidade;
    novo->left = novo->right = NULL;
    return novo;
}

// Funcao para inserir um livro na arvore
Node* inserirLivro(Node* root, char nome[], char autor[], float preco, int quantidade) {
    if (root == NULL) {
        return criarLivro(nome, autor, preco, quantidade);
    }
    if (strcmp(nome, root->livro.nome) < 0) {
        root->left = inserirLivro(root->left, nome, autor, preco, quantidade);
    } else if (strcmp(nome, root->livro.nome) > 0) {
        root->right = inserirLivro(root->right, nome, autor, preco, quantidade);
    } else {
        printf("Livro ja cadastrado!\n");
    }
    return root;
}

// Fun��o para buscar um livro pelo nome
Node* buscarLivro(Node* root, char nome[]) {
    if (root == NULL) return NULL;
    if (strcmp(nome, root->livro.nome) == 0) return root;
    if (strcmp(nome, root->livro.nome) < 0)
        return buscarLivro(root->left, nome);
    else
        return buscarLivro(root->right, nome);
}

// Funcao para alterar o preco do livro
void alterarPreco(Node* root, char nome[], float novoPreco) {
    Node* livro = buscarLivro(root, nome);
    if (livro != NULL) {
        livro->livro.preco = novoPreco;
        printf("Pre�o alterado com sucesso!\n");
    } else {
        printf("Livro n�o encontrado.\n");
    }
}

// Fun��o para alterar a quantidade do livro
void alterarQuantidade(Node* root, char nome[], int novaQuantidade) {
    Node* livro = buscarLivro(root, nome);
    if (livro != NULL) {
        livro->livro.quantidade = novaQuantidade;
        printf("Quantidade alterada com sucesso!\n");
    } else {
        printf("Livro n�o encontrado.\n");
    }
}

// Fun��o para listar os livros em ordem alfab�tica
void listarLivros(Node* root) {
    if (root != NULL) {
        listarLivros(root->left);
        printf("Nome: %s | Autor: %s | Pre�o: %.2f | Quantidade: %d\n",
               root->livro.nome, root->livro.autor, root->livro.preco, root->livro.quantidade);
        listarLivros(root->right);
    }
}

// Fun��o para exibir o menu
void menu() {
    printf("\n---- Sistema de Cadastro de Livros ----\n");
    printf("1 - Inserir novo livro\n");
    printf("2 - Buscar livro\n");
    printf("3 - Alterar pre�o do livro\n");
    printf("4 - Alterar quantidade do livro\n");
    printf("5 - Listar livros (ordem alfab�tica)\n");
    printf("0 - Sair\n");
}

int main() {
    Node* root = NULL;
    int opcao;
    char nome[100], autor[100];
    float preco;
    int quantidade;

    do {
        menu();
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer

        switch (opcao) {
            case 1:
                printf("Nome do livro: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Autor: ");
                fgets(autor, 100, stdin);
                autor[strcspn(autor, "\n")] = 0;
                printf("Pre�o: ");
                scanf("%f", &preco);
                printf("Quantidade: ");
                scanf("%d", &quantidade);
                root = inserirLivro(root, nome, autor, preco, quantidade);
                break;
            case 2:
                printf("Nome do livro para buscar: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0;
                Node* encontrado = buscarLivro(root, nome);
                if (encontrado != NULL) {
                    printf("Livro encontrado:\n");
                    printf("Nome: %s | Autor: %s | Pre�o: %.2f | Quantidade: %d\n",
                           encontrado->livro.nome, encontrado->livro.autor, encontrado->livro.preco, encontrado->livro.quantidade);
                } else {
                    printf("Livro n�o encontrado.\n");
                }
                break;
            case 3:
                printf("Nome do livro para alterar pre�o: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Novo pre�o: ");
                scanf("%f", &preco);
                alterarPreco(root, nome, preco);
                break;
            case 4:
                printf("Nome do livro para alterar quantidade: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Nova quantidade: ");
                scanf("%d", &quantidade);
                alterarQuantidade(root, nome, quantidade);
                break;
            case 5:
                printf("\nLista de livros:\n");
                listarLivros(root);
                break;
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Op��o inv�lida!\n");
        }
    } while (opcao != 0);

    return 0;
}

