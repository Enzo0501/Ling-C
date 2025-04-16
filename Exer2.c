#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int valor;
    struct Node* left;
    struct Node* right;
} Node;

// Estrutura da fila para BFS
typedef struct QueueNode {
    Node* node;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Função para criar um novo nó
Node* criarNo(int valor) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
    novoNo->valor = valor;
    novoNo->left = NULL;
    novoNo->right = NULL;
    return novoNo;
}

// Função para inserir elemento na árvore
Node* inserir(Node* raiz, int valor) {
    if (raiz == NULL) {
        return criarNo(valor);
    }

    if (valor < raiz->valor) {
        raiz->left = inserir(raiz->left, valor);
    } else if (valor > raiz->valor) {
        raiz->right = inserir(raiz->right, valor);
    }
    return raiz;
}

// Função para imprimir os elementos da árvore em ordem crescente (in-order traversal)
void imprimirEmOrdem(Node* raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->left);
        printf("%d ", raiz->valor);
        imprimirEmOrdem(raiz->right);
    }
}

// Função para buscar um elemento na árvore
Node* buscarElemento(Node* raiz, int elemento) {
    if (raiz == NULL || raiz->valor == elemento) {
        return raiz;
    }
    
    if (elemento < raiz->valor) {
        return buscarElemento(raiz->left, elemento);
    } else {
        return buscarElemento(raiz->right, elemento);
    }
}

// Função auxiliar para encontrar o menor valor de uma subárvore
Node* encontrarMinimo(Node* raiz) {
    while (raiz->left != NULL) {
        raiz = raiz->left;
    }
    return raiz;
}

// Função para remover um elemento da árvore
Node* removerElemento(Node* raiz, int elemento) {
    if (raiz == NULL) {
        return NULL;
    }

    if (elemento < raiz->valor) {
        raiz->left = removerElemento(raiz->left, elemento);
    } else if (elemento > raiz->valor) {
        raiz->right = removerElemento(raiz->right, elemento);
    } else {
        // Caso 1: Nó sem filhos
        if (raiz->left == NULL && raiz->right == NULL) {
            free(raiz);
            return NULL;
        }
        // Caso 2: Nó com um filho
        else if (raiz->left == NULL) {
            Node* temp = raiz->right;
            free(raiz);
            return temp;
        } else if (raiz->right == NULL) {
            Node* temp = raiz->left;
            free(raiz);
            return temp;
        }
        // Caso 3: Nó com dois filhos
        else {
            Node* temp = encontrarMinimo(raiz->right);
            raiz->valor = temp->valor;  // Copia o valor do sucessor
            raiz->right = removerElemento(raiz->right, temp->valor);  // Remove o sucessor
        }
    }
    return raiz;
}

//Funcao para calcular a altura da arvore
int alturaArvore(Node* raiz) {
    if (raiz == NULL) {
        return -1; // Caso base: arvore vazia
    }
    
    int alturaEsquerda = alturaArvore(raiz->left);
    int alturaDireita = alturaArvore(raiz->right);
    
    return 1 + (alturaEsquerda > alturaDireita ? alturaEsquerda : alturaDireita);
}

// Funcao auxiliar para verificar balanceamento e calcular altura ao mesmo tempo
int verificarBalanceamentoAux(Node* raiz, int* altura) {
    if (raiz == NULL) {
        *altura = -1;
        return 1;
    }

    int alturaEsq, alturaDir;
    int balanceadaEsq = verificarBalanceamentoAux(raiz->left, &alturaEsq);
    int balanceadaDir = verificarBalanceamentoAux(raiz->right, &alturaDir);

    *altura = 1 + (alturaEsq > alturaDir ? alturaEsq : alturaDir);

    if (!balanceadaEsq || !balanceadaDir) return 0; // Se alguma subrvore nao for balanceada
    if (abs(alturaEsq - alturaDir) > 1) return 0; // Diferenca de altura maior que 1

    return 1; // arvore balanceada
}

// Funcao principal para verificar se a arvore e balanceada
int verificarBalanceamento(Node* raiz) {
    int altura = 0;
    return verificarBalanceamentoAux(raiz, &altura);
}

// Funcao para percorrer a arvore em pre-ordem
void imprimirPreOrdem(Node* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        imprimirPreOrdem(raiz->left);
        imprimirPreOrdem(raiz->right);
    }
}

// Funcao para percorrer a arvore em pas-ordem
void imprimirPosOrdem(Node* raiz) {
    if (raiz != NULL) {
        imprimirPosOrdem(raiz->left);
        imprimirPosOrdem(raiz->right);
        printf("%d ", raiz->valor);
    }
}

// Funcees auxiliares para fila
Queue* criarFila() {
    Queue* fila = (Queue*)malloc(sizeof(Queue));
    fila->front = fila->rear = NULL;
    return fila;
}

void enfileirar(Queue* fila, Node* node) {
    QueueNode* novo = (QueueNode*)malloc(sizeof(QueueNode));
    novo->node = node;
    novo->next = NULL;
    if (fila->rear == NULL) {
        fila->front = fila->rear = novo;
        return;
    }
    fila->rear->next = novo;
    fila->rear = novo;
}

