#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_VENDAS 10
#define ARQUIVO_DADOS "./vendas.txt"

// Definição de cores para o terminal
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define RESET   "\033[0m"

typedef struct {
    char rua[20];
    int numero;
    char cep[10];
} Endereco;

typedef struct {
    char quadra[20];
    float dimensao;
    Endereco endereco_t;
} Lote;

typedef struct {
    int ID;
    char comprador[30];
    float valor_final;
    Lote lote_t;
} Venda;

Venda vendas[MAX_VENDAS];
int totalvendas = 0;
int proximoId = 1;

// ================== PROTÓTIPOS ==================
void limparbuffer();
void menu();
int cadastrarVenda();
void listarVendas();
int buscarVendaPorID(int id);
int excluirVenda(int id);
int salvarVendasArquivo();
int carregarVendasArquivo();
void mostrarConteudoArquivo();

// ================== FUNÇÕES ==================

void limparbuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu() {
    printf("\n--- SISTEMA DE GESTAO DE LOTES ---\n");
    printf("1. Cadastrar Venda\n2. Listar Vendas\n3. Buscar por ID\n");
    printf("4. Excluir Venda\n5. Salvar no Arquivo\n6. Carregar do Arquivo\n");
    printf("7. Ver Arquivo (Bruto)\n0. Sair\nEscolha: ");
}

int cadastrarVenda() {
    if (totalvendas >= MAX_VENDAS) {
        printf(RED "Erro: Memoria cheia.\n" RESET);
        return 0;
    }

    Venda v;
    v.ID = proximoId++;

    printf("Nome do comprador: ");
    fgets(v.comprador, 30, stdin);
    v.comprador[strcspn(v.comprador, "\n")] = 0;

    printf("Valor da venda: ");
    scanf("%f", &v.valor_final);
    limparbuffer();

    printf("Quadra: ");
    fgets(v.lote_t.quadra, 20, stdin);
    v.lote_t.quadra[strcspn(v.lote_t.quadra, "\n")] = 0;

    printf("Dimensao: ");
    scanf("%f", &v.lote_t.dimensao);
    limparbuffer();

    printf("Rua: ");
    fgets(v.lote_t.endereco_t.rua, 20, stdin);
    v.lote_t.endereco_t.rua[strcspn(v.lote_t.endereco_t.rua, "\n")] = 0;

    printf("Numero: ");
    scanf("%d", &v.lote_t.endereco_t.numero);
    limparbuffer();

    int cepValido = 0;
    do {
        printf("CEP (8 digitos): ");
        fgets(v.lote_t.endereco_t.cep, 10, stdin);
        v.lote_t.endereco_t.cep[strcspn(v.lote_t.endereco_t.cep, "\n")] = 0;

        if (strlen(v.lote_t.endereco_t.cep) != 8) {
            printf(RED "[ERRO] CEP invalido!\n" RESET);
        } else {
            cepValido = 1;
        }
    } while (!cepValido);

    vendas[totalvendas++] = v;
    printf(GREEN "Venda cadastrada com sucesso!\n" RESET);
    return 1;
}

void listarVendas() {
    if (totalvendas == 0) {
        printf(RED "\nNenhuma venda cadastrada.\n" RESET);
        return;
    }
    printf("\nID | Comprador | Valor | Quadra | Dim. | Rua | Num | CEP\n");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < totalvendas; i++) {
        printf("%02d | %s | %.2f | %s | %.2f | %s | %d | %s\n",
               vendas[i].ID, 
               vendas[i].comprador, 
               vendas[i].valor_final,
               vendas[i].lote_t.quadra, 
               vendas[i].lote_t.dimensao,
               vendas[i].lote_t.endereco_t.rua,
               vendas[i].lote_t.endereco_t.numero,
               vendas[i].lote_t.endereco_t.cep);
    }
}

int buscarVendaPorID(int id) {
    for (int i = 0; i < totalvendas; i++) {
        if (vendas[i].ID == id) return i;
    }
    return -1;
}

int excluirVenda(int id) {
    int index = buscarVendaPorID(id);
    if (index == -1) {
        printf(RED "ID nao encontrado.\n" RESET);
        return 0;
    }
    for (int i = index; i < totalvendas - 1; i++) {
        vendas[i] = vendas[i + 1];
    }
    totalvendas--;
    printf(GREEN "Venda removida com sucesso.\n" RESET);
    return 1;
}

