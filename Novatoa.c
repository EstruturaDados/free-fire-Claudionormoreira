 // Bibliotecas necessárias
#include <stdio.h>   // Para funções de entrada/saída (printf, scanf, fgets)
#include <stdlib.h>  // Para funções gerais (exit)
#include <string.h>  // Para manipulação de strings (strcpy, strcmp)

 // --- Definição da Struct ---
 /* Struct que representa um item dentro do inventário (mochila).
 Armazena informações essenciais de cada objeto coletado.
 */
typedef struct {
    char nome[30];       // Nome do item (ex: Faca, Bala .45, Kit Médico)
    char tipo[20];       // Tipo do item (ex: arma, munição, cura, ferramenta)
    int quantidade;      // Número de unidades deste item
} Item;

 // --- Variáveis Globais (Inventário/Mochila) ---
#define MAX_ITENS 10
Item mochila[MAX_ITENS]; // Vetor de structs para armazenar até 10 itens
int totalItens = 0;      // Contador que armazena o número atual de itens na mochila

 // --- Protótipos das Funções ---
void listarItens();
void inserirItem();
void removerItem();
void buscarItem();
void exibirMenu();

 // --- Implementação das Funções ---

 // Lista todos os itens atualmente registrados na mochila com seus dados.
void listarItens() {
    printf("\n--- Conteúdo Atual da Mochila (Inventário) ---\n");
    if (totalItens == 0) {
        printf("A mochila está vazia. Colete alguns itens!\n");
        return;
    }
    printf("====MOCHILA OPERACIONAL DE COMBATE NA SELVA====");
    printf("ID | Nome                   | Tipo       | Quantidade\n");
    printf("---|------------------------|------------|------------\n");

    // Percorre o vetor de itens
    for (int i = 0; i < totalItens; i++) {
        printf("%-2d | %-28s | %-20s | %d\n", 
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
    printf("-------------------------------------------------------------------\n");
}

 /*Permite ao jogador cadastrar um novo item na mochila.
 Verifica se a mochila está cheia antes de inserir o item.
 */
void inserirItem() {
    if (totalItens >= MAX_ITENS) {
        printf("\n[ALERTA] A mochila está cheia! Não é possível carregar mais itens (%d/%d).\n", 
               MAX_ITENS, MAX_ITENS);
        return;
    }

    printf("\n--- Cadastro de Novo Item ---\n");
    printf("Digite o nome do item (máx 29 caracteres): ");
    
    // Limpa o buffer de entrada para evitar problemas com fgets após scanf
    // Embora tecnicamente não seja necessário se o último for scanf para inteiro
    // Vamos garantir a limpeza se tivermos usado scanf antes para algo diferente
    // Mas usaremos scanf("%s", ...) para ler strings simples e evitar a complexidade do fgets/buffer
    
    // Leitura do nome
    if (scanf("%29s", mochila[totalItens].nome) != 1) {
        printf("Erro na leitura do nome.\n");
        return;
    }
    
    // Leitura do tipo
    printf("Digite o tipo do item (ex: arma, munição, cura): ");
    if (scanf("%19s", mochila[totalItens].tipo) != 1) {
        printf("Erro na leitura do tipo.\n");
        return;
    }

    // Leitura da quantidade
    printf("Digite a quantidade: ");
    if (scanf("%d", &mochila[totalItens].quantidade) != 1 || mochila[totalItens].quantidade <= 0) {
        printf("[ERRO] Quantidade inválida. O item não será adicionado.\n");
        // Não incrementa totalItens, efetivamente "cancelando" a inserção
        return;
    }

    // Incrementa o contador de itens
    totalItens++; 
    printf("\n[SUCESSO] Item '%s' adicionado à mochila. Capacidade: %d/%d.\n", 
           mochila[totalItens - 1].nome, totalItens, MAX_ITENS);
    
    // Requisito funcional: Listar itens após cada operação
    listarItens();
}

 /*
  Permite que o jogador remova um item da mochila pelo nome.
  Usa busca sequencial para encontrar o item e reorganiza o vetor após a remoção.
 */
void removerItem() {
    if (totalItens == 0) {
        printf("\n[ALERTA] A mochila está vazia. Nada para remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- Remoção de Item ---\n");
    printf("Digite o nome do item a ser removido: ");
    
    if (scanf("%29s", nomeBusca) != 1) {
        printf("Erro na leitura do nome.\n");
        return;
    }

    // Busca sequencial para encontrar o item
    int indice = -1;
    for (int i = 0; i < totalItens; i++) {
        // strcmp retorna 0 se as strings forem iguais
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            indice = i; // Encontrou o item
            break;
        }
    }

    if (indice != -1) {
        // Item encontrado, agora remove e move os itens subsequentes
        
        // Laço para mover todos os itens após o item removido uma posição para trás
        for (int j = indice; j < totalItens - 1; j++) {
            mochila[j] = mochila[j + 1]; // Copia o próximo item para a posição atual
        }

        // Decrementa o contador de itens
        totalItens--; 
        printf("\n[SUCESSO] Item '%s' removido da mochila.\n", nomeBusca);
    } else {
        printf("\n[ERRO] Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
    
    // Requisito funcional: Listar itens após cada operação
    listarItens();
}

 //Implementa uma busca sequencial para localizar e exibir os dados de um item.
 
void buscarItem() {
    if (totalItens == 0) {
        printf("\n[ALERTA] A mochila está vazia. Nada para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- Busca de Item ---\n");
    printf("Digite o nome do item a ser buscado: ");
    
    if (scanf("%29s", nomeBusca) != 1) {
        printf("Erro na leitura do nome.\n");
        return;
    }

    // Busca sequencial (Requisito funcional)
    int encontrado = 0;
    for (int i = 0; i < totalItens; i++) {
        // strcmp retorna 0 se as strings forem iguais
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            // Item encontrado, exibe os dados
            printf("\n--- Item Encontrado ---\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            printf("------------------------\n");
            encontrado = 1;
            break; // Sai do laço após encontrar o primeiro item com o nome
        }
    }

    if (!encontrado) {
        printf("\n[RESULTADO] Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
}

  //Exibe o menu principal para interação do jogador.
 
void exibirMenu() {
    printf("\n============================================\n");
    printf("         SISTEMA DE INVENTÁRIO (MOCHILA)    \n");
    printf("============================================\n");
    printf("1. Coletar Item (Cadastrar)\n");
    printf("2. Descartar Item (Remover)\n");
    printf("3. Listar Todos os Itens\n");
    printf("4. Buscar Item por Nome\n");
    printf("0. Sair do Jogo\n");
    printf("--------------------------------------------\n");
    printf("Escolha sua ação: ");
}

  //Função principal do programa. Controla o fluxo de execução.
 
int main() {
    int opcao;

    // Mensagem de boas-vindas
    printf("Bem-vindo ao Nível 1: Organização do Inventário!\n");
    printf("Sua missão é gerenciar os itens essenciais da sua mochila.\n");

    // Loop principal do jogo/sistema
    do {
        exibirMenu();
        
        // Leitura da opção
        if (scanf("%d", &opcao) != 1) {
             // Tratamento de erro básico para entrada não numérica
            printf("\n[ERRO] Opção inválida. Digite um número de 0 a 4.\n");
            // Limpa o buffer para evitar loop infinito
            while (getchar() != '\n');
            continue;
        }

        // Fluxo de execução (switch/case)
        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItem();
                break;
            case 0:
                printf("\nSaindo do jogo. Inventário salvo!\n");
                break;
            default:
                printf("\n[ALERTA] Opção desconhecida. Tente novamente.\n");
        }
        
    } while (opcao != 0);

    return 0; // Fim do programa
}