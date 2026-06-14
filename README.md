# ed1-biblioteca
Trabalho final da matéria de estrutura de dados 1 
Mat: 20240017215
## Plano de Implementação
// esse steo by step foiu gerado por ia 
### Step 1 — Structs (data.h)
- Struct Usuario: nome, email ok 
- Struct NoUsuario: usuario + ponteiro next ok 
- Struct Livro: titulo, autor, ano, codigo, status, email_usuario ok 
- Struct NoLivro: livro + ponteiro next ok 
- Struct NoBST: codigo + ponteiros left/right ok 
- Typedef das listas e da BST ok 

### Step 2 — Protótipos (functions.h)
- Declarar todas as funções de usuário, livro, BST e menu ok 

### Step 3 — Usuários (functions.c)
- Cadastrar usuário (inserção ordenada por email, sem duplicata)
- Buscar por email
- Buscar por nome
- Atualizar nome
- Excluir por email
- Listar livros emprestados por email

### Step 4 — BST de IDs (functions.c)
- Inserir ID na BST
- Verificar se ID existe
- Remover ID da BST
- Liberar BST

### Step 5 — Livros (functions.c)
- Cadastrar livro (ID sequencial automático, inserção ordenada por ID, verifica duplicata na BST)
- Buscar por código
- Buscar por autor (retorna todos)
- Atualizar título/autor/ano
- Excluir por código (remove também da BST)
- Empréstimo (muda status + registra email)
- Devolução (muda status + limpa email)

### Step 6 — Menu (main.c)
- Menu principal
- Submenus de cadastro, consulta, atualização, exclusão
- Fluxo de empréstimo e devolução
- Liberar toda memória alocada ao sair

### Step 7 — Testes
- Testar cada funcionalidade isoladamente
- Testar fluxos completos (cadastrar livro → emprestar → devolver → excluir)
- Testar casos de erro (ID duplicado, usuário não encontrado, etc)

### Step 8 — Revisão 
- Comentários nas partes importantes do código
- Comentários informando o uso de LLM
- Atualizar checklist do README
