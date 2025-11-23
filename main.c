#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQ "pessoas.bin"

typedef struct {
    char nome[100];
    char cpf[12];
    int idade;
    char email[100];
} Pessoa;

int cpfExiste(const char *cpf) {
    FILE *f = fopen(ARQ, "rb");
    Pessoa p;
    if (!f) return 0;

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (strcmp(p.cpf, cpf) == 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void cadastrar() {
    Pessoa p;

    printf("Digite o CPF (11 dígitos): ");
    scanf("%11s", p.cpf);

    if (cpfExiste(p.cpf)) {
        printf("ERRO: CPF já cadastrado!\n");
        return;
    }

    printf("Nome: ");
    getchar();
    fgets(p.nome, 100, stdin);
    p.nome[strcspn(p.nome, "\n")] = 0;

    printf("Idade: ");
    scanf("%d", &p.idade);

    printf("E-mail: ");
    scanf("%s", p.email);

    FILE *f = fopen(ARQ, "ab");
    fwrite(&p, sizeof(Pessoa), 1, f);
    fclose(f);

    printf("Cadastro realizado com sucesso!\n");
}

void listar() {
    FILE *f = fopen(ARQ, "rb");
    Pessoa p;

    if (!f) {
        printf("Nenhum registro encontrado.\n");
        return;
    }

    printf("\n--- LISTA DE PESSOAS ---\n");

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        printf("\nNome: %s\nCPF: %s\nIdade: %d\nEmail: %s\n",
               p.nome, p.cpf, p.idade, p.email);
    }

    fclose(f);
}

void buscar() {
    char cpf[12];
    Pessoa p;
    FILE *f = fopen(ARQ, "rb");

    if (!f) {
        printf("Arquivo inexistente.\n");
        return;
    }

    printf("Digite o CPF para buscar: ");
    scanf("%11s", cpf);

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (strcmp(p.cpf, cpf) == 0) {
            printf("\n--- Pessoa Encontrada ---\n");
            printf("Nome: %s\nCPF: %s\nIdade: %d\nEmail: %s\n",
                   p.nome, p.cpf, p.idade, p.email);
            fclose(f);
            return;
        }
    }

    printf("Pessoa não encontrada.\n");
    fclose(f);
}

void atualizar() {
    char cpf[12];
    Pessoa p;
    int encontrado = 0;

    printf("Digite o CPF para atualizar: ");
    scanf("%11s", cpf);

    FILE *f = fopen(ARQ, "rb");
    FILE *temp = fopen("temp.bin", "wb");

    if (!f || !temp) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (strcmp(p.cpf, cpf) == 0) {
            encontrado = 1;

            printf("Novo nome: ");
            getchar();
            fgets(p.nome, 100, stdin);
            p.nome[strcspn(p.nome, "\n")] = 0;

            printf("Nova idade: ");
            scanf("%d", &p.idade);

            printf("Novo email: ");
            scanf("%s", p.email);
        }
        fwrite(&p, sizeof(Pessoa), 1, temp);
    }

    fclose(f);
    fclose(temp);

    remove(ARQ);
    rename("temp.bin", ARQ);

    if (encontrado)
        printf("Registro atualizado com sucesso!\n");
    else
        printf("CPF não encontrado.\n");
}

void removerPessoa() {
    char cpf[12];
    Pessoa p;
    int encontrado = 0;

    printf("Digite o CPF para remover: ");
    scanf("%11s", cpf);

    FILE *f = fopen(ARQ, "rb");
    FILE *temp = fopen("temp.bin", "wb");

    if (!f || !temp) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (strcmp(p.cpf, cpf) == 0) {
            encontrado = 1;
            continue;
        }
        fwrite(&p, sizeof(Pessoa), 1, temp);
    }

    fclose(f);
    fclose(temp);

    remove(ARQ);
    rename("temp.bin", ARQ);

    if (encontrado)
        printf("Registro removido com sucesso!\n");
    else
        printf("CPF não encontrado.\n");
}

int main() {
    int opcao;

    do {
        printf("\n=== MENU ===\n");
        printf("1 - Cadastrar pessoa\n");
        printf("2 - Listar pessoas\n");
        printf("3 - Buscar pessoa por CPF\n");
        printf("4 - Atualizar pessoa\n");
        printf("5 - Remover pessoa\n");
        printf("6 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrar(); break;
            case 2: listar(); break;
            case 3: buscar(); break;
            case 4: atualizar(); break;
            case 5: removerPessoa(); break;
            case 6: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }

    } while (opcao != 6);

    return 0;
}
