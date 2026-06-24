#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"

int proximoID = 1; // contador global de IDs dos livros

//  USUARIOS 

Usuario* cadastrarUsuario(Usuario *lista, char *nome, char *email){

    // Verifica se o email ja existe
    Usuario *temp = lista;
    while(temp != NULL){
        if(strcmp(temp->email, email) == 0){
            printf("Email ja cadastrado!\n");
            return lista;
        }
        temp = temp->next;
    }

    // Cria o novo usuario
    Usuario *novo;
    novo = (Usuario*) malloc(sizeof(Usuario));

    strcpy(novo->nome, nome);
    strcpy(novo->email, email);
    novo->next = NULL;

    // Caso 1: lista vazia ou novo email vem antes do primeiro
    if(lista == NULL || strcmp(novo->email, lista->email) < 0){
        novo->next = lista;
        return novo;
    }

    // Caso 2: percorre a lista até achar a posicao certa
    Usuario *atual = lista;
    while(atual->next != NULL && strcmp(atual->next->email, novo->email) < 0){
        atual = atual->next;
    }

    novo->next = atual->next;
    atual->next = novo;

    return lista;
}

Usuario* buscarUsuarioPorEmail(Usuario *lista, char *email){

    Usuario *atual = lista;

    while(atual != NULL){
        if(strcmp(atual->email, email) == 0){
            return atual;
        }
        atual = atual->next;
    }

    return NULL;
}

Usuario* buscarUsuarioPorNome(Usuario *lista, char *nome){

    Usuario *atual = lista;

    while(atual != NULL){
        if(strcmp(atual->nome, nome) == 0){
            return atual;
        }
        atual = atual->next;
    }

    return NULL;
}

Usuario* atualizarNomeUsuario(Usuario *lista, char *email, char *novoNome){

    Usuario *atual = buscarUsuarioPorEmail(lista, email);
    if(atual == NULL){
        printf("Usuario nao encontrado!\n");
        return lista;
    }
    strcpy(atual->nome, novoNome);
    return lista;
}

Usuario* excluirUsuario(Usuario *lista, char *email){

    Usuario *atual = buscarUsuarioPorEmail(lista, email);
    if(atual == NULL){
        printf("Usuario nao encontrado!\n");
        return lista;
    }
    if(atual == lista){
        lista = lista->next;
        free(atual);
        return lista;
    }
    Usuario *anterior = lista;
    while(anterior->next != atual){
        anterior = anterior->next;
    }
    anterior->next = atual->next;
    free(atual);
    return lista;
}

void listarEmprestimosPorUsuario(Livro *listaLivros, char *email){

    Livro *atual = listaLivros;
    int encontrou = 0;

    while(atual != NULL){
        if(atual->status == 1 && strcmp(atual->email_usuario, email) == 0){
            printf("Codigo: %d | Titulo: %s | Autor: %s\n", atual->codigo, atual->titulo, atual->autor);
            encontrou = 1;
        }
        atual = atual->next;
    }

    if(encontrou == 0){
        printf("Nenhum livro emprestado para esse usuario.\n");
    }
}

//  ARVORE DE IDS (BST) 

IdsArvore* inserirID(IdsArvore *arvore, int codigo){

    if(arvore == NULL){
        IdsArvore *novo;
        novo = (IdsArvore*) malloc(sizeof(IdsArvore));
        novo->codigo = codigo;
        novo->left = NULL;
        novo->right = NULL;
        return novo;
    }

    if(codigo < arvore->codigo){
        arvore->left = inserirID(arvore->left, codigo);
    } else if(codigo > arvore->codigo){
        arvore->right = inserirID(arvore->right, codigo);
    }
    // se for igual, ja existe, nao insere de novo

    return arvore;
}

int verificarID(IdsArvore *arvore, int codigo){

    if(arvore == NULL){
        return 0; // nao encontrado
    }

    if(codigo == arvore->codigo){
        return 1; // encontrado
    } else if(codigo < arvore->codigo){
        return verificarID(arvore->left, codigo);
    } else {
        return verificarID(arvore->right, codigo);
    }
}

// Observacao: a arvore de IDs e usada apenas como estrutura auxiliar de
// verificacao rapida de duplicidade (extra do trabalho). Por isso nao
// implementamos remocao de nos dela: mesmo excluindo um livro, manter o
// codigo registrado na arvore nao causa efeito colateral no sistema, e
// simplifica o codigo evitando a logica mais complexa de remocao em BST.

void liberarArvore(IdsArvore *arvore){

    if(arvore == NULL){
        return;
    }

    liberarArvore(arvore->left);
    liberarArvore(arvore->right);
    free(arvore);
}

//  LIVROS 

