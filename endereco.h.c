#ifndef ENDERECO_H
#define ENDERECO_H
#define MAX_VENDAS 10
#define ARQUIVO_DADOS "vendas.txt"
#define ARQUIVO_DADOS ARQUIVOS_DADOS
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

//variaveis

void limparbuffer();
int cadastrarVenda();
void listarVendas();
int buscarVendaPorID(int id);
void exibirVenda(int index);
int atualizarVenda(int id);
int excluirVenda(int id);
void menu();
int salvarVendasArquivo();
int carregarVendasArquivo();
void mostrarConteudoArquivo();


#endif

