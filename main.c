#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"


int main(){

    Usuario *listaUsuarios = NULL;
    Livro *listaLivros = NULL;
    IdsArvore *arvoreIDs = NULL;

    menuPrincipal(&listaUsuarios, &listaLivros, &arvoreIDs);

    // Libera toda a memoria alocada antes de finalizar o programa
    liberarUsuarios(listaUsuarios);
    liberarLivros(listaLivros);
    liberarArvore(arvoreIDs);

    return 0;
}