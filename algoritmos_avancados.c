#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- ESTRUTURA 1: Árvore de Busca (BST) para Pistas ---

typedef struct NoPista {
    char texto[50];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// --- ESTRUTURA 2: Árvore Binária para o Mapa ---

typedef struct Sala {
    char nome[50];
    char* pistaEscondida;    // Pode ser NULL se não tiver pista
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// --- Funções da BST (Pistas) ---

NoPista* criarNoPista(char* texto) {
    NoPista* novo = (NoPista*)malloc(sizeof(NoPista));
    strcpy(novo->texto, texto);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Inserção ordenada (Lógica BST)
NoPista* inserirPista(NoPista* raiz, char* texto) {
    // 1. Se chegou num lugar vazio, cria o nó aqui
    if (raiz == NULL) {
        return criarNoPista(texto);
    }

    // 2. Compara strings para decidir esquerda ou direita
    // strcmp < 0: texto é "menor" (vem antes no alfabeto)
    // strcmp > 0: texto é "maior" (vem depois)
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }
    // Se for igual (0), não fazemos nada (evita duplicatas)

    return raiz;
}

// Percurso Em-Ordem (In-Order): E -> Raiz -> D
void exibirPistas(NoPista* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("  📝 - %s\n", raiz->texto);
        exibirPistas(raiz->direita);
    }
}

// --- Funções do Mapa (Salas) ---

Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    
    // Se passar uma string, aloca memória para ela. Se NULL, fica NULL.
    if (pista != NULL) {
        nova->pistaEscondida = strdup(pista); // strdup duplica a string na memória
    } else {
        nova->pistaEscondida = NULL;
    }
    
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// --- Game Loop ---

void jogar(Sala* salaAtual) {
    NoPista* inventarioPistas = NULL; // Raiz da BST começa vazia
    char opcao;

    while (salaAtual != NULL) {
        printf("\n========================================\n");
        printf("📍 Local: [%s]\n", salaAtual->nome);

        // --- Lógica de Coleta Automática ---
        if (salaAtual->pistaEscondida != NULL) {
            printf("✨ Você encontrou uma pista: \"%s\"!\n", salaAtual->pistaEscondida);
            // Insere na BST
            inventarioPistas = inserirPista(inventarioPistas, salaAtual->pistaEscondida);
            // Remove a pista da sala para não pegar de novo (opcional)
            salaAtual->pistaEscondida = NULL; 
        }

        // Menu
        printf("\nOpções:\n");
        if (salaAtual->esquerda) printf(" [e] Ir para Esquerda (%s)\n", salaAtual->esquerda->nome);
        if (salaAtual->direita)  printf(" [d] Ir para Direita (%s)\n", salaAtual->direita->nome);
        printf(" [i] Ver Inventário de Pistas\n");
        printf(" [s] Sair do Jogo\n");
        printf("👉 Escolha: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (salaAtual->esquerda) salaAtual = salaAtual->esquerda;
            else printf("⛔ Sem passagem!\n");
        }
        else if (opcao == 'd' || opcao == 'D') {
            if (salaAtual->direita) salaAtual = salaAtual->direita;
            else printf("⛔ Sem passagem!\n");
        }
        else if (opcao == 'i' || opcao == 'I') {
            printf("\n📂 --- PISTAS COLETADAS (Ordem Alfabética) ---\n");
            if (inventarioPistas == NULL) printf("  (Vazio)\n");
            else exibirPistas(inventarioPistas);
            printf("----------------------------------------------\n");
        }
        else if (opcao == 's' || opcao == 'S') {
            break;
        }
    }
}

// --- Main ---

int main() {
    // 1. Configuração do Mapa
    Sala* raiz = criarSala("Hall de Entrada", NULL);
    
    raiz->esquerda = criarSala("Cozinha", "Faca Enferrujada");
    raiz->direita = criarSala("Biblioteca", "Diário Antigo");
    
    raiz->esquerda->esquerda = criarSala("Porão", "Chave Prateada");
    raiz->direita->direita = criarSala("Observatório", "Mapa Estelar");

    /* Mapa:
             [Hall]
            /      \
       [Cozinha]  [Biblioteca]
         /              \
      [Porão]       [Observatório]
    */

    // 2. Iniciar
    printf("🕵️  Jogo iniciado! Explore e colete pistas.\n");
    jogar(raiz);

    return 0;
}