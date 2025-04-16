#include <stdio.h>
#include <stdlib.h>

// Definição do nó da árvore
typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

// Função para criar um novo nó
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Função para inserir um valor na árvore
Node* insert(Node* root, int value) {
    if (root == NULL) return createNode(value);
    if (value < root->value)
        root->left = insert(root->left, value);
    else if (value > root->value)
        root->right = insert(root->right, value);
    return root;
}

// Função para encontrar o menor valor (usada para encontrar sucessor)
Node* findMin(Node* root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

// Função para remover um nó
Node* deleteNode(Node* root, int value) {
    if (root == NULL) return root;

    if (value < root->value) {
        root->left = deleteNode(root->left, value);
    } else if (value > root->value) {
        root->right = deleteNode(root->right, value);
    } else {
        // Caso 1 e 2: Nó com 0 ou 1 filho
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        // Caso 3: Nó com dois filhos
        Node* temp = findMin(root->right);
        root->value = temp->value;
        root->right = deleteNode(root->right, temp->value);
    }
    return root;
}

// Função para exibir a árvore (in-order)
void inOrder(Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->value);
        inOrder(root->right);
    }
}

// Função principal
int main() {
    Node* root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("Árvore em ordem: ");
    inOrder(root);
    printf("\n");

    printf("Removendo 20\n");
    root = deleteNode(root, 20);
    printf("Árvore em ordem: ");
    inOrder(root);
    printf("\n");

    printf("Removendo 30\n");
    root = deleteNode(root, 30);
    printf("Árvore em ordem: ");
    inOrder(root);
    printf("\n");

    printf("Removendo 50\n");
    root = deleteNode(root, 50);
    printf("Árvore em ordem: ");
    inOrder(root);
    printf("\n");

    return 0;
}

