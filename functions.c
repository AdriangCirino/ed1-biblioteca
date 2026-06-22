#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"

int proximoID = 1; // contador global de IDs dos livros

//  USUARIOS 

Usuario* cadastrarUsuario(Usuario *lista, char *nome, char *email){

    // Cria o novo usuario
    Usuario *novo;
    novo = (Usuario*) malloc(sizeof(Usuario));

    strcpy(novo->nome, nome);
    strcpy(novo->email, email);
    novo->next = NULL;

    //se a lista for vazia ou precisar inserir o email novo antes do primeiro existente
    if(lista == NULL || strcmp(novo->email, lista->email) < 0){
        novo->next = lista;
        return novo;
    }

       //percorre a lista ate achar o lugar certo
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

//  ARVORE DE IDS

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