#ifndef DATA_H // sugestao do Copilot para evitar erros com dois arquivos .h no mesmo projeto
#define DATA_H

typedef struct Usuario {
    char nome[100];
    char email[100];
    struct Usuario *next;
} Usuario;

typedef struct Livro {
    char titulo[100];
    char autor[100];
    int ano;
    int codigo;
    int status; // 0 = disponivel, 1 = emprestado
    char email_usuario[100];
    struct Livro *next;
} Livro;

// Arvore binaria para os IDs
typedef struct IdsArvore {
    int codigo;
    struct IdsArvore *left;
    struct IdsArvore *right;
} IdsArvore;

#endif // DATA_H