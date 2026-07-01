#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"

int proximoID = 1; // Contador global para gerar o codigo dos livros

//  FUNCOES AUXILIARES 

int lerInteiro(){
    int valor;
    int resultado = scanf("%d", &valor);

    // Se o usuario digitar alguma coisa diferente em vez de numero, limpa o lixo do buffer
    if(resultado != 1){
        while(getchar() != '\n'); 
        return -1;
    }

    // Limpa o '\n' que sobra no buffer apos o scanf
    int c;
    while((c = getchar()) != '\n' && c != EOF);

    return valor;
}

// Obs: As funcoes de leitura (lerInteiro e lerString) foram sugeridas e geradas pelo Claude 
// para evitar os bugs nativos do scanf ao lidar com strings e espacos em C.
void lerString(char *destino, int tamanho){
    fgets(destino, tamanho, stdin);

    // Remove o '\n' (quebra de linha) do final da string gerado pelo fgets
    int len = strlen(destino);
    if(len > 0 && destino[len - 1] == '\n'){
        destino[len - 1] = '\0';
    }
}

//  USUARIOS 

Usuario* cadastrarUsuario(Usuario *lista, char *nome, char *email){
    Usuario *temp = lista;
    
    // Verifica na lista encadeada inteira se ja existe alguem com esse email ou nome 
    while(temp != NULL){
        if(strcmp(temp->email, email) == 0){
            printf("\n Erro: Email ja cadastrado!\n");
            return lista;
        }
        if(strcmp(temp->nome, nome) == 0){
            printf("\n Erro: Nome ja cadastrado!\n");
            return lista;
        }
        temp = temp->next;
    }

    // Aloca memoria, cria e preenche o novo usuario
    Usuario *novo = (Usuario*) malloc(sizeof(Usuario));
    strcpy(novo->nome, nome);
    strcpy(novo->email, email);
    novo->next = NULL;

    // Insercao na lista encadeada mantendo ordem alfabetica de email
    // Caso especial: Lista vazia ou novo email vem antes do primeiro elemento (head)
    if(lista == NULL || strcmp(novo->email, lista->email) < 0){
        novo->next = lista;
        printf("\n Usuario cadastrado com sucesso!\n");
        return novo;
    }

    // Navega ate encontrar a posicao correta para inserir sem quebrar a ordenacao
    Usuario *atual = lista;
    while(atual->next != NULL && strcmp(atual->next->email, novo->email) < 0){
        atual = atual->next;
    }

    // Reajusta os ponteiros para encaixar o novo no meio/fim
    novo->next = atual->next;
    atual->next = novo;

    printf("\n Usuario cadastrado com sucesso!\n");
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
    // Trecho gerado via IA para impedir a duplicidade de nomes.
    // Ele percorre a lista e bloqueia se achar o 'novoNome' em um email diferente do atual.
    Usuario *temp = lista;
    while(temp != NULL){
        if(strcmp(temp->nome, novoNome) == 0 && strcmp(temp->email, email) != 0){
            printf("\n Erro: Ja existe outro usuario cadastrado com o nome '%s'!\n", novoNome);
            return lista;
        }
        temp = temp->next;
    }

    Usuario *atual = buscarUsuarioPorEmail(lista, email);
    if(atual == NULL){
        printf("\n Usuario nao encontrado!\n");
        return lista;
    }
    
    strcpy(atual->nome, novoNome);
    printf("\n Nome atualizado com sucesso!\n");
    return lista;
}

Usuario* excluirUsuario(Usuario *lista, char *email, Livro *listaLivros){
    Usuario *atual = buscarUsuarioPorEmail(lista, email);
    if(atual == NULL){
        printf("\n Usuario nao encontrado!\n");
        return lista;
    }

    // Caso 1: O usuario a ser excluido e a head (primeiro no) da lista
    if(atual == lista){
        lista = lista->next; // A head passa a ser o proximo elemento
        free(atual);
        printf("\n Usuario excluido com sucesso!\n");
        return lista;
    }
    
    // Caso 2: O usuario esta no meio ou final da lista.
    // Precisamos de um ponteiro parando um no 'anterior' para poder pular o no excluido
    Usuario *anterior = lista;
    while(anterior->next != atual){
        anterior = anterior->next;
    }
    
    anterior->next = atual->next; // Pula o no atual e religa a lista
    free(atual); // Libera a memoria do usuario excluido
    
    printf("\n Usuario excluido com sucesso!\n");
    return lista;
}

