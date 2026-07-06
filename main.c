#include <stdio.h>
#include <string.h>
#include <time.h>


#define ARQ_PROD "RegistroProdutos.txt"
#define ARQ_VENDA "RegistroVendas.txt"
#define ARQ_CLIENTE "RegistroClientes.txt"

typedef struct Reg_Produto {
    int ID_Produto;
    char Nome_Produto[50];
    float valor_Produto;
} Produto;

typedef struct Reg_Cliente {
    char CPF_Cliente[12]; // Coloquei 12 pois o CPF tem 11 números + 1 caractere invisível de fim de texto ('\0')
    char Nome_Cliente[50];
} Cliente;

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct Reg_Venda {
    Data dataVenda;
    char CPF_Cliente[12]; // Coloquei 12 pois o CPF tem 11 números + 1 caractere invisível de fim de texto ('\0')
    char Nome_Produto[50];
    int quantVendida;
    float valorTotal;
} Reg_Venda;

void RegistroCliente() {
    //FUNÇÃO PARA CRIAR REGISTRO DO CLIENTE

    FILE *file = fopen(ARQ_CLIENTE, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    Cliente auxiliar;

    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", auxiliar.Nome_Cliente);

    printf("Digite o CPF do cliente: ");
    scanf("%s", auxiliar.CPF_Cliente);


    fprintf(file, "%s,%s\n", auxiliar.Nome_Cliente, auxiliar.CPF_Cliente);

    fclose(file);
    printf("Registro criado com sucesso!\n");
}

void RegistroProduto() {
    //FUNÇÃO PARA CRIAR REGISTRO DO PRODUTO

    FILE *file = fopen(ARQ_PROD, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    int ultimoID = 0;
    Produto auxiliar;

    //DEFINIR ID DO PRODUTO BASEADO NO ID DO ÚLTIMO PRODUTO ADICIONADO NO ARQUIVO
    FILE *file2 = fopen(ARQ_PROD, "r");
    if (file2 != NULL) {
        while (fscanf(file2, "%d,%[^,],%f\n", &auxiliar.ID_Produto, auxiliar.Nome_Produto, &auxiliar.valor_Produto) == 3) {
            ultimoID = auxiliar.ID_Produto; // Vai substituindo pelo ID da linha atual
        }
        fclose(file2);
    }

    printf("Digite o nome do produto: ");
    scanf(" %[^\n]", auxiliar.Nome_Produto);

    printf("Digite o valor do produto: ");
    scanf("%f", &auxiliar.valor_Produto);
    auxiliar.ID_Produto = ultimoID + 1;

    fprintf(file, "%d,%s,%.2f\n", auxiliar.ID_Produto, auxiliar.Nome_Produto, auxiliar.valor_Produto);

    fclose(file);
    printf("Registro criado com sucesso!\n");
}

void ListarProdutos() {
    // 1. Abre o arquivo em modo de leitura ("r" = read)
    FILE *file = fopen("RegistroProdutos.txt", "r");
    if (file == NULL) {
        printf("Nenhum produto cadastrado ou erro ao abrir o arquivo!\n");
        return;
    }

    // 2. Variáveis temporárias para guardar os dados de cada linha
    int id;
    char nome[50];
    float valor;

    printf("\n--- LISTA DE PRODUTOS ---\n");
    printf("ID | NOME\n");
    printf("-------------------------\n");

    // 3. O laço while continua lendo enquanto conseguir capturar os 3 itens da linha
    // O "%[^, ]" significa: "leia tudo até encontrar uma vírgula"
    while (fscanf(file, "%d,%[^,],%f\n", &id, nome, &valor) == 3) {

        // 4. O printf só exibe o que você quer! Ignoramos a variável 'valor' aqui.
        // O %02d formata o ID para sempre ter dois dígitos (ex: 01, 02, 03)
        printf("%02d | %s\n", id, nome);

    }

    fclose(file);
    printf("-------------------------\n");
}

// Função que verifica se o CPF existe
int VerificarClienteExiste(char *cpfBuscado) {
    if (strlen(cpfBuscado) < 11) {
        printf("Erro: O CPF digitado e invalido. Ele deve conter exatamente 11 numeros!\n");
        return 0;
    } // CPF inválido

    FILE *file = fopen(ARQ_CLIENTE, "r");
    if (file == NULL) return 0; // Arquivo ainda não existe, então o cliente não está lá

    char nomeTemp[50];
    char cpfTemp[12];

    // O seu RegistroCliente salva no formato: Nome_Cliente.CPF_Cliente
    while (fscanf(file, "%[^,],%s\n", nomeTemp, cpfTemp) == 2) {
        // strcmp retorna 0 quando as duas palavras são EXATAMENTE iguais
        if (strcmp(cpfTemp, cpfBuscado) == 0) {
            fclose(file);
            return 1; // Encontrou! Retorna 1 (Verdadeiro)
        }
    }

    fclose(file);
    return 0; // Chegou ao fim do arquivo e não encontrou
}

// Função que verifica se o Produto existe
int VerificarProdutoExiste(char *nomeBuscado) {
    FILE *file = fopen(ARQ_PROD, "r");
    if (file == NULL) return 0;

    int idTemp;
    char nomeTemp[50];
    float valorTemp;

    // O seu RegistroProduto salva no formato: ID_Produto, Nome_Produto, valor_Produto
    while (fscanf(file, "%d,%[^,],%f\n", &idTemp,nomeTemp,&valorTemp) == 3) {
        if (strcmp(nomeTemp, nomeBuscado) == 0) {
            fclose(file);
            return 1; // Encontrou! Retorna 1 (Verdadeiro)
        }
    }

    fclose(file);
    return 0; // Chegou ao fim do arquivo e não encontrou
}

// Função que retorna o valor do produto
float ObterValorProduto(char *nomeBuscado) {
    FILE *file = fopen(ARQ_PROD, "r");
    if (file == NULL) {
        printf("Erro: Arquivo de produtos nao encontrado!\n");
        return -1.0; // Retorna -1 para indicar que houve um erro
    }

    int idTemp;
    char nomeTemp[50];
    float valorTemp;

    // Varre o arquivo linha por linha
    while (fscanf(file, "%d,%[^,],%f\n", &idTemp, nomeTemp, &valorTemp) == 3) {
        // Se encontrar o nome exato
        if (strcmp(nomeTemp, nomeBuscado) == 0) {
            fclose(file);
            return valorTemp; // Encontrou! Retorna o valor do produto imediatamente
        }
    }

    fclose(file);
    return -1.0; // Chegou ao fim do arquivo e nao encontrou o produto
}

void RegistroVenda() {
    FILE *file = fopen(ARQ_VENDA, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    Reg_Venda VendaAux;

    printf("Digite o CPF do cliente: ");
    scanf(" %[^\n]", VendaAux.CPF_Cliente);
    int existeSimNao = VerificarClienteExiste(VendaAux.CPF_Cliente);
    if (existeSimNao != 1) {
        return;
    };

    printf("Digite o nome do produto do pedido: ");
    scanf(" %[^\n]", VendaAux.Nome_Produto);
    existeSimNao = VerificarProdutoExiste(VendaAux.Nome_Produto);
    if (existeSimNao != 1) {
        return;
    };

    float valorProduto = ObterValorProduto(VendaAux.Nome_Produto);

    printf("Quantas unidades desse produto foram vendidas?");
    scanf("%i", &VendaAux.quantVendida);
    VendaAux.valorTotal = VendaAux.quantVendida * valorProduto;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    VendaAux.dataVenda.dia = tm.tm_mday;
    VendaAux.dataVenda.mes = tm.tm_mon + 1;
    VendaAux.dataVenda.ano = tm.tm_year + 1900;

    fprintf(file, "%02d/%02d/%d,%s,%s,%i,%f\n",VendaAux.dataVenda.dia,VendaAux.dataVenda.mes,VendaAux.dataVenda.ano,
        VendaAux.CPF_Cliente, VendaAux.Nome_Produto, VendaAux.quantVendida, VendaAux.valorTotal);

    fclose(file);
    printf("Registro criado com sucesso!\n");

}

int main() {
    int aux = 10;
    while (aux != 0) {
        printf("\n========================================\n"
           "        MENU DO SISTEMA DE VENDAS       \n"
           "========================================\n"
           "1. Registrar Produto\n"
           "2. Registrar Cliente\n"
           "3. Registrar Nova Venda (Pedido)\n"
           "4. Registrar Devolucao de Item\n"
           "5. Gerar Relatorio Diario\n"
           "6. Gerar Relatorio Mensal\n"
           "7. Gerar Relatorio Anual\n"
           "0. Sair do Programa\n"
           "========================================\n"
           "Escolha uma opcao:  ");
    scanf("%d", &aux);

        switch (aux) {
            case 1:
                RegistroProduto();
                break;
            case 2:
                RegistroCliente();
                break;
            case 3:
                ListarProdutos();
                break;
            case 4:
                RegistroVenda();
                break;
        }
    }

    return 0;
}