Node* desenfileirar(Queue* fila) {
    if (fila->front == NULL) return NULL;
    QueueNode* temp = fila->front;
    Node* node = temp->node;
    fila->front = fila->front->next;
    if (fila->front == NULL) fila->rear = NULL;
    free(temp);
    return node;
}

int filaVazia(Queue* fila) {
    return fila->front == NULL;
}

// **Funcao para verificar se a arvore e completa**
int verificarCompleta(Node* raiz) {
    if (raiz == NULL) return 1;

    Queue* fila = criarFila();
    enfileirar(fila, raiz);
    
    int encontrouNulo = 0;

    while (!filaVazia(fila)) {
        Node* atual = desenfileirar(fila);

        if (atual == NULL) {
            encontrouNulo = 1;
        } else {
            if (encontrouNulo) {
                return 0; // Se encontramos um no nao nulo depois de um nulo, a arvore nao e completa
            }
            enfileirar(fila, atual->left);
            enfileirar(fila, atual->right);
        }
    }
    return 1; // Se seguimos a regra corretamente, a arvore e completa
}

//Funcao para contar os nos
int contarNos(Node* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contarNos(raiz->left) + contarNos(raiz->right);
}

// Funcao para encontrar o menor valor na arvore (no mais a esquerda)
int encontrarMinimoValor(Node* raiz) {
    if (raiz == NULL) {
        printf("A arvore esta vazia.\n");
        return -1;
    }
    while (raiz->left != NULL) {
        raiz = raiz->left;
    }
    return raiz->valor;
}

// Funcao para encontrar o maior valor na arvore (no mais a direita)
int encontrarMaximoValor(Node* raiz) {
    if (raiz == NULL) {
        printf("A arvore esta vazia.\n");
        return -1;
    }
    while (raiz->right != NULL) {
        raiz = raiz->right;
    }
    return raiz->valor;
}

// Funcao para calcular a soma de todos os nos na arvore
int calcularSomaNos(Node* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return raiz->valor + calcularSomaNos(raiz->left) + calcularSomaNos(raiz->right);
}



int main() {
    Node* raiz = NULL;
    int opcao, valor, elemento;

    do {
        printf("\nMenu:\n");
        printf("1. Inserir elemento\n");
        printf("2. Imprimir em ordem crescente\n");
        printf("3. Buscar elemento\n");
        printf("4. Remover elemento\n");
        printf("5. Calcular a altura da arvore\n");
        printf("6. Verificar se a arvore e balanceada\n");
        printf("7. Imprimir em pre-ordem\n");
        printf("8. Imprimir em em-ordem\n");
        printf("9. Imprimir em pos-ordem\n");
        printf("10. Verificar se a arvore e completa\n");
        printf("11. Contar numero de nos\n");
        printf("12. Encontrar o menor valor\n");
		printf("13. Encontrar o maior valor\n");
		printf("14. Calcular a soma de todos os nos\n");
        printf("15. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;
            case 2:
                printf("Elementos da árvore em ordem crescente: ");
                imprimirEmOrdem(raiz);
                printf("\n");
                break;
            case 3:
                printf("Digite o elemento que deseja buscar: ");
                scanf("%d", &elemento);
                if (buscarElemento(raiz, elemento)) {
                    printf("Elemento encontrado!\n");
                } else {
                    printf("Elemento nao encontrado!\n");
                }
                break;
            case 4:
                printf("Digite o elemento que deseja remover: ");
                scanf("%d", &elemento);
                raiz = removerElemento(raiz, elemento);
                printf("Elemento removido com sucesso!\n");
                break;
            case 5:
            	printf("Altura da arvore: %d\n", alturaArvore(raiz));
                break;
            case 6:
                if (verificarBalanceamento(raiz)) {
                    printf("A arvore esta balanceada.\n");
                } else {
                    printf("A arvore NaO esta balanceada.\n");
                }
                break;
            case 7:
                printf("Percurso Pre-Ordem: ");
                imprimirPreOrdem(raiz);
                printf("\n");
                break;
            case 8:
                printf("Percurso Em-Ordem: ");
                imprimirEmOrdem(raiz);
                printf("\n");
                break;
            case 9:
                printf("Percurso Pos-Ordem: ");
                imprimirPosOrdem(raiz);
                printf("\n");
                break;
            case 10:
                if (verificarCompleta(raiz)) {
                    printf("A arvore e completa.\n");
                } else {
                    printf("A arvore NAO e completa.\n");
                }
                break;
            case 11:
                printf("Numero total de nos na arvore: %d\n", contarNos(raiz));
                break;
            case 12:
			    printf("Menor valor na arvore: %d\n", encontrarMinimoValor(raiz));
			    break;
			case 13:
			    printf("Maior valor na arvore: %d\n", encontrarMaximoValor(raiz));
			    break;
			case 14:
			    printf("Soma de todos os nos da arvore: %d\n", calcularSomaNos(raiz));
			    break;
            case 15:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 15);

    return 0;
}