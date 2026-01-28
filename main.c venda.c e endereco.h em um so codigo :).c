#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINIÇÕES E ESTRUTURAS

#define MAX_VENDAS 10
#define ARQUIVO_DADOS "./vendas.txt"

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

// Protótipos
void limparbuffer();
void menu();
int cadastrarVenda();
void listarVendas();
int buscarVendaPorID(int id);
int excluirVenda(int id);
int salvarVendasArquivo();
int carregarVendasArquivo();
void mostrarConteudoArquivo();

// IMPLEMENTAÇÃO DAS FUNÇÕES

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
        printf("Erro: Memoria cheia.\n");
        return 0;
    }
    Venda v;
    v.ID = proximoId++;

    printf("Comprador: "); 
    fgets(v.comprador, 30, stdin);
    v.comprador[strcspn(v.comprador, "\n")] = 0;

    printf("Valor Final: "); 
    scanf("%f", &v.valor_final); 
    limparbuffer();

    printf("Quadra: "); 
    fgets(v.lote_t.quadra, 20, stdin);
    v.lote_t.quadra[strcspn(v.lote_t.quadra, "\n")] = 0;

    printf("Dimensao (m2): "); 
    scanf("%f", &v.lote_t.dimensao); 
    limparbuffer();

    printf("Rua: "); 
    fgets(v.lote_t.endereco_t.rua, 20, stdin);
    v.lote_t.endereco_t.rua[strcspn(v.lote_t.endereco_t.rua, "\n")] = 0;

    printf("Numero: "); 
    scanf("%d", &v.lote_t.endereco_t.numero); 
    limparbuffer();

    // LOOP DE VALIDAÇÃO DO CEP
    int cepValido = 0;
    do {
        printf("CEP (apenas 8 numeros): "); 
        fgets(v.lote_t.endereco_t.cep, 10, stdin);
        v.lote_t.endereco_t.cep[strcspn(v.lote_t.endereco_t.cep, "\n")] = 0;

        // Verifica o tamanho usando IF
        if (strlen(v.lote_t.endereco_t.cep) != 8) {
            printf("[ERRO] CEP invalido! Digite exatamente 8 caracteres.\n");
        } else {
            cepValido = 1;
        }
    } while (!cepValido);
    
    vendas[totalvendas++] = v;
    return 1;
}

void listarVendas() {
    if (totalvendas == 0) { 
        printf("\nNenhuma venda cadastrada.\n"); 
        return; 
    }
    printf("\nID | Comprador      | Valor      | Quadra | Dimensao | CEP\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < totalvendas; i++) {
        printf("%02d | %-14s | R$%-8.2f | %-6s | %-8.2f | %s\n",
               vendas[i].ID, 
               vendas[i].comprador, 
               vendas[i].valor_final, 
               vendas[i].lote_t.quadra, 
               vendas[i].lote_t.dimensao,
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
    if (index == -1) { printf("ID nao encontrado.\n"); return 0; }

    for (int i = index; i < totalvendas - 1; i++) {
        vendas[i] = vendas[i + 1];
    }
    totalvendas--;
    printf("Venda removida com sucesso.\n");
    return 1;
}

int salvarVendasArquivo() {
    FILE *f = fopen(ARQUIVO_DADOS, "w");
    if (!f) return 0;
    fprintf(f, "PROXIMO_ID:%d\n", proximoId);
    for (int i = 0; i < totalvendas; i++) {
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
    return 1;
}

int carregarVendasArquivo() {
    FILE *f = fopen(ARQUIVO_DADOS, "r");
    if (!f) return 0;
    totalvendas = 0;
    char linha[256];
    if (fgets(linha, sizeof(linha), f)) sscanf(linha, "PROXIMO_ID:%d", &proximoId);
    //usamos o strtok para separar as strings em pedaços(tokens) modificando o delimitador por "\0"
    //e o atoi que converte uma string em int
    while (fgets(linha, sizeof(linha), f) && totalvendas < MAX_VENDAS) {
        Venda v;
        char *t = strtok(linha, "|");
        if(t) v.ID = atoi(t);
        t = strtok(NULL, "|"); if(t) strcpy(v.comprador, t);
        t = strtok(NULL, "|"); if(t) v.valor_final = (float)atof(t);
        t = strtok(NULL, "|"); if(t) strcpy(v.lote_t.quadra, t);
        t = strtok(NULL, "|"); if(t) v.lote_t.dimensao = (float)atof(t);
        t = strtok(NULL, "|"); if(t) strcpy(v.lote_t.endereco_t.rua, t);
        t = strtok(NULL, "|"); if(t) v.lote_t.endereco_t.numero = atoi(t);
        t = strtok(NULL, "|"); if(t) strcpy(v.lote_t.endereco_t.cep, t);
        vendas[totalvendas++] = v;
    }
    fclose(f);
    return 1;
}

void mostrarConteudoArquivo() {
    FILE *f = fopen(ARQUIVO_DADOS, "r");
    if (!f) { printf("Arquivo nao encontrado.\n"); return; }
    char c;
    while ((c = fgetc(f)) != EOF) printf("%c", c);
    fclose(f);
}

int main() {
    int opcao, id_busca;

    if (carregarVendasArquivo()) {
        printf("Sistema: Dados carregados com sucesso!\n");
    } else {
        printf("Sistema: Nenhum arquivo de dados encontrado. Iniciando novo banco.\n");
    }

    do {
        menu();
        if (scanf("%d", &opcao) != 1) {
            printf("Erro: Digite apenas numeros.\n");
            limparbuffer();
            continue;
        }
        limparbuffer();

        switch (opcao) {
            case 1:
                if (cadastrarVenda()) {
                    printf("Sucesso: Venda registrada na memoria.\n");
                }
                break;

            case 2:
                listarVendas();
                break;

            case 3: {
                printf("Digite o ID para buscar: ");
                scanf("%d", &id_busca);
                limparbuffer();
                //index usada para acessar indices em listas
                int index = buscarVendaPorID(id_busca);
                if (index != -1) {
                    printf("\n--- Detalhes da Venda ---\n");
                    printf("Comprador: %s\nValor: R$ %.2f\nQuadra: %s\nDimensao: %.2f m2\nCEP: %s\n",
                            vendas[index].comprador, 
                            vendas[index].valor_final,
                            vendas[index].lote_t.quadra,
                            vendas[index].lote_t.dimensao,
                            vendas[index].lote_t.endereco_t.cep);
                } else {
                    printf("Erro: Venda com ID %d nao encontrada.\n", id_busca);
                }
                break;
            }

            case 4: {
                printf("Digite o ID para excluir: ");
                scanf("%d", &id_busca);
                limparbuffer();
                excluirVenda(id_busca);
                break;
            }

            case 5:
                if (salvarVendasArquivo()) {
                    printf("Sucesso: Arquivo atualizado!\n");
                } else {
                    printf("Erro ao salvar arquivo.\n");
                }
                break;

            case 6:
                if (carregarVendasArquivo()) {
                    printf("Dados recarregados do arquivo.\n");
                }
                break;

            case 7:
                printf("\n--- Conteudo Bruto do Arquivo ---\n");
                mostrarConteudoArquivo();
                break;

            case 0: {
                printf("Deseja salvar antes de sair? (1-Sim / 0-Nao): ");
                int salvar;
                if(scanf("%d", &salvar) == 1 && salvar == 1) {
                    salvarVendasArquivo();
                }
                printf("Finalizando programa...\n");
                break;
            }

            default:
                printf("Opcao invalida!\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
