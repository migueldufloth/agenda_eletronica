#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMITE 100
#define CIANO "\x1b[36m"
#define VERDE "\x1b[32m"
#define RESETAR "\x1b[0m"
#define VERMELHO "\x1b[31m"
#define NOME_DO_ARQUIVO "agenda_eletronica.txt"

struct contato{
    char nome[50];
    char telefone[16];
};

struct contato contatos[LIMITE];
int contador = 0;

void menu();
void abrirAgenda();
void excluirContato();
void consultarContato();
void adicionarContato();
void procurarContato(int i);
int inserirContato(struct contato ctt);
void salvarContato(struct contato ctt, FILE *arquivo);
int buscarContato(char *a, char *b, int i, int limite);

int main(void){
    abrirAgenda();
    menu();

    return 0;
}

// funções 
void abrirAgenda(){
    FILE *arquivo = fopen(NOME_DO_ARQUIVO, "rb");

    if (arquivo != NULL){
        contador = 0;

        while (!feof(arquivo)){
            fgets(contatos[contador].nome, 50, arquivo);
            if (strcmp(contatos[contador].nome, "") > 0){
                fgets(contatos[contador].telefone, 16, arquivo);
                contador++;
            }
        }
        fclose(arquivo);
    }
    else{
        printf(VERMELHO "Erro ao carregar os contatos\n\n" RESETAR);
    }
}

void salvarContato(struct contato ctt, FILE *arquivo){
    fputs(ctt.nome, arquivo);
    fputs(ctt.telefone, arquivo);
}

int inserirContato(struct contato ctt){
    int r = 0;
    FILE *arquivo = fopen(NOME_DO_ARQUIVO, "a+");

    if (arquivo != NULL){
        r = 1;
        salvarContato(ctt, arquivo);
        contatos[contador] = ctt;
        contador++;
        fclose(arquivo);
    }

    return r;
}

void adicionarContato(){
    printf("\nADICIONAR CONTATO\n\n");

    struct contato ctt;

    printf("Digite o nome do contato: ");
    fgets(ctt.nome, 50, stdin);
    fflush(stdin);
    printf("Digite o telefone do contato: ");
    fgets(ctt.telefone, 16, stdin);
    fflush(stdin);

    printf("\n");

    if (inserirContato(ctt)){
        printf(VERDE "Contato salvo com sucesso\n\n" RESETAR);
    }
    else{
        printf(VERMELHO "Erro ao salvar o contato\n\n" RESETAR);
    }
}

void procurarContato(int i){
    printf(CIANO "Nome: %s", contatos[i].nome);
    printf("Telefone: %s\n" RESETAR, contatos[i].telefone);
}

int buscarContato(char *a, char *b, int i, int limite){
    if (a[i] == b[i]){
        i++;
        if (i < limite){
            return buscarContato(a, b, i, limite);
        }
        else{
            return 1;
        }
    }
    else{
        return 0;
    }
}

void consultarContato(){
    char nome[50] = {""};
    int encontrado = 0;

    printf("\nCONSULTAR CONTATO\n\n");

    printf("Digite o nome do contato que deseja consultar: ");
    fgets(nome, 50, stdin);
    fflush(stdin);

    printf("\n");

    int limite = strlen(nome) - 1;

    for (int i = 0; i < contador; i++){
        if (strlen(contatos[i].nome) >= limite){
            if (buscarContato(nome, contatos[i].nome, 0, limite)){
                procurarContato(i);
                encontrado = 1;
            }
        }
    }

    if (!encontrado){
        printf(VERMELHO "Contato nao encontrado\n\n" RESETAR);
    }
}

void excluirContato(){
    char nome[50] = {""};

    printf("\nEXCLUIR CONTATO\n\n");

    printf("Digite o nome do contato que deseja excluir: ");
    fgets(nome, 50, stdin);
    fflush(stdin);

    printf("\n");

    int limite = strlen(nome) - 1;
    int encontrado = 0;

    for (int i = 0; i < contador; i++){
        if (strlen(contatos[i].nome) >= limite){
            if (buscarContato(nome, contatos[i].nome, 0, limite)){
                encontrado = 1;
                for (int j = i; j < contador - 1; j++){
                    contatos[j] = contatos[j + 1];
                }
                contador--;
                break;
            }
        }
    }

    if (encontrado){
        FILE *arquivo = fopen(NOME_DO_ARQUIVO, "wb");

        if (arquivo != NULL){
            for (int i = 0; i < contador; i++){
                salvarContato(contatos[i], arquivo);
            }
            fclose(arquivo);
            printf(VERDE "Contato excluido com sucesso\n\n" RESETAR);
        }
        else{
            printf(VERMELHO "Erro ao atualizar os contatos\n\n" RESETAR);
        }
    }
    else{
        printf(VERMELHO "Contato nao encontrado\n\n" RESETAR);
    }
}

void menu(){
    char opcao;

    do{
        printf("AGENDA ELETRONICA\n\n");
        printf("[1] Adicionar um contato\n");
        printf("[2] Consultar um contato\n");
        printf("[3] Excluir um contato\n");
        printf("[4] Fechar\n");
        printf("\nDigite uma opcao: ");
        scanf("%c", &opcao);
        fflush(stdin);

        switch (opcao){
        case '1':
            adicionarContato();
            break;
        case '2':
            consultarContato();
            break;
        case '3':
            excluirContato();
            break;
        case '4':
            printf("\nFechando...");
            break;
        default:
            printf(VERMELHO "\nOpcao invalida\n\n" RESETAR);
        }

    } while (opcao != '4');
}