#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para medir o tempo, conforme sugerido

// *****************************************************************************
// 1. Definição das Structs
// *****************************************************************************
// Estrutura que representa um Item na mochila.
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

//Estrutura para um Nó da Lista Encadeada.

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// *****************************************************************************
// Variáveis Globais para Contadores
// *****************************************************************************
long long cont_comparacoes_seq = 0;
long long cont_comparacoes_bin = 0;

// *****************************************************************************
// Variáveis e Funções para a Mochila com Vetor (Lista Sequencial)
// *****************************************************************************

#define MAX_ITENS 10 // Tamanho máximo da mochila (vetor)
Item mochila_vetor[MAX_ITENS];
int total_itens_vetor = 0;

/**
 * @brief Requisito um novo item no vetor da mochila, se houver espaço.
 * @brief novo O item a ser inserido.
 *@return 1 se a inserção foi bem-sucedida, 0 caso contrário (mochila cheia).
 */

int inserirItemVetor(Item novo) {
    if (total_itens_vetor < MAX_ITENS) {
        // Copia os dados do novo item para a próxima posição livre do vetor
        strcpy(mochila_vetor[total_itens_vetor].nome, novo.nome);
        strcpy(mochila_vetor[total_itens_vetor].tipo, novo.tipo);
        mochila_vetor[total_itens_vetor].quantidade = novo.quantidade;
        total_itens_vetor++;
        return 1;
    }
    return 0; // Mochila cheia
}

/**
 * @brief Requisito 3 (Remover): Remove um item por nome do vetor.
 * Implementa deslocamento para manter a sequência.
 * @param nome O nome do item a ser removido.
 * @return 1 se removido, 0 se não encontrado.
 */

int removerItemVetor(const char *nome) {
    int i, j;
    for (i = 0; i < total_itens_vetor; i++) {
        if (strcmp(mochila_vetor[i].nome, nome) == 0) {
            // Item encontrado. Remove-o, deslocando os itens seguintes.
            for (j = i; j < total_itens_vetor - 1; j++) {
                mochila_vetor[j] = mochila_vetor[j + 1];
            }
            total_itens_vetor--;
            return 1;
        }
    }
    return 0; // Item não encontrado
}
/**
 * @brief Requisito 3 (Listar): Lista todos os itens do vetor.
 */
void listarItensVetor() {
    printf("\n--- Mochila (Vetor) - %d/%d itens ---\n", total_itens_vetor, MAX_ITENS);
    if (total_itens_vetor == 0) {
        printf("A mochila está vazia.\n");
        return;
    }
    for (int i = 0; i < total_itens_vetor; i++) {
        printf("  %d. Nome: %s, Tipo: %s, Quantidade: %d\n", 
               i + 1, mochila_vetor[i].nome, mochila_vetor[i].tipo, mochila_vetor[i].quantidade);
    }
    printf("--------------------------------------\n");
}
/**
 * @brief Requisito 4 (Ordenação): Ordena os itens no vetor por nome (Selection Sort).
 * @note A ordenação é crucial para habilitar a busca binária.
 */
 
void ordenarVetor() {
    // Implementação de Bubble Sort para ordenar o vetor por nome
    int i, j;
    Item temp;
    for (i = 0; i < total_itens_vetor - 1; i++) {
        for (j = 0; j < total_itens_vetor - 1 - i; j++) {
            // Comparação de strings para ordenação alfabética
            if (strcmp(mochila_vetor[j].nome, mochila_vetor[j + 1].nome) > 0) {
                // Troca os itens
                temp = mochila_vetor[j];
                mochila_vetor[j] = mochila_vetor[j + 1];
                mochila_vetor[j + 1] = temp;
            }
        }
    }
    printf("\nVetor ordenado com sucesso!\n");
}
/**
 * @brief Requisito 3 (Buscar Sequencial): Busca um item por nome no vetor.
 * @param nome O nome do item a buscar.
 * @return Ponteiro para o Item se encontrado, NULL caso contrário.
 */
 
