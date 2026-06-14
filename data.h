#ifndef DATA_H // sugestao do Copilot para evitar erros com dois arquivos .h no mesmo projeto
#define DATA_H

//usuario
typedef struct Usuario {
    char nome[100];
    char email[100];
    struct Usuario *next;
} Usuario;

//livro
typedef struct Livro {
    char titulo[100];
    char autor[100];
    int ano;
    int codigo;
    int status; // 0 = disponivel, 1 = emprestado LEMBRAR DISSO NA IMPLEMENTACAO
    char email_usuario[100];
    struct Livro *next;
} Livro;

// arvore binaria auxiliar para ids
typedef struct IdsArvore {
    int codigo;
    struct IdsArvore *left;
    struct IdsArvore *right;
} IdsArvore;

#endif //esse endif foi sugerido pelo Copilot para evitar possiveis conflitos com mais de um arquivo .h