// SALVA TODOS OS DADOS NO TXT
int salvarVendasArquivo() {
    FILE *f = fopen(ARQUIVO_DADOS, "w");
    if (!f) {
        printf(RED "Erro ao abrir o arquivo!\n" RESET);
        return 0;
    }

    fprintf(f, "PROXIMO_ID:%d\n", proximoId);
    for (int i = 0; i < totalvendas; i++) {
        // Garantindo que todos os campos da struct sejam escritos
        fprintf(f, "%d|%s|%.2f|%s|%.2f|%s|%d|%s\n",
                vendas[i].ID, 
                vendas[i].comprador, 
                vendas[i].valor_final,
                vendas[i].lote_t.quadra, 
                vendas[i].lote_t.dimensao,
                vendas[i].lote_t.endereco_t.rua, 
                vendas[i].lote_t.endereco_t.numero,
                vendas[i].lote_t.endereco_t.cep);
    }
    fclose(f);
    printf(GREEN "Dados salvos com sucesso no arquivo TXT!\n" RESET);
    return 1;
}

int carregarVendasArquivo() {
    FILE *f = fopen(ARQUIVO_DADOS, "r");
    if (!f) return 0;

    totalvendas = 0;
    char linha[256];
    if (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "PROXIMO_ID:%d", &proximoId);
    }

    while (fgets(linha, sizeof(linha), f) && totalvendas < MAX_VENDAS) {
        Venda v;
        char *t = strtok(linha, "|");
        if (t) v.ID = atoi(t);
        t = strtok(NULL, "|"); if (t) strcpy(v.comprador, t);
        t = strtok(NULL, "|"); if (t) v.valor_final = atof(t);
        t = strtok(NULL, "|"); if (t) strcpy(v.lote_t.quadra, t);
        t = strtok(NULL, "|"); if (t) v.lote_t.dimensao = atof(t);
        t = strtok(NULL, "|"); if (t) strcpy(v.lote_t.endereco_t.rua, t);
        t = strtok(NULL, "|"); if (t) v.lote_t.endereco_t.numero = atoi(t);
        t = strtok(NULL, "|"); if (t) {
            // Remove o \n que sobra no final da string do CEP antes de copiar
            t[strcspn(t, "\n")] = 0;
            strcpy(v.lote_t.endereco_t.cep, t);
        }
        vendas[totalvendas++] = v;
    }
    fclose(f);
    return 1;
}

void mostrarConteudoArquivo() {
    FILE *f = fopen(ARQUIVO_DADOS, "r");
    if (!f) {
        printf(RED "Arquivo nao encontrado.\n" RESET);
        return;
    }
    printf("\n--- CONTEUDO BRUTO DO ARQUIVO ---\n");
    char c;
    while ((c = fgetc(f)) != EOF) printf("%c", c);
    printf("---------------------------------\n");
    fclose(f);
}

int main() {
    setlocale(LC_ALL, "Portuguese"); 

    int opcao, id_busca, pos;
    carregarVendasArquivo();

    do {
        menu();
        if (scanf("%d", &opcao) != 1) {
            limparbuffer();
            continue;
        }
        limparbuffer();

        switch (opcao) {
            case 1: cadastrarVenda(); break;
            case 2: listarVendas(); break;
            case 3:
                printf("ID para busca: ");
                scanf("%d", &id_busca);
                pos = buscarVendaPorID(id_busca);
                if (pos != -1) {
                    printf(GREEN "Venda Encontrada!\n" RESET);
                    printf("Comprador: %s | Valor: %.2f | Rua: %s, %d\n", 
                            vendas[pos].comprador, vendas[pos].valor_final, 
                            vendas[pos].lote_t.endereco_t.rua, vendas[pos].lote_t.endereco_t.numero);
                }
                else printf(RED "Nao encontrado.\n" RESET);
                break;
            case 4:
                printf("ID para excluir: ");
                scanf("%d", &id_busca);
                excluirVenda(id_busca);
                break;
            case 5: salvarVendasArquivo(); break;
            case 6: carregarVendasArquivo(); printf(GREEN "Dados recarregados!\n" RESET); break;
            case 7: mostrarConteudoArquivo(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf(RED "Invalido!\n" RESET);
        }
    } while (opcao != 0);

    return 0;
}
