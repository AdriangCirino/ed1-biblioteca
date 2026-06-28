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
- Cadastrar usuário (inserção ordenada por email, sem duplicata) ok
- Buscar por email ok
- Buscar por nome ok
- Atualizar nome ok
- Excluir por email ok
- Listar livros emprestados por email ok

### Step 4 — BST de IDs (functions.c)
- Inserir ID na BST ok
- Verificar se ID existe ok
- Remover ID da BST ok
- Liberar BST ok

### Step 5 — Livros (functions.c)
- Cadastrar livro (ID sequencial automático, inserção ordenada por ID, verifica duplicata na BST) ok
- Buscar por código ok 
- Buscar por autor (retorna todos) ok
- Atualizar título/autor/ano ok
- Excluir por código (remove também da BST) ok
- Empréstimo (muda status + registra email) ok
- Devolução (muda status + limpa email) ok

### Step 6 — Menu (main.c)
- Menu principal ok 
- Submenus de cadastro, consulta, atualização, exclusão ok
- Fluxo de empréstimo e devolução ok 
- Liberar toda memória alocada ao sair ok 

### Step 7 — Testes
- Testar cada funcionalidade isoladamente ok 
- Testar fluxos completos (cadastrar livro → emprestar → devolver → excluir) ok
- Testar casos de erro (ID duplicado, usuário não encontrado, etc) ok 

### Step 8 — Revisão 
- Comentários nas partes importantes do código ok
- Comentários informando o uso de LLM ok
- Atualizar checklist do README ok 