void listarEmprestimosPorUsuario(Livro *listaLivros, char *email){
    Livro *atual = listaLivros;
    int encontrou = 0;

    printf("\n Livros com o usuario %s \n", email);
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

//  ARVORE DE IDS 

IdsArvore* inserirID(IdsArvore *arvore, int codigo){
    // Se chegou numa folha vazia, cria o no
    if(arvore == NULL){
        IdsArvore *novo = (IdsArvore*) malloc(sizeof(IdsArvore));
        novo->codigo = codigo;
        novo->left = NULL;
        novo->right = NULL;
        return novo;
    }

    // Recursao para organizar a Arvore Binaria de Busca (menores a esquerda, maiores a direita)
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
    // Libera a memoria da arvore em pos-ordem (esquerda, direita, raiz)
    if(arvore == NULL){
        return;
    }
    liberarArvore(arvore->left);
    liberarArvore(arvore->right);
    free(arvore);
}

// As funcoes 'encontrarMinimo' e 'removerID' foram geradas 100% via LLM (IA) 
// para lidar com a logica complexa de rebalanceamento e exclusao de nos da Arvore Binaria (BST).

// Função auxiliar para encontrar o nó com o menor código (usada na remoção)
IdsArvore* encontrarMinimo(IdsArvore* no) {
    IdsArvore* atual = no;
    while (atual && atual->left != NULL)
        atual = atual->left;
    return atual;
}

// Função para remover um ID da árvore binária e reajustá-la
IdsArvore* removerID(IdsArvore* raiz, int codigo) {
    if (raiz == NULL) return raiz;

    // Navega pela árvore até achar o código
    if (codigo < raiz->codigo) {
        raiz->left = removerID(raiz->left, codigo);
    } else if (codigo > raiz->codigo) {
        raiz->right = removerID(raiz->right, codigo);
    } else {
        // Encontrou o nó a ser removido

        // Caso 1 e 2: Nó com apenas um filho ou nenhum filho (folha)
        if (raiz->left == NULL) {
            IdsArvore* temp = raiz->right;
            free(raiz);
            return temp;
        } else if (raiz->right == NULL) {
            IdsArvore* temp = raiz->left;
            free(raiz);
            return temp;
        }

        // Caso 3: Nó com dois filhos
        // Pega o sucessor in-order (o menor valor da subárvore direita) para manter a estrutura ordenada
        IdsArvore* temp = encontrarMinimo(raiz->right);
        
        // Copia o valor do sucessor para o nó atual
        raiz->codigo = temp->codigo;
        
        // Remove o sucessor da subárvore direita, que agora duplicou
        raiz->right = removerID(raiz->right, temp->codigo);
    }
    return raiz;
}

//  LIVROS 

Livro* cadastrarLivro(Livro *lista, IdsArvore **arvore, char *titulo, char *autor, int ano){
    
    // Verifica se ja existe livro com mesmo titulo
    Livro *temp = lista;
    while(temp != NULL){
        if(strcmp(temp->titulo, titulo) == 0){
            printf("\n Erro: Livro com este titulo ja existe!\n");
            return lista;
        }
        temp = temp->next;
    }
    
    int codigo = proximoID;
    
    // Utiliza busca em Arvore Binaria (complexidade O(log n) no caso medio)
    // para garantir agilidade na validacao de IDs unicos, cumprindo o requisito extra do projeto.
    // Estruturado em code-review com IA.
    if (verificarID(*arvore, codigo) == 1) {
        printf("\n Erro critico: ID %d ja existe na arvore!\n", codigo);
        return lista;
    }
    
    proximoID = proximoID + 1;

    Livro *novo = (Livro*) malloc(sizeof(Livro));
    strcpy(novo->titulo, titulo);
    strcpy(novo->autor, autor);
    novo->ano = ano;
    novo->codigo = codigo;
    novo->status = 0; 
    strcpy(novo->email_usuario, ""); 
    novo->next = NULL;

    // Atualiza a estrutura auxiliar de busca rapida
    *arvore = inserirID(*arvore, codigo);

    printf("\n Livro cadastrado! Codigo gerado: %d\n", codigo);

    // Insere na lista ordenado por codigo crescente
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

    printf("\n Resultados para o autor: %s \n", autor);
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
    // 1. Verifica se o novo titulo ja pertence a OUTRO livro (refatoracao por IA)
    Livro *temp = lista;
    while(temp != NULL){
        // Se o titulo for igual, mas o codigo for diferente, significa que a duplicata é real
        if(strcmp(temp->titulo, novoTitulo) == 0 && temp->codigo != codigo){
            printf("\n Erro: Ja existe outro livro cadastrado com o titulo '%s'!\n", novoTitulo);
            return lista; // Cancela a edicao e retorna a lista intacta
        }
        temp = temp->next;
    }

    // 2. Se passou pela verificacao antiburla, faz a atualizacao normal
    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("\n Livro nao encontrado.\n");
        return lista;
    }

    strcpy(atual->titulo, novoTitulo);
    strcpy(atual->autor, novoAutor);
    atual->ano = novoAno;
    
    printf("\n Livro atualizado com sucesso!\n");
    return lista;
}

