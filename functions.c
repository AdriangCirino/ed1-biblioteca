#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"

int proximoID = 1; // Contador global para gerar o codigo dos livros

// --- FUNCOES AUXILIARES ---

int lerInteiro(){
    int valor;
    int resultado = scanf("%d", &valor);

    // Se o usuario digitar letra em vez de numero, limpa o lixo
    if(resultado != 1){
        while(getchar() != '\n'); 
        return -1;
    }

    // Limpa o '\n' que sobra no buffer
    int c;
    while((c = getchar()) != '\n' && c != EOF);

    return valor;
}

void lerString(char *destino, int tamanho){
    fgets(destino, tamanho, stdin);

    // Remove o '\n' do final da string, se existir
    int len = strlen(destino);
    if(len > 0 && destino[len - 1] == '\n'){
        destino[len - 1] = '\0';
    }
}


// --- USUARIOS ---

Usuario* cadastrarUsuario(Usuario *lista, char *nome, char *email){
    Usuario *temp = lista;
    
    // Verifica se ja existe alguem com esse email ou nome
    while(temp != NULL){
        if(strcmp(temp->email, email) == 0){
            printf("\n[!] Erro: Email ja cadastrado!\n");
            return lista;
        }
        if(strcmp(temp->nome, nome) == 0){
            printf("\n[!] Erro: Nome ja cadastrado!\n");
            return lista;
        }
        temp = temp->next;
    }

    // Cria e preenche o novo usuario
    Usuario *novo = (Usuario*) malloc(sizeof(Usuario));
    strcpy(novo->nome, nome);
    strcpy(novo->email, email);
    novo->next = NULL;

    // Insere em ordem alfabetica de email
    if(lista == NULL || strcmp(novo->email, lista->email) < 0){
        novo->next = lista;
        printf("\n[+] Usuario cadastrado com sucesso!\n");
        return novo;
    }

    Usuario *atual = lista;
    while(atual->next != NULL && strcmp(atual->next->email, novo->email) < 0){
        atual = atual->next;
    }

    novo->next = atual->next;
    atual->next = novo;

    printf("\n[+] Usuario cadastrado com sucesso!\n");
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
        printf("\n[!] Usuario nao encontrado!\n");
        return lista;
    }
    strcpy(atual->nome, novoNome);
    printf("\n[+] Nome atualizado com sucesso!\n");
    return lista;
}

Usuario* excluirUsuario(Usuario *lista, char *email){
    Usuario *atual = buscarUsuarioPorEmail(lista, email);
    if(atual == NULL){
        printf("\n[!] Usuario nao encontrado!\n");
        return lista;
    }

    // Se for o primeiro da lista
    if(atual == lista){
        lista = lista->next;
        free(atual);
        printf("\n[+] Usuario excluido com sucesso!\n");
        return lista;
    }
    
    // Procura quem aponta para o usuario que vamos excluir
    Usuario *anterior = lista;
    while(anterior->next != atual){
        anterior = anterior->next;
    }
    
    anterior->next = atual->next; // Pula o no atual
    free(atual);
    
    printf("\n[+] Usuario excluido com sucesso!\n");
    return lista;
}

void listarEmprestimosPorUsuario(Livro *listaLivros, char *email){
    Livro *atual = listaLivros;
    int encontrou = 0;

    printf("\n--- Livros com o usuario %s ---\n", email);
    while(atual != NULL){
        if(atual->status == 1 && strcmp(atual->email_usuario, email) == 0){
            printf("Codigo: %d | Titulo: %s\n", atual->codigo, atual->titulo);
            encontrou = 1;
        }
        atual = atual->next;
    }

    if(encontrou == 0){
        printf("Nenhum livro emprestado para esse usuario.\n");
    }
}


// --- ARVORE DE IDS ---

