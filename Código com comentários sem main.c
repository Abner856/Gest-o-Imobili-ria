#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/*
O vetor vendas funciona como um "banco de dados em memória".
Todas as funções do sistema leem ou alteram esse vetor.
*/
Venda vendas[MAX_VENDAS];

/*
totalvendas controla quantas posições do vetor vendas estão ocupadas.
Ele é usado como limite em loops e também para inserção e remoção.
*/
int totalvendas = 0;

/*
proximoId garante que cada venda tenha um identificador único,
mesmo após salvar e carregar do arquivo.
*/
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

/*
Essa função é usada por várias outras funções que fazem leitura
com scanf e fgets. Ela evita que restos de caracteres no buffer
causem leituras erradas.
*/
void limparbuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
menu() apenas exibe as opções do sistema.
Ela NÃO toma decisões e NÃO chama outras funções.
Quem decide o que fazer com a opção escolhida é a função main().
*/
void menu() {
    printf("\n--- SISTEMA DE GESTAO DE LOTES ---\n");
    printf("1. Cadastrar Venda\n2. Listar Vendas\n3. Buscar por ID\n");
    printf("4. Excluir Venda\n5. Salvar no Arquivo\n6. Carregar do Arquivo\n");
    printf("7. Ver Arquivo (Bruto)\n0. Sair\nEscolha: ");
}

/*
cadastrarVenda() cria uma nova venda APENAS NA MEMÓRIA.
Ela interage diretamente com:
- vendas[]  -> para armazenar a venda
- totalvendas -> para controlar o tamanho do vetor
- proximoId -> para gerar o ID automaticamente

Ela NÃO grava no arquivo.
Para persistir os dados, salvarVendasArquivo() deve ser chamada depois.
*/
int cadastrarVenda() {

    // Evita ultrapassar o limite do vetor
    if (totalvendas >= MAX_VENDAS) {
        printf("Erro: Memoria cheia.\n");
        return 0;
    }

    // Venda temporária usada para coletar os dados
    Venda v;

    // Geração de ID único usando variável global
    v.ID = proximoId++;

    // Leitura do nome do comprador
    fgets(v.comprador, 30, stdin);
    v.comprador[strcspn(v.comprador, "\n")] = 0;

    // Leitura do valor da venda
    scanf("%f", &v.valor_final);
    limparbuffer();

    // Leitura dos dados do lote
    fgets(v.lote_t.quadra, 20, stdin);
    v.lote_t.quadra[strcspn(v.lote_t.quadra, "\n")] = 0;

    scanf("%f", &v.lote_t.dimensao);
    limparbuffer();

    // Leitura do endereço
    fgets(v.lote_t.endereco_t.rua, 20, stdin);
    v.lote_t.endereco_t.rua[strcspn(v.lote_t.endereco_t.rua, "\n")] = 0;

    scanf("%d", &v.lote_t.endereco_t.numero);
    limparbuffer();

    /*
    Validação do CEP:
    O loop só termina quando o CEP tiver exatamente 8 caracteres.
    Isso impede que dados inválidos entrem no sistema.
    */
    int cepValido = 0;
    do {
        fgets(v.lote_t.endereco_t.cep, 10, stdin);
        v.lote_t.endereco_t.cep[strcspn(v.lote_t.endereco_t.cep, "\n")] = 0;

        if (strlen(v.lote_t.endereco_t.cep) != 8) {
            printf("[ERRO] CEP invalido!\n");
        } else {
            cepValido = 1;
        }
    } while (!cepValido);

    /*
    A venda só é realmente adicionada ao sistema aqui.
    Ela é copiada para o vetor global vendas[].
    */
    vendas[totalvendas++] = v;

    return 1;
}

/*
listarVendas() apenas CONSULTA o estado atual do sistema.
Ela lê vendas[] e totalvendas, mas NÃO altera nada.
*/
void listarVendas() {

    if (totalvendas == 0) {
        printf("\nNenhuma venda cadastrada.\n");
        return;
    }

    for (int i = 0; i < totalvendas; i++) {
        printf("%02d | %s | %.2f | %s | %.2f | %s\n",
               vendas[i].ID,
               vendas[i].comprador,
               vendas[i].valor_final,
               vendas[i].lote_t.quadra,
               vendas[i].lote_t.dimensao,
               vendas[i].lote_t.endereco_t.cep);
    }
}

/*
buscarVendaPorID() percorre o vetor vendas[] procurando o ID informado.
Ela retorna o ÍNDICE da venda, não a venda em si.
Esse índice é usado por outras funções como excluirVenda().
*/
int buscarVendaPorID(int id) {
    for (int i = 0; i < totalvendas; i++) {
        if (vendas[i].ID == id)
            return i;
    }
    return -1;
}

/*
excluirVenda() depende diretamente de buscarVendaPorID().
Primeiro ela localiza a venda, depois reorganiza o vetor,
deslocando os elementos para não deixar "buracos".
*/
int excluirVenda(int id) {

    int index = buscarVendaPorID(id);

    if (index == -1) {
        printf("ID nao encontrado.\n");
        return 0;
    }

    /*
    Como não existe remoção direta em vetores,
    os elementos seguintes são movidos uma posição para trás.
    */
    for (int i = index; i < totalvendas - 1; i++) {
        vendas[i] = vendas[i + 1];
    }

    totalvendas--;

    /*
    A exclusão acontece SOMENTE NA MEMÓRIA.
    Para refletir no arquivo, salvarVendasArquivo() deve ser chamada.
    */
    printf("Venda removida com sucesso.\n");
    return 1;
}

/*
salvarVendasArquivo() grava TODO o estado atual do sistema no arquivo.
Ela lê:
- vendas[]
- totalvendas
- proximoId

Essa função transforma as estruturas em texto.
*/
int salvarVendasArquivo() {

    FILE *f = fopen(ARQUIVO_DADOS, "w");
    if (!f) return 0;

    // Salva o próximo ID para manter continuidade
    fprintf(f, "PROXIMO_ID:%d\n", proximoId);

    /*
    Cada venda vira uma linha no arquivo.
    Os campos são separados por '|'
    */
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

/*
carregarVendasArquivo() faz o processo inverso de salvarVendasArquivo().
Ela reconstrói o sistema em memória a partir do arquivo.
*/
int carregarVendasArquivo() {

    FILE *f = fopen(ARQUIVO_DADOS, "r");
    if (!f) return 0;

    totalvendas = 0;
    char linha[256];

    // Lê o próximo ID salvo
    if (fgets(linha, sizeof(linha), f)) {
        sscanf(linha, "PROXIMO_ID:%d", &proximoId);
    }

    /*
    Cada linha é quebrada em partes usando strtok.
    Cada parte corresponde a um campo da struct Venda.
    */
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
        t = strtok(NULL, "|"); if (t) strcpy(v.lote_t.endereco_t.cep, t);

        vendas[totalvendas++] = v;
    }

    fclose(f);
    return 1;
}

/*
mostrarConteudoArquivo() apenas lê o arquivo e imprime na tela.
Ela NÃO usa o vetor vendas[].
Serve para conferência e depuração.
*/
void mostrarConteudoArquivo() {

    FILE *f = fopen(ARQUIVO_DADOS, "r");
    if (!f) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    char c;
    while ((c = fgetc(f)) != EOF) {
        printf("%c", c);
    }

    fclose(f);
}