Livro* excluirLivro(Livro *lista, IdsArvore **arvore, int codigo){
    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("\n Livro nao encontrado.\n");
        return lista;
    }

    // Nao deixa excluir se estiver emprestado
    if(atual->status == 1){
        printf("\n Nao e possivel excluir: o livro esta emprestado.\n");
        return lista;
    }

    // Caso 1: Se for o primeiro da lista encadeada
    if(atual == lista){
        lista = lista->next;
        free(atual);
        
        // Mantem a integridade apagando o dado da arvore tambem (rebalanceando via recursao)
        *arvore = removerID(*arvore, codigo); 
        printf("\n Livro excluido com sucesso!\n");
        return lista;
    }

    // Caso 2: Procura no meio ou fim da lista e religa o ponteiro pulando o excluido
    Livro *anterior = lista;
    while(anterior->next != atual){
        anterior = anterior->next;
    }
    anterior->next = atual->next;
    free(atual);
    
    // Remocao sicronizada na arvore BST gerada por IA
    *arvore = removerID(*arvore, codigo); 

    printf("\n Livro excluido com sucesso!\n");
    return lista;
}

Livro* emprestarLivro(Livro *lista, int codigo, char *emailUsuario){
    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("\n Livro nao encontrado.\n");
        return lista;
    }

    if(atual->status == 1){
        printf("\n Livro ja esta emprestado para outro usuario.\n");
        return lista;
    }

    atual->status = 1;
    strcpy(atual->email_usuario, emailUsuario);
    
    printf("\n Livro emprestado com sucesso!\n");
    return lista;
}

Livro* devolverLivro(Livro *lista, int codigo){
    Livro *atual = buscarLivroPorCodigo(lista, codigo);
    if(atual == NULL){
        printf("\n Livro nao encontrado.\n");
        return lista;
    }

    if(atual->status == 0){
        printf("\n Este livro ja esta disponivel na biblioteca.\n");
        return lista;
    }

    atual->status = 0;
    strcpy(atual->email_usuario, "");
    
    printf("\n Devolucao realizada com sucesso!\n");
    return lista;
}


//  MEMORIA 

