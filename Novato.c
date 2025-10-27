#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10 // capacidade máxima da mochila

// ------------------------------------------------------
// Estrutura que representa um item do inventário
// ------------------------------------------------------
typedef struct {
    char nome[30];     // nome do item (ex: "Faca, pistola", "Kit Médico")
    char tipo[20];     // tipo do item (ex: "arma", "munição", "cura")
    int quantidade;    // quantidade disponível
} Item;

// ------------------------------------------------------
// Função: inserirItem()
// Adiciona um novo item à mochila, se houver espaço
// ------------------------------------------------------
void inserirItem(Item mochila[], int *numItens) {
    if (*numItens >= MAX_ITENS) {
        printf("\nA mochila está cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novoItem;

    printf("\n--- Cadastro de Item ---\n");
    printf("Nome do item: ");
    scanf(" %[^\n]", novoItem.nome);
    printf("Tipo do item (arma, munição, cura...): ");
    scanf(" %[^\n]", novoItem.tipo);
    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);

    mochila[*numItens] = novoItem;
    (*numItens)++;

    printf("\nItem '%s' adicionado com sucesso!\n", novoItem.nome);
}

// ------------------------------------------------------
// Função: listarItens()
// Exibe todos os itens cadastrados na mochila
// ------------------------------------------------------
void listarItens(Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    printf("\n--- Itens na Mochila ---\n");
    for (int i = 0; i < numItens; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// ------------------------------------------------------
// Função: buscarItem()
// Busca um item pelo nome usando busca sequencial
// ------------------------------------------------------
void buscarItem(Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("\nA mochila está vazia. Nenhum item para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item que deseja buscar: ");
    scanf(" %[^\n]", nomeBusca);

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("\nItem '%s' não encontrado na mochila.\n", nomeBusca);
}

// ------------------------------------------------------
// Função: removerItem()
// Remove um item da mochila com base no nome informado
// ------------------------------------------------------
void removerItem(Item mochila[], int *numItens) {
    if (*numItens == 0) {
        printf("\nA mochila está vazia! Nenhum item para remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\nDigite o nome do item que deseja remover: ");
    scanf(" %[^\n]", nomeRemover);

    for (int i = 0; i < *numItens; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            // move os itens seguintes uma posição para trás
            for (int j = i; j < *numItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*numItens)--;
            printf("\nItem '%s' removido com sucesso!\n", nomeRemover);
            return;
        }
    }

    printf("\nItem '%s' não encontrado na mochila.\n", nomeRemover);
}

// ------------------------------------------------------
// Função principal (menu do sistema)
// ------------------------------------------------------
int main() {
    Item mochila[MAX_ITENS];
    int numItens = 0;
    int opcao;

    do {
        printf("\n======================================\n");
        printf("======= SISTEMA DE INVENTÁRIO =======\n");
        printf("======================================\n\n");
        printf("1. Cadastrar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItem(mochila, &numItens);
                break;
            case 2:
                removerItem(mochila, &numItens);
                break;
            case 3:
                listarItens(mochila, numItens);
                break;
            case 4:
                buscarItem(mochila, numItens);
                break;
            case 0:
                printf("\nSaindo do sistema...\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
