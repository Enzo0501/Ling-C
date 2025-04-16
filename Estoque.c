#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estrutura dos produtos
typedef struct Produto{
	char nome[100];
	int id;
	float preco;
	int quantidade;
}Produto;

//Estrututra da arvore binaria
typedef struct Node {
    Produto produto;
    struct Node* left;
    struct Node* right;
} Node;

//Funcao para cadastrar um produto
Node* criarNode(char nome[], int id, float preco, int quantidade) {
    Node* novo = (Node*)malloc(sizeof(Node));
    strcpy(novo->produto.nome, nome);
    novo->produto.id = id;
    novo->produto.preco = preco;
    novo->produto.quantidade = quantidade;
    novo->left = novo->right = NULL;
    return novo;
}

//Inserir produto na arvore Bin
Node* inserirProduto(Node* root, char nome[], int id, float preco, int quantidade) {
    if (root == NULL) {
        return criarNode(nome, id, preco, quantidade);
    }
    if (strcmp(nome, root->produto.nome) < 0) {
        root->left = inserirProduto(root->left, nome, id, preco, quantidade);
    } else if (strcmp(nome, root->produto.nome) > 0) {
        root->right = inserirProduto(root->right, nome, id, preco, quantidade);
    } else {
        printf("O produto ja foi cadastrado...\n");
    }
    return root;
}


//Buscar produto pelo ID
Node* buscarProduto(Node* root, int id) {
    if (root == NULL) return NULL;
    if (root->produto.id == id) return root;

    Node* esquerda = buscarProduto(root->left, id);
    if (esquerda != NULL) return esquerda;

    return buscarProduto(root->right, id);
}


//Remover produto
Node* encontrarMinimo(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

Node* removerProduto(Node* root, int id) {
    if (root == NULL) {
        return NULL;
    }

    // Busca o numero pelo ID
    if (id < root->produto.id) {
        root->left = removerProduto(root->left, id);
    } else if (id > root->produto.id) {
        root->right = removerProduto(root->right, id);
    } else {
        // Encontrou o produto para remover

        // Caso 1: Sem filhos
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }

        // Caso 2: Um filho
        else if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        // Caso 3: Dois filhos
        else {
            Node* temp = encontrarMinimo(root->right);
            root->produto = temp->produto; // Copia dados do sucessor
            root->right = removerProduto(root->right, temp->produto.id); // Remove o sucessor
        }
    }
    return root;
}


//Exibir produtos
void exibirProdutos(Node* root) {
    if (root != NULL) {
        exibirProdutos(root->left);
        printf("Nome: %s | ID: %d | Preco: %.2f | Quantidade: %d\n",
               root->produto.nome, root->produto.id, root->produto.preco, root->produto.quantidade);
        exibirProdutos(root->right);
    }
}


//Atualizar Produto
int atualizarProduto(Node* root, int id, float novoPreco, int novaQuantidade) {
    if (root == NULL) return 0;
    if (root->produto.id == id) {
        root->produto.preco = novoPreco;
        root->produto.quantidade = novaQuantidade;
        return 1;
    }
    if (atualizarProduto(root->left, id, novoPreco, novaQuantidade)) return 1;
    if (atualizarProduto(root->right, id, novoPreco, novaQuantidade)) return 1;
    return 0;
}

//Menu do Estoque
void menu(){
	
	printf("\n---- Sistema de Estoque de Produtos ----\n");
    printf("1 - Cadastrar produtos\n");
    printf("2 - Buscar Produto\n");
    printf("3 - Remover Produto\n");
    printf("4 - Exibir Produtos\n");
    printf("5 - Atualizar Produto\n");
    printf("0 - Sair\n");
	
}

int main(){
	
	Node* root = NULL;
	int opcao;
	char nome[100];
	float preco;
	int id;
	int quantidade;
	
	do{
		menu();
		printf("Escolha uma opcao: ");
		scanf("%d", &opcao);
		getchar();
		
		switch(opcao){
			case 1:
				printf("Insira o ID do produto: ");
				scanf("%d", &id);
				getchar();
				printf("Nome do produto: ");
				fgets(nome, 100, stdin);
				nome[strcspn(nome, "\n")] = 0;
				printf("Preco: ");
				scanf("%f", &preco);
				printf("Quantidade: ");
				scanf("%d", &quantidade);
				root = inserirProduto(root, nome, id, preco, quantidade);
				break;
				
			case 2:
			    printf("Digite o ID do produto para buscar: ");
			    scanf("%d", &id);
			    Node* encontrado = buscarProduto(root, id);
			    if (encontrado != NULL) {
			        printf("Produto encontrado!\n");
			        printf("ID: %d | Nome: %s | Preço: %.2f | Quantidade: %d\n",
			               encontrado->produto.id, encontrado->produto.nome, 
			               encontrado->produto.preco, encontrado->produto.quantidade);
			    } else {
			        printf("Produto com ID %d não encontrado.\n", id);
			    }
			    break;

			
			case 3:
				printf("Digite o ID do produto a ser removido: ");
			    scanf("%d", &id);
			    root = removerProduto(root, id);
			    printf("Produto removido com sucesso!\n");
			    break;
			
			case 4:
				printf("\nLista de Produtos:\n");
                exibirProdutos(root);
                break;
                
            case 5:
			    printf("Digite o ID do produto que deseja atualizar: ");
			    scanf("%d", &id);
			    printf("Novo preco: ");
			    scanf("%f", &preco);
			    printf("Nova quantidade: ");
			    scanf("%d", &quantidade);
			    if (atualizarProduto(root, id, preco, quantidade)) {
			        printf("Produto atualizado com sucesso!\n");
			    } else {
			        printf("Produto com ID %d nao encontrado!\n", id);
			    }
			    break;

            case 0:
            	printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
		}
	} while (opcao != 0);
	
	return 0;
}