int buscarSequencialVetor(const char *nome) {
    cont_comparacoes_seq = 0; // Reseta o contador
    for (int i = 0; i < total_itens_vetor; i++) {
        cont_comparacoes_seq++;
        if (strcmp(mochila_vetor[i].nome, nome) == 0) {
            return i; // Item encontrado
        }
    }
    return -1; // Item não encontrado
}
/**
 * @brief Requisito 5 (Busca Binária): Busca um item por nome no vetor (requer ordenação prévia).
 * @param nome O nome do item a buscar.
 * @return Ponteiro para o Item se encontrado, NULL caso contrário.
 */

int buscarBinariaVetor(const char *nome) {
    cont_comparacoes_bin = 0; // Reseta o contador
    int inicio = 0;
    int fim = total_itens_vetor - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int comparacao = strcmp(mochila_vetor[meio].nome, nome);
        cont_comparacoes_bin++; // Conta a comparação principal

        if (comparacao == 0) {
            return meio; // Item encontrado
        } else if (comparacao < 0) {
            inicio = meio + 1; // Item está na metade direita
        } else {
            fim = meio - 1; // Item está na metade esquerda
        }
    }
    return -1; // Item não encontrado
}

// *****************************************************************************
// Variáveis e Funções para a Mochila com Lista Encadeada (Estrutura Dinâmica)
// *****************************************************************************

No* cabeca = NULL; // Ponteiro para o primeiro nó da lista encadeada

/**
 * @brief Requisito 3 (Inserir): Insere um novo item na lista encadeada ou atualiza a quantidade.
 * @param novoItem O Item a ser inserido/atualizado.
 */

void inserirItemLista(Item novo) {
    // Aloca memória para o novo nó
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        perror("Erro ao alocar memória para o nó");
        return;
    }
    
    // Copia os dados
    novoNo->dados = novo;
    
    // Insere no início da lista (operação O(1) - rápida)
    novoNo->proximo = cabeca;
    cabeca = novoNo;
    printf("Item '%s' inserido com sucesso (Lista Encadeada).\n", novo.nome);
}

/**
 * @brief Requisito 3 (Remover): Remove um item por nome da lista encadeada.
 * @param nome O nome do item a ser removido.
 * @return 1 se removido, 0 se não encontrado.
 */
 
int removerItemLista(const char *nome) {
    No *atual = cabeca;
    No *anterior = NULL;

    // Percorre a lista
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            // Item encontrado.
            if (anterior == NULL) {
                // É o primeiro nó (cabeça)
                cabeca = atual->proximo;
            } else {
                // É um nó no meio ou no fim
                anterior->proximo = atual->proximo;
            }
            free(atual); // Libera a memória
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return 0; // Item não encontrado
}


 // Lista todos os itens presentes na lista encadeada.
 /**
 * @brief Requisito 3 (Listar): Lista todos os itens da lista encadeada.
 */