Livro* cadastrarLivro(Livro *lista, IdsArvore **arvore, char *titulo, char *autor, int ano){

    int codigo = proximoID;
    proximoID = proximoID + 1;

    Livro *novo;
    novo = (Livro*) malloc(sizeof(Livro));

    strcpy(novo->titulo, titulo);
    strcpy(novo->autor, autor);
    novo->ano = ano;
    novo->codigo = codigo;
    novo->status = 0; // disponivel por padrao
    strcpy(novo->email_usuario, ""); // ninguem pegou ainda
    novo->next = NULL;

    // insere o codigo na arvore de IDs (extra)
    *arvore = inserirID(*arvore, codigo);

    // insercao ordenada por codigo
    if(lista == NULL || novo->codigo < lista->codigo){
        novo->next = lista;
        return novo;
    }

    Livro *atual = lista;
    while(atual->next != NULL && atual->next->codigo < novo->codigo){
        atual = atual->next;
    }

    novo->next = atual->next;
    atual->next = novo;

    return lista;
}

Livro* buscarLivroPorCodigo(Livro *lista, int codigo){

    Livro *atual = lista;

    while(atual != NULL){
        if(atual->codigo == codigo){
            return atual;
        }
        atual = atual->next;
    }

    return NULL;
}

void buscarLivroPorAutor(Livro *lista, char *autor){

    Livro *atual = lista;
    int encontrou = 0;

    while(atual != NULL){
        if(strcmp(atual->autor, autor) == 0){
            printf("Codigo: %d | Titulo: %s | Ano: %d\n", atual->codigo, atual->titulo, atual->ano);
            encontrou = 1;
        }
        atual = atual->next;
    }

    if(encontrou == 0){
        printf("Livro nao encontrado\n");
    }
}

Livro* atualizarLivro(Livro *lista, int codigo, char *novoTitulo, char *novoAutor, int novoAno){

    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("Livro nao encontrado\n");
        return lista;
    }

    strcpy(atual->titulo, novoTitulo);
    strcpy(atual->autor, novoAutor);
    atual->ano = novoAno;

    return lista;
}

Livro* excluirLivro(Livro *lista, IdsArvore **arvore, int codigo){

    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("Livro nao encontrado\n");
        return lista;
    }

    if(atual == lista){
        lista = lista->next;
        free(atual);
        return lista;
    }

    Livro *anterior = lista;
    while(anterior->next != atual){
        anterior = anterior->next;
    }
    anterior->next = atual->next;
    free(atual);

    return lista;
}

Livro* emprestarLivro(Livro *lista, int codigo, char *emailUsuario){

    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("Livro nao encontrado\n");
        return lista;
    }

    if(atual->status == 1){
        printf("Livro ja esta emprestado\n");
        return lista;
    }

    atual->status = 1;
    strcpy(atual->email_usuario, emailUsuario);

    return lista;
}

Livro* devolverLivro(Livro *lista, int codigo){

    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("Livro nao encontrado\n");
        return lista;
    }

    if(atual->status == 0){
        printf("Livro nao estava emprestado\n");
        return lista;
    }

    atual->status = 0;
    strcpy(atual->email_usuario, "");

    return lista;
}

//  MEMORIA 

void liberarUsuarios(Usuario *lista){

    Usuario *atual = lista;
    Usuario *proximo;

    while(atual != NULL){
        proximo = atual->next;
        free(atual);
        atual = proximo;
    }
}

void liberarLivros(Livro *lista){

    Livro *atual = lista;
    Livro *proximo;

    while(atual != NULL){
        proximo = atual->next;
        free(atual);
        atual = proximo;
    }
}

//  MENUS 

