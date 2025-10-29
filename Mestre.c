// Biblioteca inclusas.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // Para tolower() em comparação de strings se necessário

// Constante para o número máximo de componentes
#define MAX_COMPONENTES 20
// Constante para o nome do componente-chave
#define COMPONENTE_CHAVE "chip central"

// 1. Criação da struct Componente
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade; // De 1 a 10
} Componente;

// Protótipos das Funções Obrigatórias

// Funções de Suporte e Exibição
void mostrarComponentes(Componente comps[], int n);
void limparBuffer();
void cadastrarComponentes(Componente comps[], int *n);
void inicializarDados(Componente comps[], int *n);
void copiarComponentes(Componente origem[], Componente destino[], int n);

// Funções de Ordenação com Contagem de Comparações e Tempo
void bubbleSortNome(Componente comps[], int n, long long *comparacoes);
void insertionSortTipo(Componente comps[], int n, long long *comparacoes);
void selectionSortPrioridade(Componente comps[], int n, long long *comparacoes);

// Função de Busca
int buscaBinariaPorNome(Componente comps[], int n, const char chave[]);

// Função de Menu Principal
void menuPrincipal();


// --- IMPLEMENTAÇÕES DAS FUNÇÕES DE SUPORTE E UTILITÁRIOS ---

/**
 * @brief Limpa o buffer de entrada.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Exibe a lista de componentes em formato tabular.
 * @param comps Array de componentes.
 * @param n Número de componentes.
 */
void mostrarComponentes(Componente comps[], int n) {
    if (n == 0) {
        printf("\n[Nenhum componente cadastrado.]\n");
        return;
    }
    printf("\n---  Lista de Componentes da Torre de Fuga (%d Itens)  ---\n", n);
    printf("+------------------------------+--------------------+------------+\n");
    printf("| NOME (Prioridade para Nome)  | TIPO (Prioridade p/ Tipo) | PRIORIDADE (Prioridade p/ Pri) |\n");
    printf("+------------------------------+--------------------+------------+\n");
    for (int i = 0; i < n; i++) {
        printf("| %-28s | %-18s | %-10d |\n", comps[i].nome, comps[i].tipo, comps[i].prioridade);
    }
    printf("+------------------------------+--------------------+------------+\n");
}

/**
 * @brief Copia os dados de um array de componentes para outro.
 * Usado para garantir que os algoritmos de ordenação trabalhem em uma cópia.
 * @param origem Array de origem.
 * @param destino Array de destino.
 * @param n Número de componentes.
 */
void copiarComponentes(Componente origem[], Componente destino[], int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origem[i];
    }
}

/**
 * @brief Inicializa o array de componentes com dados de teste.
 * @param comps Array de componentes.
 * @param n Ponteiro para o número de componentes.
 */
void inicializarDados(Componente comps[], int *n) {
    Componente dadosIniciais[] = {
        {"Bateria de Emergencial", "Energia", 10},
        {"Cabo de Forca", "Suporte", 8},
        {"Antena Base", "Controle", 6},
        {"Chip Eletronico", "Controle", 9},
        {"Sensor de Altitude", "Contole", 3},
        {"Bico Propusor", "Propulsao", 5},
        {"Painel Solar", "Energia", 7},
        {"Estrutura Base", "Suporte", 4}
    };
    int tamanho = sizeof(dadosIniciais) / sizeof(Componente);
    copiarComponentes(dadosIniciais, comps, tamanho);
    *n = tamanho;
    printf("\n[ Dados de teste preenchidos com %d componentes.]\n", *n);
}

/**
 * @brief Permite ao jogador cadastrar novos componentes.
 * @param comps Array de componentes.
 * @param n Ponteiro para o número de componentes atual.
 */
