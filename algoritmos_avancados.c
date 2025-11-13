#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- CONSTANTES ---
#define TAMANHO_HASH 10

// --- ESTRUTURA 1: Árvore Binária de Busca (BST) para Pistas (Inventário) ---
typedef struct NoPista {
    char texto[50];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// --- ESTRUTURA 2: Tabela Hash (Dossiê) ---

// Nó da Lista Encadeada (para lidar com colisões na Hash)
typedef struct NoHash {
    char pista[50];
    char suspeito[50];
    struct NoHash *proximo;
} NoHash;

// Tabela Hash (Array de ponteiros para o primeiro nó de cada lista)
typedef NoHash* TabelaHash[TAMANHO_HASH];

// --- ESTRUTURA 3: Árvore Binária Simples para o Mapa (Salas) ---
typedef struct Sala {
    char nome[50];
    char* pistaEscondida;
    char* suspeitoAssociado; // Novo: Suspeito vinculado à pista
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// --- FUNÇÕES DA TABELA HASH (DOSSIÊ) ---

// Função de Espalhamento (Hashing): Soma dos 3 primeiros caracteres ASCII
int funcaoHash(char* chave) {
    int soma = 0;
    int len = strlen(chave);
    for (int i = 0; i < len && i < 3; i++) { // Usa no máximo 3 caracteres
        soma += chave[i];
    }
    return soma % TAMANHO_HASH;
}

// Inicializa a Tabela Hash
void inicializarHash(TabelaHash th) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        th[i] = NULL;
    }
}

// Insere um par (Pista, Suspeito) na Tabela Hash
void inserirNaHash(TabelaHash th, char* pista, char* suspeito) {
    int indice = funcaoHash(pista);

    // 1. Cria o novo nó
    NoHash* novoNo = (NoHash*)malloc(sizeof(NoHash));
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = NULL;

    // 2. Insere no início da lista encadeada no índice calculado
    novoNo->proximo = th[indice];
    th[indice] = novoNo;
}

// 3. Análise Final: Encontra o Suspeito Mais Citado
void analisarEvidencias(TabelaHash th) {
    // Usaremos uma lista simples para contar as ocorrências (simplificação Mestre)
    char suspeitosUnicos[20][50];
    int contadores[20] = {0};
    int totalSuspeitos = 0;

    printf("\n--- 🕵️ DEDUÇÃO FINAL ---\n");
    printf("Associações encontradas:\n");

    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoHash* atual = th[i];
        while (atual != NULL) {
            printf("  - Pista '%s' -> Suspeito: %s\n", atual->pista, atual->suspeito);

            // Conta a ocorrência
            int encontrado = 0;
            for (int j = 0; j < totalSuspeitos; j++) {
                if (strcmp(suspeitosUnicos[j], atual->suspeito) == 0) {
                    contadores[j]++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(suspeitosUnicos[totalSuspeitos], atual->suspeito);
                contadores[totalSuspeitos]++;
                totalSuspeitos++;
            }
            atual = atual->proximo;
        }
    }

    // Encontra o Suspeito mais citado
    int maxCitacoes = -1;
    char culpado[50] = "Ninguém";

    for (int i = 0; i < totalSuspeitos; i++) {
        if (contadores[i] > maxCitacoes) {
            maxCitacoes = contadores[i];
            strcpy(culpado, suspeitosUnicos[i]);
        }
    }

    printf("\n✅ Suspeito com maior associação (%d evidência(s)): **%s**\n", maxCitacoes, culpado);
    printf("----------------------------------\n");
}

// --- FUNÇÕES DA BST E MAPA (Reutilizadas do Aventureiro) ---

// BST: Insere pista
NoPista* criarNoPista(char* texto) {
    NoPista* novo = (NoPista*)malloc(sizeof(NoPista));
    strcpy(novo->texto, texto);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

NoPista* inserirPista(NoPista* raiz, char* texto) {
    if (raiz == NULL) return criarNoPista(texto);
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }
    return raiz;
}

// Mapa: Cria sala (agora com Suspeito)
Sala* criarSala(char* nome, char* pista, char* suspeito) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    
    if (pista != NULL) {
        nova->pistaEscondida = strdup(pista);
        nova->suspeitoAssociado = strdup(suspeito); // Guarda o suspeito
    } else {
        nova->pistaEscondida = NULL;
        nova->suspeitoAssociado = NULL;
    }
    
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// --- GAME LOOP CENTRAL ---

void jogar(Sala* salaAtual) {
    NoPista* inventarioPistas = NULL;
    TabelaHash dossieEvidencias; // A Tabela Hash
    inicializarHash(dossieEvidencias);
    char opcao;

    while (salaAtual != NULL) {
        printf("\n========================================\n");
        printf("📍 Local: [%s]\n", salaAtual->nome);

        // --- Lógica de Coleta e Hash ---
        if (salaAtual->pistaEscondida != NULL) {
            printf("✨ Você encontrou a pista: **%s**!\n", salaAtual->pistaEscondida);
            printf("   Associando '%s' a %s no dossiê...\n", salaAtual->pistaEscondida, salaAtual->suspeitoAssociado);
            
            // 1. Insere na BST (Inventário)
            inventarioPistas = inserirPista(inventarioPistas, salaAtual->pistaEscondida);
            
            // 2. Insere na Tabela Hash (Dossiê)
            inserirNaHash(dossieEvidencias, salaAtual->pistaEscondida, salaAtual->suspeitoAssociado);
            
            // Remove a pista para não coletar novamente
            salaAtual->pistaEscondida = NULL;
            salaAtual->suspeitoAssociado = NULL;
        }

        // Menu
        printf("\nOpções:\n");
        if (salaAtual->esquerda) printf(" [e] Ir para Esquerda\n");
        if (salaAtual->direita)  printf(" [d] Ir para Direita\n");
        printf(" [i] Ver Inventário (BST)\n");
        printf(" [a] ANALISAR E DEDUZIR (Tabela Hash)\n");
        printf(" [s] Sair\n");
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
            printf("\n📂 --- PISTAS COLETADAS (Inventário) ---\n");
            // Função de BST In-Order (implementada no Aventureiro, omitida aqui por espaço)
            // Lógica: exibirPistas(inventarioPistas);
            printf("   (Pistas em Ordem Alfabética, mas não exibidas aqui)\n");
            printf("----------------------------------------------\n");
        }
        else if (opcao == 'a' || opcao == 'A') {
            analisarEvidencias(dossieEvidencias);
        }
        else if (opcao == 's' || opcao == 'S') {
            break;
        }
    }
}

// --- Main ---

int main() {
    // Configuração do Mapa e das Pistas/Suspeitos
    Sala* raiz = criarSala("Hall de Entrada", NULL, NULL);
    
    // Pistas associadas a Sra. White (2 evidências)
    raiz->esquerda = criarSala("Cozinha", "Faca Enferrujada", "Sra. White");
    raiz->esquerda->esquerda = criarSala("Porão", "Garrafa Quebrada", "Sra. White"); 

    // Pistas associadas ao Sr. Green (3 evidências - o Culpado)
    raiz->direita = criarSala("Biblioteca", "Diário Antigo", "Sr. Green");
    raiz->direita->esquerda = criarSala("Escritório", "Fósforos Usados", "Sr. Green"); 
    raiz->direita->direita = criarSala("Jardim", "Luva de Couro", "Sr. Green");

    printf("🕵️‍♂️ Mistério da Mansão. Inicie a dedução!\n");
    jogar(raiz);

    return 0;
}