void menuCadastro(Usuario **listaUsuarios, Livro **listaLivros, IdsArvore **arvore){

    int opcao;
    char nome[100], email[100], titulo[100], autor[100];
    int ano;

    do{
        printf("\n--- CADASTRO ---\n");
        printf("1. Livros\n");
        printf("2. Usuarios\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if(opcao == 1){
            printf("Titulo: ");
            scanf("%s", titulo);
            printf("Autor: ");
            scanf("%s", autor);
            printf("Ano: ");
            scanf("%d", &ano);

            *listaLivros = cadastrarLivro(*listaLivros, arvore, titulo, autor, ano);
            printf("Livro cadastrado com sucesso!\n");
        }
        else if(opcao == 2){
            printf("Nome: ");
            scanf("%s", nome);
            printf("Email: ");
            scanf("%s", email);

            *listaUsuarios = cadastrarUsuario(*listaUsuarios, nome, email);
        }
        else if(opcao != 0){
            printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
}

void menuConsulta(Usuario *listaUsuarios, Livro *listaLivros){

    int opcao, subOpcao, codigo;
    char busca[100];
    Usuario *u;
    Livro *l;

    do{
        printf("\n--- CONSULTA ---\n");
        printf("1. Livros\n");
        printf("2. Usuarios\n");
        printf("3. Emprestimos\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if(opcao == 1){
            printf("Buscar por: 1-Codigo  2-Autor: ");
            scanf("%d", &subOpcao);

            if(subOpcao == 1){
                printf("Codigo: ");
                scanf("%d", &codigo);
                l = buscarLivroPorCodigo(listaLivros, codigo);
                if(l == NULL){
                    printf("Livro nao encontrado\n");
                } else {
                    printf("Titulo: %s | Autor: %s | Ano: %d | Status: %s\n",
                        l->titulo, l->autor, l->ano, l->status == 0 ? "Disponivel" : "Emprestado");
                }
            }
            else if(subOpcao == 2){
                printf("Autor: ");
                scanf("%s", busca);
                buscarLivroPorAutor(listaLivros, busca);
            }
        }
        else if(opcao == 2){
            printf("Buscar por: 1-Email  2-Nome: ");
            scanf("%d", &subOpcao);
            printf("Valor: ");
            scanf("%s", busca);

            if(subOpcao == 1){
                u = buscarUsuarioPorEmail(listaUsuarios, busca);
            } else {
                u = buscarUsuarioPorNome(listaUsuarios, busca);
            }

            if(u == NULL){
                printf("Usuario nao cadastrado\n");
            } else {
                printf("Nome: %s | Email: %s\n", u->nome, u->email);
            }
        }
        else if(opcao == 3){
            printf("Email do usuario: ");
            scanf("%s", busca);
            listarEmprestimosPorUsuario(listaLivros, busca);
        }
        else if(opcao != 0){
            printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
}

void menuAtualizacao(Usuario **listaUsuarios, Livro **listaLivros){

    int opcao, codigo, ano;
    char email[100], nome[100], titulo[100], autor[100];

    do{
        printf("\n--- ATUALIZACAO ---\n");
        printf("1. Livros\n");
        printf("2. Usuarios\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if(opcao == 1){
            printf("Codigo do livro: ");
            scanf("%d", &codigo);
            printf("Novo titulo: ");
            scanf("%s", titulo);
            printf("Novo autor: ");
            scanf("%s", autor);
            printf("Novo ano: ");
            scanf("%d", &ano);

            *listaLivros = atualizarLivro(*listaLivros, codigo, titulo, autor, ano);
        }
        else if(opcao == 2){
            printf("Email do usuario: ");
            scanf("%s", email);
            printf("Novo nome: ");
            scanf("%s", nome);

            *listaUsuarios = atualizarNomeUsuario(*listaUsuarios, email, nome);
        }
        else if(opcao != 0){
            printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
}

void menuExclusao(Usuario **listaUsuarios, Livro **listaLivros, IdsArvore **arvore){

    int opcao, codigo;
    char email[100];

    do{
        printf("\n--- EXCLUSAO ---\n");
        printf("1. Livros\n");
        printf("2. Usuarios\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if(opcao == 1){
            printf("Codigo do livro: ");
            scanf("%d", &codigo);
            *listaLivros = excluirLivro(*listaLivros, arvore, codigo);
        }
        else if(opcao == 2){
            printf("Email do usuario: ");
            scanf("%s", email);
            *listaUsuarios = excluirUsuario(*listaUsuarios, email);
        }
        else if(opcao != 0){
            printf("Opcao invalida!\n");
        }

    } while(opcao != 0);
}

void menuPrincipal(Usuario **listaUsuarios, Livro **listaLivros, IdsArvore **arvore){

    int opcao, codigo;
    char email[100];

    do{
        printf("\n\n");
        printf("   SISTEMA DE CONTROLE DE BIBLIOTECA\n");
        printf("\n");
        printf("1. Cadastro\n");
        printf("2. Consulta\n");
        printf("3. Atualizacao\n");
        printf("4. Exclusao\n");
        printf("5. Emprestimo\n");
        printf("6. Devolucao\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if(opcao == 1){
            menuCadastro(listaUsuarios, listaLivros, arvore);
        }
        else if(opcao == 2){
            menuConsulta(*listaUsuarios, *listaLivros);
        }
        else if(opcao == 3){
            menuAtualizacao(listaUsuarios, listaLivros);
        }
        else if(opcao == 4){
            menuExclusao(listaUsuarios, listaLivros, arvore);
        }
        else if(opcao == 5){
            printf("Codigo do livro a emprestar: ");
            scanf("%d", &codigo);
            printf("Email do usuario: ");
            scanf("%s", email);
            *listaLivros = emprestarLivro(*listaLivros, codigo, email);
        }
        else if(opcao == 6){
            printf("Codigo do livro a devolver: ");
            scanf("%d", &codigo);
            *listaLivros = devolverLivro(*listaLivros, codigo);
        }
        else if(opcao != 0){
            printf("Opcao invalida!\n");
        }

    } while(opcao != 0);

    printf("\nFinalizando o sistema...\n");
}