void cadastrarComponentes(Componente comps[], int *n) {
    char temp[30];
    int p, count = *n;

    printf("\n---  Cadastro de Componentes ---\n");
    printf("Voce pode cadastrar ate %d componentes (atualmente %d cadastrados).\n", MAX_COMPONENTES, count);

    while (count < MAX_COMPONENTES) {
        printf("\nComponente #%d:\n", count + 1);

        // 1. Nome
        printf("Nome (max 29 caracteres): ");
        if (fgets(temp, sizeof(temp), stdin) == NULL) break;
        temp[strcspn(temp, "\n")] = 0; // Remove newline
        if (strlen(temp) == 0) break; // Parar se nome vazio
        strcpy(comps[count].nome, temp);

        // 2. Tipo
        printf("Tipo (ex: Controle, Suporte, Propulsao - max 19 caracteres): ");
        if (fgets(temp, sizeof(temp), stdin) == NULL) break;
        temp[strcspn(temp, "\n")] = 0; // Remove newline
        strcpy(comps[count].tipo, temp);

        // 3. Prioridade
        do {
            printf("Prioridade (1 a 10): ");
            if (scanf("%d", &p) != 1) {
                printf("Entrada invalida. Digite um numero.\n");
                limparBuffer();
                p = -1;
            } else {
                limparBuffer();
            }
        } while (p < 1 || p > 10);
        comps[count].prioridade = p;

        count++;
        printf("\nComponente cadastrado. Deseja cadastrar outro? (Pressione ENTER para continuar ou qualquer outra tecla + ENTER para parar): ");
        if (getchar() != '\n') {
            limparBuffer();
            break;
        }
    }
    *n = count;
    printf("\n[ Cadastro finalizado. Total de %d componentes.]\n", *n);
}

// --- IMPLEMENTAÇÕES DOS ALGORITMOS DE ORDENAÇÃO ---

/**
 * @brief Implementa o Bubble Sort para ordenar por Nome (string).
 * @param comps Array de componentes.
 * @param n Número de componentes.
 * @param comparacoes Ponteiro para armazenar o número de comparações.
 */
void bubbleSortNome(Componente comps[], int n, long long *comparacoes) {
    *comparacoes = 0;
    Componente temp;
    int trocado;
    clock_t inicio = clock();

    for (int i = 0; i < n - 1; i++) {
        trocado = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++; // Contagem de comparação: strcmp
            // Note: Usando strcmp. Para uma ordenação que não diferencia maiúsculas/minúsculas, 
            // usaríamos strcasecmp (se disponível) ou implementariamos uma função de comparação customizada.
            if (strcmp(comps[j].nome, comps[j + 1].nome) > 0) {
                // Troca
                temp = comps[j];
                comps[j] = comps[j + 1];
                comps[j + 1] = temp;
                trocado = 1;
            }
        }
        if (trocado == 0) break; // Otimização: se não houve troca, o array está ordenado
    }

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC * 1000.0; // Tempo em milissegundos

    printf("\n---  Relatorio Bubble Sort por Nome ---\n");
    printf("Comparacoes realizadas: %lld\n", *comparacoes);
    printf("Tempo de execucao: %.6f ms\n", tempo);
    printf("----------------------------------------\n");
}

/**
 * @brief Implementa o Insertion Sort para ordenar por Tipo (string).
 * @param comps Array de componentes.
 * @param n Número de componentes.
 * @param comparacoes Ponteiro para armazenar o número de comparações.
 */
void insertionSortTipo(Componente comps[], int n, long long *comparacoes) {
    *comparacoes = 0;
    Componente chave;
    int j;
    clock_t inicio = clock();

    for (int i = 1; i < n; i++) {
        chave = comps[i];
        j = i - 1;

        // Compara o Tipo da chave com os elementos anteriores
        while (j >= 0) {
            (*comparacoes)++; // Contagem de comparação: strcmp
            if (strcmp(comps[j].tipo, chave.tipo) > 0) {
                comps[j + 1] = comps[j];
                j = j - 1;
            } else {
                break; // Parar se estiver na posição correta
            }
        }
        comps[j + 1] = chave;
    }

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC * 1000.0; // Tempo em milissegundos

    printf("\n---  Relatorio Insertion Sort por Tipo ---\n");
    printf("Comparacoes realizadas: %lld\n", *comparacoes);
    printf("Tempo de execucao: %.6f ms\n", tempo);
    printf("--------------------------------------------\n");
}