IdsArvore* inserirID(IdsArvore *arvore, int codigo){
    if(arvore == NULL){
        IdsArvore *novo = (IdsArvore*) malloc(sizeof(IdsArvore));
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
    return arvore;
}

int verificarID(IdsArvore *arvore, int codigo){
    if(arvore == NULL){
        return 0; 
    }
    if(codigo == arvore->codigo){
        return 1; 
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


// --- LIVROS ---

Livro* cadastrarLivro(Livro *lista, IdsArvore **arvore, char *titulo, char *autor, int ano){
    
    // Verifica se ja existe livro com mesmo titulo
    Livro *temp = lista;
    while(temp != NULL){
        if(strcmp(temp->titulo, titulo) == 0){
            printf("\n[!] Erro: Livro com este titulo ja existe!\n");
            return lista;
        }
        temp = temp->next;
    }

    int codigo = proximoID;
    proximoID = proximoID + 1;

    Livro *novo = (Livro*) malloc(sizeof(Livro));
    strcpy(novo->titulo, titulo);
    strcpy(novo->autor, autor);
    novo->ano = ano;
    novo->codigo = codigo;
    novo->status = 0; 
    strcpy(novo->email_usuario, ""); 
    novo->next = NULL;

    *arvore = inserirID(*arvore, codigo);

    printf("\n[+] Livro cadastrado! Codigo gerado: %d\n", codigo);

    // Insere ordenado por codigo crescente
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

    printf("\n--- Resultados para o autor: %s ---\n", autor);
    while(atual != NULL){
        if(strcmp(atual->autor, autor) == 0){
            printf("Cod: %d | Titulo: %s | Ano: %d\n", atual->codigo, atual->titulo, atual->ano);
            encontrou = 1;
        }
        atual = atual->next;
    }

    if(encontrou == 0){
        printf("Nenhum livro encontrado.\n");
    }
}

Livro* atualizarLivro(Livro *lista, int codigo, char *novoTitulo, char *novoAutor, int novoAno){
    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("\n[!] Livro nao encontrado.\n");
        return lista;
    }

    strcpy(atual->titulo, novoTitulo);
    strcpy(atual->autor, novoAutor);
    atual->ano = novoAno;
    
    printf("\n[+] Livro atualizado com sucesso!\n");
    return lista;
}

Livro* excluirLivro(Livro *lista, IdsArvore **arvore, int codigo){
    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("\n[!] Livro nao encontrado.\n");
        return lista;
    }

    // Nao deixa excluir se estiver emprestado
    if(atual->status == 1){
        printf("\n[!] Nao e possivel excluir: o livro esta emprestado.\n");
        return lista;
    }

    if(atual == lista){
        lista = lista->next;
        free(atual);
        printf("\n[+] Livro excluido com sucesso!\n");
        return lista;
    }

    Livro *anterior = lista;
    while(anterior->next != atual){
        anterior = anterior->next;
    }
    anterior->next = atual->next;
    free(atual);

    printf("\n[+] Livro excluido com sucesso!\n");
    return lista;
}

Livro* emprestarLivro(Livro *lista, int codigo, char *emailUsuario){
    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("\n[!] Livro nao encontrado.\n");
        return lista;
    }

    if(atual->status == 1){
        printf("\n[!] Livro ja esta emprestado para outro usuario.\n");
        return lista;
    }

    atual->status = 1;
    strcpy(atual->email_usuario, emailUsuario);
    
    printf("\n[+] Livro emprestado com sucesso!\n");
    return lista;
}

Livro* devolverLivro(Livro *lista, int codigo){
    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("\n[!] Livro nao encontrado.\n");
        return lista;
    }

    if(atual->status == 0){
        printf("\n[!] Este livro ja esta disponivel na biblioteca.\n");
        return lista;
    }

    atual->status = 0;
    strcpy(atual->email_usuario, "");
    
    printf("\n[+] Devolucao realizada com sucesso!\n");
    return lista;
}


// --- MEMORIA ---

void liberarUsuarios(Usuario *lista){
    Usuario *atual = lista;
    while(atual != NULL){
        Usuario *proximo = atual->next;
        free(atual);
        atual = proximo;
    }
}

void liberarLivros(Livro *lista){
    Livro *atual = lista;
    while(atual != NULL){
        Livro *proximo = atual->next;
        free(atual);
        atual = proximo;
    }
}


// --- MENUS ---