void listarItensLista() {
    No *atual = cabeca;
    int count = 0;
    
    printf("\n--- Mochila (Lista Encadeada) ---\n");
    if (cabeca == NULL) {
        printf("A mochila está vazia.\n");
        return;
    }
    
    while (atual != NULL) {
        count++;
        printf("  %d. Nome: %s, Tipo: %s, Quantidade: %d\n", 
               count, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
    printf("--------------------------------\n");
}

/**
 * @brief Requisito 3 (Buscar Sequencial): Busca um item por nome na lista encadeada.
 * @param nome O nome do item a buscar.
 * @return Ponteiro para o Item se encontrado, NULL caso contrário.
 */
 
No* buscarSequencialLista(const char *nome) {
    cont_comparacoes_seq = 0; // Reseta o contador
    No *atual = cabeca;

    // Percorre a lista (Busca Sequencial)
    while (atual != NULL) {
        cont_comparacoes_seq++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            return atual; // Item encontrado
        }
        atual = atual->proximo;
    }
    return NULL; // Item não encontrado
}

   // *****************************************************************************
  // Funções Auxiliares e Menu Principal
 // *****************************************************************************

/**
 * @brief Libera toda a memória alocada para a lista encadeada.
 */
 
void limparTela() {
    // Tenta usar o comando 'cls' (Windows) ou 'clear' (Unix/Linux/macOS)
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


  // Função para capturar os dados de um novo item.
 //return O item preenchido.
 
Item lerNovoItem() {
    Item novo;
    printf("Nome do Item (max 29 caracteres): ");
    scanf(" %29[^\n]", novo.nome); 
    printf("Tipo do Item (max 19 caracteres): ");
    scanf(" %19[^\n]", novo.tipo); 
    printf("Quantidade: ");
    while (scanf("%d", &novo.quantidade) != 1) {
        printf("Entrada inválida. Digite um número para a quantidade: ");
        while (getchar() != '\n'); // Limpa o buffer de entrada
    }
    while (getchar() != '\n'); // Limpa o restante do buffer
    return novo;
}


 //Função para gerenciar as operações do Vetor (Lista Sequencial).
 
void menuVetor() {
    int opcao;
    char nomeBusca[30];
    clock_t start, end;
    double cpu_time_used;
    
    do {
        limparTela();
        printf("\n\n--- Gerenciamento de Mochila (VETOR) ---\n");
        printf("1. Inserir Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Sequencial\n");
        printf("5. Ordenar Itens (Habilita Busca Binária)\n");
        printf("6. Buscar Binária\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; // Entrada inválida
            while (getchar() != '\n');
        }
        
        Item novo;
        int resultado;

        start = clock(); // Inicia a contagem de tempo
        
        switch (opcao) {
            case 1:
                novo = lerNovoItem();
                if (inserirItemVetor(novo)) {
                    printf("Item inserido com sucesso!\n");
                } else {
                    printf("ERRO: Mochila cheia (capacidade máxima: %d).\n", MAX_ITENS);
                }
                break;
            case 2:
                printf("Nome do item a remover: ");
                scanf(" %29[^\n]", nomeBusca);
                if (removerItemVetor(nomeBusca)) {
                    printf("Item '%s' removido com sucesso.\n", nomeBusca);
                } else {
                    printf("ERRO: Item '%s' não encontrado.\n", nomeBusca);
                }
                break;
            case 3:
                listarItensVetor();
                break;
            case 4:
                printf("Nome do item para busca sequencial: ");
                scanf(" %29[^\n]", nomeBusca);
                resultado = buscarSequencialVetor(nomeBusca);
                if (resultado != -1) {
                    printf("SUCESSO: Item encontrado na posição %d.\n", resultado + 1);
                    printf("Item: %s, Tipo: %s, Quantidade: %d\n", 
                           mochila_vetor[resultado].nome, mochila_vetor[resultado].tipo, mochila_vetor[resultado].quantidade);
                } else {
                    printf("ERRO: Item '%s' não encontrado.\n", nomeBusca);
                }
                printf("--- Análise de Performance ---\n");
                printf("Comparações realizadas (Busca Sequencial): %lld\n", cont_comparacoes_seq);
                break;
            case 5:
                ordenarVetor();
                break;
            case 6:
                if (total_itens_vetor == 0) {
                     printf("A mochila está vazia.\n");
                     break;
                }
                // Verifica se o vetor está ordenado (verificação simples, mas necessária)
                printf("ATENÇÃO: A busca binária SÓ FUNCIONARÁ CORRETAMENTE se a mochila tiver sido ORDENADA (Opção 5) recentemente.\n");
                printf("Nome do item para busca binária: ");
                scanf(" %29[^\n]", nomeBusca);
                resultado = buscarBinariaVetor(nomeBusca);
                if (resultado != -1) {
                    printf("SUCESSO: Item encontrado na posição %d.\n", resultado + 1);
                    printf("Item: %s, Tipo: %s, Quantidade: %d\n", 
                           mochila_vetor[resultado].nome, mochila_vetor[resultado].tipo, mochila_vetor[resultado].quantidade);
                } else {
                    printf("ERRO: Item '%s' não encontrado.\n", nomeBusca);
                }
                printf("--- Análise de Performance ---\n");
                printf("Comparações realizadas (Busca Binária): %lld\n", cont_comparacoes_bin);
                break;
            case 0:
                // Sai do loop
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
        
        end = clock(); // Finaliza a contagem de tempo
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        if (opcao != 0) {
            printf("\nTempo de Resposta: %.4f segundos (Requisito NF: < 2s).\n", cpu_time_used);
            printf("Pressione ENTER para continuar...");
            if (opcao != 1 && opcao != 2 && opcao != 4 && opcao != 6 && opcao != -1) {
                while (getchar() != '\n'); // Limpa buffer se necessário
            }
            getchar(); // Espera pelo ENTER
        }
        
    } while (opcao != 0);
}

 //Função para gerenciar as operações da Lista Encadeada.
 
void menuListaEncadeada() {
    int opcao;
    char nomeBusca[30];
    clock_t start, end;
    double cpu_time_used;

    do {
        limparTela();
        printf("\n\n--- Gerenciamento de Mochila (LISTA ENCAD.) ---\n");
        printf("1. Inserir Item (O(1))\n");
        printf("2. Remover Item (O(n))\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item (Busca Sequencial O(n))\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1; // Entrada inválida
            while (getchar() != '\n');
        }

        Item novo;
        No* resultado;
        
        start = clock();

        switch (opcao) {
            case 1:
                novo = lerNovoItem();
                inserirItemLista(novo);
                break;
            case 2:
                printf("Nome do item a remover: ");
                scanf(" %29[^\n]", nomeBusca);
                if (removerItemLista(nomeBusca)) {
                    printf("Item '%s' removido com sucesso.\n", nomeBusca);
                } else {
                    printf("ERRO: Item '%s' não encontrado.\n", nomeBusca);
                }
                break;
            case 3:
                listarItensLista();
                break;
            case 4:
                printf("Nome do item para busca sequencial: ");
                scanf(" %29[^\n]", nomeBusca);
                resultado = buscarSequencialLista(nomeBusca);
                if (resultado != NULL) {
                    printf("SUCESSO: Item encontrado.\n");
                    printf("Item: %s, Tipo: %s, Quantidade: %d\n", 
                           resultado->dados.nome, resultado->dados.tipo, resultado->dados.quantidade);
                } else {
                    printf("ERRO: Item '%s' não encontrado.\n", nomeBusca);
                }
                printf("--- Análise de Performance ---\n");
                printf("Comparações realizadas (Busca Sequencial): %lld\n", cont_comparacoes_seq);
                printf("\nOBS: Listas encadeadas não suportam Busca Binária de forma eficiente, pois não possuem acesso direto (O(1)) aos elementos por índice.\n");
                break;
            case 0:
                // Sai do loop
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        if (opcao != 0) {
            printf("\nTempo de Resposta: %.4f segundos (Requisito NF: < 2s).\n", cpu_time_used);
            printf("Pressione ENTER para continuar...");
            if (opcao != 1 && opcao != 2 && opcao != 4 && opcao != -1) {
                while (getchar() != '\n');
            }
            getchar();
        }
        
    } while (opcao != 0);
}


 //Função principal que exibe o menu inicial.
 
int main() {
    int opcao;

    do {
        limparTela();
        printf("====================================================\n");
        printf("  MOCHILA - SISTEMA DE INVENTÁRIO:  \n");
        printf("====================================================\n");
        printf("Escolha a estrutura de dados da mochila:\n");
        printf("1. Vetor (Lista Sequencial - Fixo e Ordenável)\n");
        printf("2. Lista Encadeada (Estrutura Dinâmica - Flexível)\n");
        printf("0. Sair do Programa\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1; // Entrada inválida
            while (getchar() != '\n');
        }

        switch (opcao) {
            case 1:
                menuVetor();
                break;
            case 2:
                menuListaEncadeada();
                break;
            case 0:
                printf("\nSaindo do jogo. Adeus!\n");
                // Libera memória da lista encadeada ao sair, se não estiver vazia
                No *atual = cabeca;
                No *proximo;
                while (atual != NULL) {
                    proximo = atual->proximo;
                    free(atual);
                    atual = proximo;
                }
                break;
            default:
                printf("Opção inválida. Pressione ENTER para tentar novamente...");
                while (getchar() != '\n');
                getchar();
                break;
        }
    } while (opcao != 0);

    return 0;
}