/**
 * @brief Implementa o Selection Sort para ordenar por Prioridade (int).
 * @param comps Array de componentes.
 * @param n Número de componentes.
 * @param comparacoes Ponteiro para armazenar o número de comparações.
 */
void selectionSortPrioridade(Componente comps[], int n, long long *comparacoes) {
    *comparacoes = 0;
    int min_idx;
    Componente temp;
    clock_t inicio = clock();

    for (int i = 0; i < n - 1; i++) {
        min_idx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++; // Contagem de comparação: Prioridade
            // Ordenação decrescente: maior prioridade (10) primeiro
            if (comps[j].prioridade > comps[min_idx].prioridade) {
                min_idx = j;
            }
        }

        // Troca o elemento encontrado pelo primeiro elemento não ordenado
        if (min_idx != i) {
            temp = comps[i];
            comps[i] = comps[min_idx];
            comps[min_idx] = temp;
        }
    }

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC * 1000.0; // Tempo em milissegundos

    printf("\n---  Relatorio Selection Sort por Prioridade ---\n");
    printf("Comparacoes realizadas: %lld\n", *comparacoes);
    printf("Tempo de execucao: %.6f ms\n", tempo);
    printf("-------------------------------------------------\n");
}

// --- IMPLEMENTAÇÃO DA BUSCA BINÁRIA ---

/**
 * @brief Implementa a Busca Binária para localizar o componente-chave por Nome.
 * Requer que o array esteja ordenado por Nome.
 * @param comps Array de componentes (DEVE ESTAR ORDENADO POR NOME).
 * @param n Número de componentes.
 * @param chave Nome do componente a ser buscado.
 * @return O índice do componente ou -1 se não for encontrado.
 */
int buscaBinariaPorNome(Componente comps[], int n, const char chave[]) {
    int inicio = 0;
    int fim = n - 1;
    int meio;
    int resultado_comparacao;
    int comparacoes = 0;

    printf("\n--- 🔍 Iniciando Busca Binaria por '%s' ---\n", chave);

    while (inicio <= fim) {
        meio = inicio + (fim - inicio) / 2; // Previne overflow

        // Compara o nome do meio com a chave
        resultado_comparacao = strcmp(comps[meio].nome, chave);
        comparacoes++;

        // Verifica se a chave está presente no meio
        if (resultado_comparacao == 0) {
            printf("[ CHAVE ENCONTRADA!] Item localizado em comps[%d]. Comparacoes: %d\n", meio, comparacoes);
            printf("-----------------------------------------------------\n");
            return meio;
        }

        // Se a chave for menor, ignora a metade direita
        if (resultado_comparacao < 0) {
            inicio = meio + 1;
        }
        // Se a chave for maior, ignora a metade esquerda
        else {
            fim = meio - 1;
        }
    }

    // Se a execução chegar aqui, o elemento não está presente
    printf("[ CHAVE NAO ENCONTRADA!] Item nao localizado. Comparacoes: %d\n", comparacoes);
    printf("-----------------------------------------------------\n");
    return -1;
}

// --- MENU PRINCIPAL E LÓGICA DO JOGO ---

/**
 * @brief Exibe o menu interativo e processa a escolha do jogador.
 */