void menuCadastro(Usuario **listaUsuarios, Livro **listaLivros, IdsArvore **arvore){
    int opcao, ano;
    char nome[100], email[100], titulo[100], autor[100];

    do{
        printf("\n=== CADASTRO ===\n");
        printf("1. Novo Livro\n");
        printf("2. Novo Usuario\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        opcao = lerInteiro();

        if(opcao == 1){
            printf("Titulo do livro: ");
            lerString(titulo, sizeof(titulo));
            printf("Autor: ");
            lerString(autor, sizeof(autor));
            printf("Ano: ");
            ano = lerInteiro();

            *listaLivros = cadastrarLivro(*listaLivros, arvore, titulo, autor, ano);
        }
        else if(opcao == 2){
            printf("Nome do usuario: ");
            lerString(nome, sizeof(nome));
            printf("Email: ");
            lerString(email, sizeof(email));

            *listaUsuarios = cadastrarUsuario(*listaUsuarios, nome, email);
        }
        else if(opcao != 0){
            printf("\n[!] Opcao invalida!\n");
        }
    } while(opcao != 0);
}

void menuConsulta(Usuario *listaUsuarios, Livro *listaLivros){
    int opcao, subOpcao, codigo;
    char busca[100];
    Usuario *u;
    Livro *l;

    do{
        printf("\n=== CONSULTA ===\n");
        printf("1. Buscar Livro\n");
        printf("2. Buscar Usuario\n");
        printf("3. Ver Emprestimos de um Usuario\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        opcao = lerInteiro();

        if(opcao == 1){
            printf("Buscar por: 1-Codigo ou 2-Autor: ");
            subOpcao = lerInteiro();

            if(subOpcao == 1){
                printf("Codigo: ");
                codigo = lerInteiro();
                l = buscarLivroPorCodigo(listaLivros, codigo);
                if(l == NULL){
                    printf("\n[!] Livro nao encontrado.\n");
                } else {
                    printf("\n--- Livro Encontrado ---\n");
                    printf("Titulo: %s\nAutor: %s\nAno: %d\nStatus: %s\n",
                        l->titulo, l->autor, l->ano, l->status == 0 ? "Disponivel" : "Emprestado");
                }
            }
            else if(subOpcao == 2){
                printf("Autor: ");
                lerString(busca, sizeof(busca));
                buscarLivroPorAutor(listaLivros, busca);
            }
        }
        else if(opcao == 2){
            printf("Buscar por: 1-Email ou 2-Nome: ");
            subOpcao = lerInteiro();
            
            printf("Digite o nome: ");
            lerString(busca, sizeof(busca));

            if(subOpcao == 1){
                u = buscarUsuarioPorEmail(listaUsuarios, busca);
            } else {
                u = buscarUsuarioPorNome(listaUsuarios, busca);
            }

            if(u == NULL){
                printf("\n[!] Usuario nao encontrado.\n");
            } else {
                printf("\n--- Usuario Encontrado ---\n");
                printf("Nome: %s | Email: %s\n", u->nome, u->email);
            }
        }
        else if(opcao == 3){
            printf("Email do usuario: ");
            lerString(busca, sizeof(busca));
            listarEmprestimosPorUsuario(listaLivros, busca);
        }
        else if(opcao != 0){
            printf("\n[!] Opcao invalida!\n");
        }

    } while(opcao != 0);
}

void menuAtualizacao(Usuario **listaUsuarios, Livro **listaLivros){
    int opcao, codigo, ano;
    char titulo[100], autor[100], email[100], nome[100];

    do {
        printf("\n=== ATUALIZACAO ===\n");
        printf("1. Editar Livro\n");
        printf("2. Editar Nome do Usuario\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        opcao = lerInteiro();

        if(opcao == 1){
            printf("Codigo do livro: ");
            codigo = lerInteiro();

            Livro *l = buscarLivroPorCodigo(*listaLivros, codigo);
            if(l == NULL){
                printf("\n[!] Livro nao encontrado.\n");
            } else {
                printf("\nLivro atual: %s | Autor: %s | Ano: %d\n", l->titulo, l->autor, l->ano);
                printf("O que deseja alterar?\n");
                printf("1. Titulo\n2. Autor\n3. Ano\n0. Cancelar\nEscolha: ");
                int campo = lerInteiro();

                if(campo == 1){
                    printf("Novo titulo: ");
                    lerString(titulo, sizeof(titulo));
                    *listaLivros = atualizarLivro(*listaLivros, codigo, titulo, l->autor, l->ano);
                }
                else if(campo == 2){
                    printf("Novo autor: ");
                    lerString(autor, sizeof(autor));
                    *listaLivros = atualizarLivro(*listaLivros, codigo, l->titulo, autor, l->ano);
                }
                else if(campo == 3){
                    printf("Novo ano: ");
                    ano = lerInteiro();
                    *listaLivros = atualizarLivro(*listaLivros, codigo, l->titulo, l->autor, ano);
                }
            }
        }
        else if(opcao == 2){
            printf("Email do usuario: ");
            lerString(email, sizeof(email));
            printf("Novo nome: ");
            lerString(nome, sizeof(nome));
            
            *listaUsuarios = atualizarNomeUsuario(*listaUsuarios, email, nome);
        }
        else if(opcao != 0){
            printf("\n[!] Opcao invalida!\n");
        }
    } while(opcao != 0);
}

void menuExclusao(Usuario **listaUsuarios, Livro **listaLivros, IdsArvore **arvore){
    int opcao, codigo;
    char email[100];

    do{
        printf("\n=== EXCLUSAO ===\n");
        printf("1. Excluir Livro\n");
        printf("2. Excluir Usuario\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        opcao = lerInteiro();

        if(opcao == 1){
            printf("Codigo do livro: ");
            codigo = lerInteiro();

            Livro *l = buscarLivroPorCodigo(*listaLivros, codigo);
            if(l == NULL){
                printf("\n[!] Livro nao encontrado.\n");
            } else {
                printf("\nLivro: %s | Autor: %s\n", l->titulo, l->autor);
                printf("Confirma a exclusao? 1-Sim ou 2-Nao: ");
                int confirma = lerInteiro();

                if(confirma == 1){
                    *listaLivros = excluirLivro(*listaLivros, arvore, codigo);
                } else {
                    printf("\n[-] Cancelado.\n");
                }
            }
        }
        else if(opcao == 2){
            printf("Email do usuario: ");
            lerString(email, sizeof(email));
            
            // Verificacao cruzada: checa se o usuario tem livros pendentes
            Livro *temp = *listaLivros;
            int temLivro = 0;
            while(temp != NULL){
                if(temp->status == 1 && strcmp(temp->email_usuario, email) == 0){
                    temLivro = 1;
                    break;
                }
                temp = temp->next;
            }

            if(temLivro == 1){
                printf("\n[!] Nao e possivel excluir: usuario possui livros pendentes.\n");
                printf("Faca a devolucao antes de exclui-lo.\n");
            } else {
                *listaUsuarios = excluirUsuario(*listaUsuarios, email);
            }
        }
        else if(opcao != 0){
            printf("\n[!] Opcao invalida!\n");
        }

    } while(opcao != 0);
}

void menuPrincipal(Usuario **listaUsuarios, Livro **listaLivros, IdsArvore **arvore){
    int opcao, codigo;
    char email[100];

    do{
        printf("\n========================================\n");
        printf("   SISTEMA DE CONTROLE DE BIBLIOTECA\n");
        printf("========================================\n");
        printf("1. Cadastro\n");
        printf("2. Consulta\n");
        printf("3. Atualizacao\n");
        printf("4. Exclusao\n");
        printf("5. Emprestimo\n");
        printf("6. Devolucao\n");
        printf("0. Sair\n");
        printf("========================================\n");
        printf("Escolha uma opcao: ");
        opcao = lerInteiro();

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
            printf("\n=== EMPRESTIMO ===\n");
            printf("Email do usuario: ");
            lerString(email, sizeof(email));
            
            // Validacao cruzada: so empresta se o usuario existir
            Usuario *u = buscarUsuarioPorEmail(*listaUsuarios, email);
            if(u == NULL){
                printf("\n[!] Erro: Usuario nao cadastrado.\n");
            } else {
                printf("Codigo do livro: ");
                codigo = lerInteiro();
                *listaLivros = emprestarLivro(*listaLivros, codigo, email);
            }
        }
        else if(opcao == 6){
            printf("\n=== DEVOLUCAO ===\n");
            printf("Codigo do livro: ");
            codigo = lerInteiro();
            *listaLivros = devolverLivro(*listaLivros, codigo);
        }
        else if(opcao != 0){
            printf("\n[!] Opcao invalida!\n");
        }

    } while(opcao != 0);

    printf("\nSaindo do sistema...\n");
}