#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Definição da Estrutura ---

// Cada "Nó" da árvore é uma Sala
typedef struct Sala {
    char nome[50];           // Dado (Payload)
    struct Sala *esquerda;   // Ponteiro para o caminho da esquerda
    struct Sala *direita;    // Ponteiro para o caminho da direita
} Sala;

// --- Funções Auxiliares ---

// Função para alocar e criar uma nova sala na memória
Sala* criarSala(char* nomeSala) {
    Sala* nova = (Sala*)malloc(sizeof(Sala)); // Aloca memória
    if (nova == NULL) {
        printf("Erro de memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nomeSala); // Copia o nome para a estrutura
    nova->esquerda = NULL;        // Inicializa sem filhos
    nova->direita = NULL;
    return nova;
}

// Função principal de navegação (Game Loop)
void explorarSalas(Sala* atual) {
    char opcao;

    printf("\n🔦 --- INÍCIO DA EXPLORAÇÃO ---\n");

    // Loop continua enquanto o jogador não sair ou chegar num beco sem saída
    while (atual != NULL) {
        printf("\n📍 Você está em: [%s]\n", atual->nome);

        // Verifica se é um nó folha (sem saídas)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("👻 Este é um beco sem saída (Nó Folha). Fim da linha!\n");
            break;
        }

        printf("Para onde deseja ir?\n");
        if (atual->esquerda != NULL) printf(" [e] Esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL)  printf(" [d] Direita (%s)\n", atual->direita->nome);
        printf(" [s] Sair da mansão\n");
        
        printf("👉 Escolha: ");
        scanf(" %c", &opcao); // O espaço antes de %c ignora quebras de linha pendentes

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda; // Move o ponteiro para o nó da esquerda
            } else {
                printf("⛔ Não há porta à esquerda!\n");
            }
        } 
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita; // Move o ponteiro para o nó da direita
            } else {
                printf("⛔ Não há porta à direita!\n");
            }
        } 
        else if (opcao == 's' || opcao == 'S') {
            printf("🏃 Você fugiu da mansão!\n");
            break;
        } 
        else {
            printf("⚠️ Opção inválida.\n");
        }
    }
}

// --- Função Principal ---

int main() {
    // 1. Construção da Árvore (O Mapa da Mansão)
    // A estrutura é montada manualmente conectando os ponteiros
    
    Sala* raiz = criarSala("Hall de Entrada");
    
    // Lado Esquerdo da Mansão
    raiz->esquerda = criarSala("Sala de Jantar");
    raiz->esquerda->esquerda = criarSala("Cozinha"); // Folha
    raiz->esquerda->direita = criarSala("Despensa"); // Folha
    
    // Lado Direito da Mansão
    raiz->direita = criarSala("Biblioteca");
    raiz->direita->esquerda = criarSala("Escritório"); // Folha
    raiz->direita->direita = criarSala("Jardim de Inverno"); // Folha

    /*
       Estrutura Visual da Árvore:
                 [Hall de Entrada]
                /                 \
        [Sala de Jantar]       [Biblioteca]
          /        \            /         \
      [Cozinha] [Despensa] [Escritório] [Jardim]
    */

    // 2. Iniciar o Jogo
    explorarSalas(raiz);

    // (Opcional) Aqui deveríamos ter uma função para liberar a memória (free)
    // mas focaremos na estrutura para o nível Novato.

    return 0;
}