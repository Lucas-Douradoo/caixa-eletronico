#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SENHA_CORRETA "1234"
#define ARQUIVO_HISTORICO "historico.txt"
#define ARQUIVO_SALDO "saldo.txt"

// Função para obter data/hora formatada
void pegarDataHora(char *buffer, int tamanho) {
    time_t agora;
    struct tm *infoTempo;

    time(&agora);
    infoTempo = localtime(&agora);

    strftime(buffer, tamanho, "%d/%m/%Y %H:%M:%S", infoTempo);
}

// Função para mostrar o menu
void mostrarMenu() {
    printf("\n======= CAIXA ELETRONICO =======\n");
    printf("1. Ver Saldo\n");
    printf("2. Sacar Dinheiro\n");
    printf("3. Depositar Dinheiro\n");
    printf("4. Fazer PIX\n");
    printf("5. Ver Histórico\n");
    printf("6. Sair\n");
    printf("Escolha uma opcao: ");
}

// Função para registrar no histórico com data/hora
void registrarHistorico(char* mensagem) {
    FILE *arquivo = fopen(ARQUIVO_HISTORICO, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de historico.\n");
        return;
    }
    char dataHora[30];
    pegarDataHora(dataHora, sizeof(dataHora));
    fprintf(arquivo, "[%s] %s\n", dataHora, mensagem);
    fclose(arquivo);
}

// Exibir histórico
void mostrarHistorico() {
    FILE *arquivo = fopen(ARQUIVO_HISTORICO, "r");
    char linha[300];

    if (arquivo == NULL) {
        printf("Nenhum historico encontrado.\n");
        return;
    }

    printf("\n======= HISTORICO DE OPERACOES =======\n");
    while (fgets(linha, sizeof(linha), arquivo)) {
        printf("%s", linha);
    }
    fclose(arquivo);
}

// Salvar saldo no arquivo
void salvarSaldo(float saldo) {
    FILE *arquivo = fopen(ARQUIVO_SALDO, "w");
    if (arquivo != NULL) {
        fprintf(arquivo, "%.2f", saldo);
        fclose(arquivo);
    }
}

// Carregar saldo do arquivo
float carregarSaldo() {
    FILE *arquivo = fopen(ARQUIVO_SALDO, "r");
    float saldo = 1000.0;

    if (arquivo != NULL) {
        fscanf(arquivo, "%f", &saldo);
        fclose(arquivo);
    }
    return saldo;
}

int main() {
    float saldo = carregarSaldo();
    int opcao;
    float valor;
    char senha[10];
    char chavePix[100];
    char historico[300];

    // Login
    printf("Digite sua senha para acessar o sistema: ");
    scanf("%s", senha);

    if (strcmp(senha, SENHA_CORRETA) != 0) {
        printf("Senha incorreta! Acesso negado.\n");
        return 1;
    }

    do {
        mostrarMenu();
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Seu saldo atual é: R$ %.2f\n", saldo);
                break;

            case 2:
                printf("Digite o valor para saque: R$ ");
                scanf("%f", &valor);
                if(valor <= 0) {
                    printf("Valor invalido!\n");
                } else if(valor > saldo) {
                    printf("Saldo insuficiente!\n");
                } else {
                    saldo -= valor;
                    printf("Saque realizado. Novo saldo: R$ %.2f\n", saldo);
                    sprintf(historico, "Saque de R$ %.2f", valor);
                    registrarHistorico(historico);
                    salvarSaldo(saldo);
                }
                break;

            case 3:
                printf("Digite o valor para deposito: R$ ");
                scanf("%f", &valor);
                if(valor <= 0) {
                    printf("Valor invalido!\n");
                } else {
                    saldo += valor;
                    printf("Depósito realizado. Novo saldo: R$ %.2f\n", saldo);
                    sprintf(historico, "Depósito de R$ %.2f", valor);
                    registrarHistorico(historico);
                    salvarSaldo(saldo);
                }
                break;

            case 4:
                printf("Digite o valor do PIX: R$ ");
                scanf("%f", &valor);
                printf("Digite a chave PIX do destinatario: ");
                scanf("%s", chavePix);

                if(valor <= 0) {
                    printf("Valor inválido!\n");
                } else if(valor > saldo) {
                    printf("Saldo insuficiente!\n");
                } else {
                    saldo -= valor;
                    printf("PIX enviado: R$ %.2f para chave %s\n", valor, chavePix);
                    sprintf(historico, "PIX de R$ %.2f para chave %s", valor, chavePix);
                    registrarHistorico(historico);
                    salvarSaldo(saldo);
                }
                break;

            case 5:
                mostrarHistorico();
                break;

            case 6:
                printf("Encerrando... Obrigado por usar nosso caixa!\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 6);

    return 0;
}