void liberarUsuarios(Usuario *lista){
    Usuario *atual = lista;
    while(atual != NULL){
        Usuario *proximo = atual->next; // Guarda o proximo no antes de destruir o atual
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


//  MENUS 

void menuCadastro(Usuario **listaUsuarios, Livro **listaLivros, IdsArvore **arvore){
    int opcao, ano;
    char nome[100], email[100], titulo[100], autor[100];

    do{
        printf("\n--- CADASTRO ---\n");
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
            
            // Loop para blindar entrada de dados invalidos. Refatorado via IA.
            while(ano < 0){ 
                printf("Ano invalido. Digite um ano valido Ex:(2026) : ");
                ano = lerInteiro();
            }

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
            printf("\n Opcao invalida!\n");
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
                    printf("\n Livro nao encontrado.\n");
                } else {
                    printf("\n Livro Encontrado \n");
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
            
            // Separa a leitura para exibir a mensagem correta ao usuario
            if(subOpcao == 1){
                printf("Digite o email: ");
                lerString(busca, sizeof(busca));
                u = buscarUsuarioPorEmail(listaUsuarios, busca);
            } 
            else if(subOpcao == 2){
                printf("Digite o nome: ");
                lerString(busca, sizeof(busca));
                u = buscarUsuarioPorNome(listaUsuarios, busca);
            } 
            else {
                printf("\n Opcao invalida!\n");
                continue; // Pula a impressao abaixo e volta para o menu
            }

            // Exibe o resultado da busca
            if(u == NULL){
                printf("\n Usuario nao encontrado.\n");
            } else {
                printf("\n Usuario Encontrado \n");
                printf("Nome: %s | Email: %s\n", u->nome, u->email);
            }
        }
        else if(opcao == 3){
            printf("Email do usuario: ");
            lerString(busca, sizeof(busca));
            listarEmprestimosPorUsuario(listaLivros, busca);
        }
        else if(opcao != 0){
            printf("\n Opcao invalida!\n");
        }

    } while(opcao != 0);
}

void menuAtualizacao(Usuario **listaUsuarios, Livro **listaLivros){
    int opcao, codigo, ano;
    char titulo[100], autor[100], email[100], nome[100];

    do {
        printf("\n--- ATUALIZACAO ---\n");
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
                printf("\nLivro nao encontrado.\n");
            } else {
                int campo = -1;
                while(campo != 0){
                    printf("\nLivro atual: %s | Autor: %s | Ano: %d\n", l->titulo, l->autor, l->ano);
                    printf("O que deseja alterar?\n");
                    printf("1. Titulo\n2. Autor\n3. Ano\n0. Concluir\nEscolha: ");
                    campo = lerInteiro();

                    if(campo == 1){
                        printf("Novo titulo: ");
                        lerString(titulo, sizeof(titulo));
                        *listaLivros = atualizarLivro(*listaLivros, codigo, titulo, l->autor, l->ano);
                        l = buscarLivroPorCodigo(*listaLivros, codigo); // atualiza o ponteiro
                    }
                    else if(campo == 2){
                        printf("Novo autor: ");
                        lerString(autor, sizeof(autor));
                        *listaLivros = atualizarLivro(*listaLivros, codigo, l->titulo, autor, l->ano);
                        l = buscarLivroPorCodigo(*listaLivros, codigo);
                    }
                    else if(campo == 3){
                        printf("Novo ano: ");
                        ano = lerInteiro();
                        while(ano <= 0){
                            printf("Ano invalido: ");
                            ano = lerInteiro();
                        }
                        *listaLivros = atualizarLivro(*listaLivros, codigo, l->titulo, l->autor, ano);
                        l = buscarLivroPorCodigo(*listaLivros, codigo);
                    }
                    else if(campo != 0){
                        printf("Opcao invalida!\n");
                    }
                } // Fim do while(campo != 0)
            } // Fim do else (livro encontrado)
        } // Fim do if(opcao == 1)
        
        else if(opcao == 2){
            printf("Email do usuario: ");
            lerString(email, sizeof(email));

            Usuario *u = buscarUsuarioPorEmail(*listaUsuarios, email);
            if(u == NULL){
                printf("Usuario nao encontrado!\n");
            } else {
                printf("\nUsuario atual: %s | Email: %s\n", u->nome, u->email);
                printf("Tem certeza que deseja editar? 1-Sim  2-Nao: ");
                int confirma = lerInteiro();

                if(confirma == 1){
                    printf("Novo nome: ");
                    lerString(nome, sizeof(nome));
                    *listaUsuarios = atualizarNomeUsuario(*listaUsuarios, email, nome);
                } else {
                    printf("Edicao cancelada.\n");
                }
            }
        } // Fim do else if(opcao == 2)
        
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
        opcao = lerInteiro();

        if(opcao == 1){
            printf("Codigo do livro: ");
            codigo = lerInteiro();

            Livro *l = buscarLivroPorCodigo(*listaLivros, codigo);
            if(l == NULL){
                printf("\nLivro nao encontrado.\n");
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

            Usuario *u = buscarUsuarioPorEmail(*listaUsuarios, email);
            if(u == NULL){
                printf("Usuario nao encontrado!\n");
            } else {
                Livro *temp = *listaLivros;
                int temLivro = 0;
                
                // Valida se o usuario tem pendencias antes de excluir da memoria
                while(temp != NULL){
                    if(temp->status == 1 && strcmp(temp->email_usuario, email) == 0){
                        temLivro = 1;
                        break;
                    }
                    temp = temp->next;
                }

                if(temLivro == 1){
                    printf("Usuario possui livro(s) emprestado(s). Exclusao bloqueada.\n");
                } else {
                    printf("\nUsuario: %s | Email: %s\n", u->nome, u->email);
                    printf("Confirma a exclusao? 1-Sim ou 2-Nao: ");
                    int confirma = lerInteiro();

                if(confirma == 1){
                        *listaUsuarios = excluirUsuario(*listaUsuarios, email, *listaLivros);
                    } else {
                        printf("\n[-] Cancelado.\n");
                    }
                }
            }
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
        printf("  +--------------------------------------+\n");
        printf("  |           /tmp/Bibioteca             |\n");
        printf("  +--------------------------------------+\n");
        printf("  | 1. Cadastro                          |\n");
        printf("  | 2. Consulta                          |\n");
        printf("  | 3. Atualizacao                       |\n");
        printf("  | 4. Exclusao                          |\n");
        printf("  | 5. Emprestimo                        |\n");
        printf("  | 6. Devolucao                         |\n");
        printf("  | 0. Sair                              |\n");
        printf("  +--------------------------------------+\n");
        printf("  -> Escolha uma opcao:"); opcao = lerInteiro();

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
            // Trecho refatorado com auxilio de IA (LLM) para implementar laços de validacao 
            // ('while') com rota de fuga ('0'), evitando a quebra abrupta para o menu anterior.
            printf("\n--- EMPRESTIMO ---\n");
            Usuario *u = NULL;

            // Mantém o loop até achar um usuário válido ou o usuário cancelar
            while (u == NULL) {
                printf("Email do usuario (ou '0' para cancelar): ");
                lerString(email, sizeof(email));

                if (strcmp(email, "0") == 0) {
                    printf("\n[-] Emprestimo cancelado.\n");
                    break; // Quebra o laco e volta para o menu
                }

                u = buscarUsuarioPorEmail(*listaUsuarios, email);
                
                if (u == NULL) {
                    printf("\n Erro: Usuario nao cadastrado. Tente novamente.\n");
                }
            }

            // Só avança para pedir o livro se o usuário foi encontrado (u != NULL)
            if (u != NULL) {
                printf("Codigo do livro: ");
                codigo = lerInteiro();
                
                // Impede que o programa tente buscar códigos inválidos (como letras/negativos)
                while (codigo <= 0) {
                    printf("\n Erro: Codigo invalido! Digite o numero do livro: ");
                    codigo = lerInteiro();
                }

                *listaLivros = emprestarLivro(*listaLivros, codigo, email);
            }
        }
        else if(opcao == 6){
            printf("\n--- DEVOLUCAO ---\n");
            int sucesso = 0; // Refatoracao com laço gerada via auxilio de IA

            while (sucesso == 0) {
                printf("Codigo do livro (ou '0' para cancelar): ");
                codigo = lerInteiro();

                if (codigo == 0) {
                    printf("\n[-] Devolucao cancelada.\n");
                    break; 
                }

                // Protege contra digitação de caracteres incorretos
                if (codigo < 0) {
                    printf("\n Erro: Codigo invalido! Digite o numero do livro.\n");
                    continue; 
                }

                // Armazena a referencia para testar se o livro existia
                Livro *l = buscarLivroPorCodigo(*listaLivros, codigo);
                
                *listaLivros = devolverLivro(*listaLivros, codigo);
                
                // Se o ponteiro nao e nulo, a operacao fluiu
                if (l != NULL) {
                    sucesso = 1; 
                }
            }
        }
    } while(opcao != 0);
    
    // Processo de limpeza de memoria sugerido em code review por IA. daqui ate 870 full gerado por ia 
    // Garante que todos os mallocs tenham seus devidos frees, evitando Memory Leak ao sair.
    liberarUsuarios(*listaUsuarios); 
    liberarLivros(*listaLivros);
    liberarArvore(*arvore);
    
    // Atribui NULL para garantir que ponteiros nao fiquem apontando para 'lixo' de memoria
    *listaUsuarios = NULL;
    *listaLivros = NULL;
    *arvore = NULL;
    
    printf("\n  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("  : Sistema de Biblioteca encerrado.                         :\n");
    printf("  :                                                          :\n");
    printf("  :    \"Eu nao mato a literatura, eu sou um sintoma          :\n");
    printf("  :      de um leitor que ja desistiu dela.\"                 : \n");
    printf("  :                       - resposta de uma IA,              :\n");
    printf("  :                        em debate com Leandro Karnal      :\n");
    printf("  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
}
