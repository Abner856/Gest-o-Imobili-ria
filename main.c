#ifndef ENDERECO_H
#define ENDERECO_H
#define MAX_VENDAS 10
//preencher dados do terreno
typedef struct{
    char rua[20];
    int numero;
    char cep[11];
}Endereco;

typedef struct{
    char quadra[20];
    float dimensao;
    Endereco endereco_t;
}Lote;

typedef struct{
    int ID;
    char comprador[30];
    float valor_final;

    Lote lote_t;

}Venda;
//declaraçao das funcoes
extern Venda vendas[MAX_VENDAS];
extern int totalvendas;
extern int proximoId;

void limparbuffer();
void cadastrarVenda();

#endif
