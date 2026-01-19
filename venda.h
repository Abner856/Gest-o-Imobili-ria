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

//codigo imcompleto