void menuPrincipal() {
    Componente componentesOriginais[MAX_COMPONENTES];
    Componente componentesEmUso[MAX_COMPONENTES]; // Cópia para ordenação
    int numComponentes = 0;
    int opcao;
    int ordenadoPorNome = 0; // Flag para a pré-condição da Busca Binária

    // Inicializa com dados de teste para facilitar
    inicializarDados(componentesOriginais, &numComponentes);

    do {
        printf("\n\n===============  MODULO DE ORGANIZACAO DA TORRE  ===============\n");
        printf("               Escolha sua estrategia de montagem:\n");
        printf("------------------------------------------------------------------\n");
        printf("1. Visualizar Componentes Atuais (%d Itens)\n", numComponentes);
        printf("2. (Estrategia Rapida) Ordenar por NOME (Bubble Sort) e Buscar CHAVE\n");
        printf("3. (Estrategia Tipologica) Ordenar por TIPO (Insertion Sort)\n");
        printf("4. (Estrategia Critica) Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("5. Buscar Componente-Chave (Requer ordenacao por NOME!)\n");
        printf("6. Resetar para a Ordem Original\n");
        printf("0. Sair e Abortar Missao\n");
        printf("------------------------------------------------------------------\n");
        printf("Escolha sua opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1; // Opcao invalida
        } else {
            limparBuffer();
        }

        // Se houver componentes, copia-os para a versão 'em uso' antes de qualquer operação
        if (numComponentes > 0 && opcao != 6 && opcao != 0) {
            copiarComponentes(componentesOriginais, componentesEmUso, numComponentes);
        }

        switch (opcao) {
            case 1:
                mostrarComponentes(componentesOriginais, numComponentes);
                break;

            case 2: {
                long long comparacoes;
                bubbleSortNome(componentesEmUso, numComponentes, &comparacoes);
                mostrarComponentes(componentesEmUso, numComponentes);
                ordenadoPorNome = 1;

                // Busca após a ordenação
                printf("\n>>> FASE DE BUSCA IMEDIATA APÓS ORDENAÇÃO POR NOME <<<\n");
                buscaBinariaPorNome(componentesEmUso, numComponentes, COMPONENTE_CHAVE);
                break;
            }

            case 3: {
                long long comparacoes;
                insertionSortTipo(componentesEmUso, numComponentes, &comparacoes);
                mostrarComponentes(componentesEmUso, numComponentes);
                ordenadoPorNome = 0; // Invalida a busca por nome
                break;
            }

            case 4: {
                long long comparacoes;
                selectionSortPrioridade(componentesEmUso, numComponentes, &comparacoes);
                mostrarComponentes(componentesEmUso, numComponentes);
                ordenadoPorNome = 0; // Invalida a busca por nome
                break;
            }

            case 5:
                if (ordenadoPorNome && numComponentes > 0) {
                    buscaBinariaPorNome(componentesEmUso, numComponentes, COMPONENTE_CHAVE);
                } else if (numComponentes == 0) {
                    printf("\n[ ERRO] Nao ha componentes para buscar.\n");
                } else {
                    printf("\n[ ALERTA] A busca binaria so funciona apos a ordenacao por NOME (Opcao 2)!\n");
                    printf("A ordem atual e: ");
                    mostrarComponentes(componentesEmUso, numComponentes);
                }
                break;

            case 6: // Reset
                if (numComponentes > 0) {
                    copiarComponentes(componentesOriginais, componentesEmUso, numComponentes);
                    printf("\n[ Dados resetados para a ordem original.]\n");
                    mostrarComponentes(componentesEmUso, numComponentes);
                } else {
                    printf("\n[ Nenhum dado para resetar.]\n");
                }
                ordenadoPorNome = 0;
                break;
            
            case 0:
                printf("\n[ MISSAO ABORTADA] O jogador abandonou o desafio final!\n");
                break;

            default:
                printf("\n[ Opcao invalida. Escolha um numero de 0 a 6.]\n");
        }

    } while (opcao != 0); // fIM DAS BUSCAS.
}

// --- FUNÇÃO PRINCIPAL ---

int main() {
    menuPrincipal();
    return 0; 
}