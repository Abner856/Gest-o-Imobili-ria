#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "endereco.h"

// variáveis globais
Venda vendas[MAX_VENDAS];
int totalvendas = 0;
int proximoId = 1;

void menu() {
    printf("\n========================================\n");
    printf("       SISTEMA DE VENDAS DE LOTES      \n");
    printf("========================================\n");
    printf("1. Cadastrar nova venda\n");
    printf("2. Listar todas as vendas\n");
    printf("3. Buscar venda por ID\n");
    printf("4. Atualizar venda\n");
    printf("5. Excluir venda\n");
    printf("6. Estatísticas\n");
    printf("7. Salvar dados em arquivo\n");
    printf("8. Carregar dados do arquivo\n");
    printf("9. Mostrar conteúdo do arquivo\n");
    printf("0. Sair\n");
    printf("========================================\n");
    printf("Escolha uma opção: ");

}

void FormatarparaArquivo(Venda varquivo, char* linha, int tamanho){

    snprintf(linha, tamanho, "%d|%s|%.2f|%s|%.2f|%s|%d|%s",
             varquivo.ID,
             varquivo.comprador,
             varquivo.valor_final,
             varquivo.lote_t.quadra,
             varquivo.lote_t.dimensao,
             varquivo.lote_t.endereco_t.rua,
             varquivo.lote_t.endereco_t.numero,
             varquivo.lote_t.endereco_t.cep);

}

int SalvarVendasArquivo(){
    FILE *arquivo = fopen(ARQUIVOS_DADOS, "w");
    if(arquivo == NULL){
        printf("ERRO: Não foi possível criar o arquivo %s!\n", ARQUIVO_DADOS);
        return 0;
       }
}

fprintf(arquivo, "PROXIMO_ID: %d\n", proximoId);

for (int i = 0; i < totalVendas; i++) {
        char linha[200];
        FormatarParaArquivo(vendas[i], linha, sizeof(linha));
        fprintf(arquivo, "%s\n", linha);
}

fclose(arquivo);
printf("Dados salvos com sucesso em '%s' (%d vendas)\n", ARQUIVO_DADOS, totalVendas);
    return 1;
}

int carregarVendasArquivo() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "r");
    if (arquivo == NULL) {
        printf("Arquivo '%s' não encontrado. Criando novo...\n", ARQUIVO_DADOS);
        return 0;
    }

    // Limpa os dados atuais
    totalvendas = 0;

    char linha[200];

if (fgets(linha, sizeof(linha), arquivo)) {
        if (strstr(linha, "PROXIMO_ID:") != NULL) {
            sscanf(linha, "PROXIMO_ID:%d", &proximoId);
        } else {
            // Se não encontrar NEXT_ID, volta ao início
            rewind(arquivo);
        }
    }
//lendo as vendas
while (fgets(linha, sizeof(linha), arquivo) && totalvendas < MAX_VENDAS) {
        linha[strcspn(linha, "\n")] = 0; // Remove newline
        Venda varquivo;
        char *token;
        int campo = 0;

        // Usa strtok para separar as strings em pedaços menores(tokens) usando o delimitador '|'
        // atoi converte uma string em um valor inteiro
        token = strtok(linha, "|");
        while (token != NULL && campo < 8) {
            switch(campo) {
                case 0: v.ID = atoi(token); break;
                case 1: strcpy(v.comprador, token); break;
                case 2: v.valor_final = atof(token); break;
                case 3: strcpy(v.lote_t.quadra, token); break;
                case 4: v.lote_t.dimensao = atof(token); break;
                case 5: strcpy(v.lote_t.endereco_t.rua, token); break;
                case 6: v.lote_t.endereco_t.numero = atoi(token); break;
                case 7: strcpy(v.lote_t.endereco_t.cep, token); break;
            }
            campo++;
            token = strtok(NULL, "|");
        }
    if (campo == 8) { // Todos os campos foram lidos
            vendas[totalvendas] = v;
            totalvendas++;
        }
    }

    fclose(arquivo);
    printf("Dados carregados com sucesso de '%s' (%d vendas)\n", ARQUIVO_DADOS, totalvendas);
    return 1;
}
    void mostrarConteudoArquivo() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "r");
    if (arquivo == NULL) {
        printf("Arquivo '%s' não encontrado!\n", ARQUIVO_DADOS);
        return;
    }

    printf("\n ===CONTEÚDO DO ARQUIVO '%s' ===\n", ARQUIVO_DADOS);
    printf("========================================\n");

    char linha[200];
    int linhaNum = 1;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0; // Remove newline
        printf("%03d: %s\n", linhaNum++, linha);
    }

    fclose(arquivo);

    // Estatísticas do arquivo
    arquivo = fopen(ARQUIVO_DADOS, "r");
    if (arquivo) {
        int totallinhas = 0;
        while (fgets(linha, sizeof(linha), arquivo)) {
            totalLinhas++;
        }
        fclose(arquivo);
        printf("Estatísticas do arquivo:\n");
        printf("   Total de linhas: %d\n", totallinhas);
        printf("   Tamanho aproximado: %ld bytes\n", totallinhas * 100);
    }
}
