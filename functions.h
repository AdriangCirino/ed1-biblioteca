#ifndef FUNCTIONS_H // essa e a ultima linha foram sugeridas pela Claude=
#define FUNCTIONS_H

#include "data.h"

Usuario* cadastrarUsuario(Usuario *lista, char *nome, char *email);
Usuario* buscarUsuarioPorEmail(Usuario *lista, char *email);
Usuario* buscarUsuarioPorNome(Usuario *lista, char *nome);
Usuario* atualizarNomeUsuario(Usuario *lista, char *email, char *novoNome);
Usuario* excluirUsuario(Usuario *lista, char *email, Livro *listaLivros);
void listarEmprestimosPorUsuario(Livro *listaLivros, char *email);

Livro* cadastrarLivro(Livro *lista, IdsArvore **arvore, char *titulo, char *autor, int ano);
Livro* buscarLivroPorCodigo(Livro *lista, int codigo);
void buscarLivroPorAutor(Livro *lista, char *autor);
Livro* atualizarLivro(Livro *lista, int codigo, char *novoTitulo, char *novoAutor, int novoAno);
Livro* excluirLivro(Livro *lista, IdsArvore **arvore, int codigo);
Livro* emprestarLivro(Livro *lista, int codigo, char *emailUsuario);
Livro* devolverLivro(Livro *lista, int codigo);

IdsArvore* inserirID(IdsArvore *arvore, int codigo);
int verificarID(IdsArvore *arvore, int codigo);
void liberarArvore(IdsArvore *arvore);

int lerInteiro();

void menuPrincipal(Usuario **listaUsuarios, Livro **listaLivros, IdsArvore **arvore);
void menuCadastro(Usuario **listaUsuarios, Livro **listaLivros, IdsArvore **arvore);
void menuConsulta(Usuario *listaUsuarios, Livro *listaLivros);
void menuAtualizacao(Usuario **listaUsuarios, Livro **listaLivros);
void menuExclusao(Usuario **listaUsuarios, Livro **listaLivros, IdsArvore **arvore);

void liberarUsuarios(Usuario *lista);
void liberarLivros(Livro *lista);

#endif // FUNCTIONS_H