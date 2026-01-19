#include <stdio.h>
#include <stdlib.h>
#define MAX_VENDAS 10
#include <string.h>
#include <ctype.h>
void Endereco(){
    printf("executada");
}

int main()
{
    Endereco();
    return 0;
}

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

Venda vendas[MAX_VENDAS];
int totalvendas = 0;
int proximoId = 1;

void limparbuffer(){
   int c;
   while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarVenda(){
if (totalvendas >= MAX_VENDAS){
    printf("atingiu limite de vendas ")

}

Venda novaVenda;

novaVenda.ID = proximoId++;
//comprador

printf("dados comprador:\n");
fgets(novaVenda.comprador, 100, stdin);
novaVenda.comprador[strcspn(novaVenda.comprador, "\n")]=0;

//coleta valor da venda
printf("valor final da venda:\n");
scanf("%f", &novaVenda.valor_final);
limparbuffer();
}
}