#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------
// Estrutura do Usuário (AVL)
// -------------------------
typedef struct Usuario {
    char nome[100];
    int id;
    char email[100];
    int altura;
    struct Usuario *esq, *dir;
} Usuario;

// -------------------------
// Estrutura do Produto (Rubro-Negra)
// -------------------------
typedef enum { VERMELHO, PRETO } Cor;

typedef struct Produto {
    int codigo;
    char nome[100];
    int quantidade;
    float preco;
} Produto;

typedef struct Node {
    Produto produto;
    Cor cor;
    struct Node *esquerda, *direita, *pai;
} Node;

// Função auxiliar para criar novo nó vermelho
Node* criar_node(Produto p) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->produto = p;
    novo->cor = VERMELHO;
    novo->esquerda = novo->direita = novo->pai = NULL;
    return novo;
}

Node* raiz_rn = NULL;

// Função auxiliar para rotacionar à esquerda
Node* rotacao_esquerda(Node* raiz, Node* x) {
    Node* y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda) y->esquerda->pai = x;
    y->pai = x->pai;
    if (!x->pai) raiz = y;
    else if (x == x->pai->esquerda) x->pai->esquerda = y;
    else x->pai->direita = y;
    y->esquerda = x;
    x->pai = y;
    return raiz;
}

// Função auxiliar para rotacionar à direita
Node* rotacao_direita(Node* raiz, Node* x) {
    Node* y = x->esquerda;
    x->esquerda = y->direita;
    if (y->direita) y->direita->pai = x;
    y->pai = x->pai;
    if (!x->pai) raiz = y;
    else if (x == x->pai->direita) x->pai->direita = y;
    else x->pai->esquerda = y;
    y->direita = x;
    x->pai = y;
    return raiz;
}

// Corrigir inserção
Node* corrigir_insercao(Node* raiz, Node* z) {
    while (z->pai && z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) {
            Node* y = z->pai->pai->direita;
            if (y && y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    raiz = rotacao_esquerda(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                raiz = rotacao_direita(raiz, z->pai->pai);
            }
        } else {
            Node* y = z->pai->pai->esquerda;
            if (y && y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    raiz = rotacao_direita(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                raiz = rotacao_esquerda(raiz, z->pai->pai);
            }
        }
    }
    raiz->cor = PRETO;
    return raiz;
}

// Inserir produto na árvore rubro-negra
Node* inserir_rn(Node* raiz, Produto p) {
    Node* z = criar_node(p);
    Node* y = NULL;
    Node* x = raiz;

    while (x) {
        y = x;
        if (p.codigo < x->produto.codigo)
            x = x->esquerda;
        else
            x = x->direita;
    }

    z->pai = y;
    if (!y)
        raiz = z;
    else if (p.codigo < y->produto.codigo)
        y->esquerda = z;
    else
        y->direita = z;

    return corrigir_insercao(raiz, z);
}

// Buscar produto pelo código
Node* buscar(Node* raiz, int codigo) {
    if (!raiz || raiz->produto.codigo == codigo)
        return raiz;
    if (codigo < raiz->produto.codigo)
        return buscar(raiz->esquerda, codigo);
    else
        return buscar(raiz->direita, codigo);
}

// Listar produtos (in-order)
void listar(Node* raiz) {
    if (raiz) {
        listar(raiz->esquerda);
        printf("Código: %d | Nome: %s | Qtd: %d | Preço: %.2f | Cor: %s\n",
               raiz->produto.codigo, raiz->produto.nome,
               raiz->produto.quantidade, raiz->produto.preco,
               raiz->cor == VERMELHO ? "Vermelho" : "Preto");
        listar(raiz->direita);
    }
}

// -------------------------
// Menu principal (usuários AVL + produtos RN)
// -------------------------
int main() {
    Usuario *raiz = NULL;
    int opcao, id;
    char nome[100], email[100];

    do {
        printf("\n--- Sistema Principal ---\n");
        printf("1. Cadastrar Usuário\n2. Remover Usuário\n3. Buscar Usuário\n4. Listar Usuários\n");
        printf("5. Cadastrar Produto\n6. Buscar Produto\n7. Listar Produtos\n0. Sair\nEscolha: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao >= 1 && opcao <= 4) {
            switch(opcao) {
                case 1:
                    printf("Nome: "); fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = 0;
                    printf("ID: "); scanf("%d", &id); getchar();
                    printf("Email: "); fgets(email, 100, stdin); email[strcspn(email, "\n")] = 0;
                    raiz = inserir_usuario(raiz, nome, id, email);
                    break;
                case 2:
                    printf("Nome para remover: "); fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = 0;
                    raiz = remover_usuario(raiz, nome);
                    break;
                case 3:
                    printf("Nome para buscar: "); fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = 0;
                    Usuario *u = buscar_usuario(raiz, nome);
                    if (u)
                        printf("Encontrado -> Nome: %s, ID: %d, Email: %s\n", u->nome, u->id, u->email);
                    else
                        printf("Usuário não encontrado.\n");
                    break;
                case 4:
                    listar_usuarios(raiz);
                    break;
            }
        } else if (opcao == 5) {
            Produto p;
            printf("Código: "); scanf("%d", &p.codigo); getchar();
            printf("Nome: "); fgets(p.nome, 100, stdin); p.nome[strcspn(p.nome, "\n")] = 0;
            printf("Quantidade: "); scanf("%d", &p.quantidade);
            printf("Preço: "); scanf("%f", &p.preco);
            raiz_rn = inserir_rn(raiz_rn, p);
        } else if (opcao == 6) {
            int codigo;
            printf("Código do produto: "); scanf("%d", &codigo);
            Node *encontrado = buscar(raiz_rn, codigo);
            if (encontrado)
                printf("Produto -> Código: %d | Nome: %s | Qtd: %d | Preço: %.2f\n",
                       encontrado->produto.codigo, encontrado->produto.nome,
                       encontrado->produto.quantidade, encontrado->produto.preco);
            else
                printf("Produto não encontrado.\n");
        } else if (opcao == 7) {
            listar(raiz_rn);
        } else if (opcao == 0) {
            printf("Saindo...\n");
        } else {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}
