# Mini-Shell — Interpretador de Comandos em C++

## Objetivo do Programa

O **Mini-Shell** é um interpretador de comandos interativo de baixo nível, desenvolvido integralmente em linguagem C++. Seu propósito fundamental é demonstrar, na prática, os mecanismos primitivos de gerenciamento de processos de um sistema operacional — os mesmos que sustentam shells consagrados como Bash, Zsh e Fish.

Através de chamadas de sistema POSIX (fork, execvp, waitpid, chdir), o programa implementa um loop REPL (Read-Eval-Print Loop) que lê comandos do usuário, cria processos filhos isolados em memória para executá-los, aguarda sua conclusão e retorna o controle ao terminal — tudo sem depender de frameworks externos, apenas C++ puro e as APIs nativas do kernel.

## Stacks e Tecnologias

| Camada | Tecnologia |
|---|---|
| Linguagem | C++ ISO C++17 |
| APIs de Sistema | POSIX (fork, execvp, waitpid, chdir — via unistd.h, sys/wait.h, sys/types.h) |
| Build System | GNU Make |
| Testes | Python 3 com subprocess |
| Compilador | GCC (g++) 7+ ou Clang 6+ |
| Ambiente alvo | Linux, macOS, Windows com WSL |

## Especificações Técnicas

- **Paradigma**: Loop REPL síncrono (Read-Eval-Print Loop)
- **Entrada**: Leitura de linha via stdin com `std::getline`
- **Tokenização**: Parsing textual com `std::istringstream`, remoção de espaços extras
- **Conversão**: `std::vector<std::string>` para `char**` null-terminado (formato exigido por execvp)
- **Criação de processo**: `fork()` — clonagem do processo atual
- **Substituição de imagem**: `execvp()` — carrega binário do sistema no processo filho
- **Sincronização**: `waitpid()` — pai aguarda término do filho, evitando processos zumbis
- **Comando built-in (cd)**: `chdir()` — chamada de sistema direta no processo pai
- **Comando built-in (exit)**: Retorno false para encerrar o loop REPL
- **Gerenciamento de memória**: Alocação manual com `new[]` / `delete[]` para o array de argumentos C
- **Tratamento de erros**: Verificação de retorno em todas as syscalls com `std::strerror(errno)`

## Requisitos

### Hardware
- Processador x86_64 ou ARM64
- 64 MB de RAM (mínimo)
- 5 MB de espaço em disco

### Software
- Sistema operacional Linux, macOS ou Windows 11 com WSL (Ubuntu/Debian recomendado)
- Compilador GCC (g++) versão 7 ou superior, ou Clang versão 6 ou superior
- GNU Make (qualquer versão)
- Python 3.6 ou superior (apenas para testes automatizados)

## Dependências

O projeto não possui dependências externas. Todo o código utiliza exclusivamente:

- **Bibliotecas padrão C++**: `<iostream>`, `<string>`, `<vector>`, `<sstream>`, `<cstring>`, `<cerrno>`
- **APIs POSIX nativas**: `<unistd.h>`, `<sys/wait.h>`, `<sys/types.h>`

Nenhum pacote adicional precisa ser instalado além do compilador e do make.

## Arquitetura do Projeto

### Estrutura de Diretórios

```
.
├── main.cpp           # Loop principal REPL (ponto de entrada)
├── parser.hpp         # Cabecalho do modulo de tokenizacao
├── parser.cpp         # Implementacao: string -> tokens -> char** (para exec)
├── executor.hpp       # Cabecalho do modulo de execucao de processos
├── executor.cpp       # Implementacao: fork, execvp, waitpid, built-ins
├── Makefile           # Build system com C++17 e warnings habilitados
├── tests/
│   └── shell_tester.py  # Testes de integracao automatizados
├── .gitignore         # Arquivos ignorados pelo Git
├── LICENSE            # Licenca MIT
└── README.md          # Documentacao (este arquivo)
```

### Fluxograma da Arquitetura

```
+---------------------+
|     INICIO          |
| main(): REPL Loop   |
+----------+----------+
           |
           |  (loop infinito ate exit)
           v
+---------------------+
|  Exibe prompt:      |
|  "mini-shell> "     |
+----------+----------+
           |
           v
+---------------------+
|  Le entrada com     |
|  std::getline(cin)  |
+----------+----------+
           |
           v
+---------------------+
|  EOF ou erro? ------+---> [break] --> FIM
+----------+----------+
           |
           v
+---------------------+
|  Parser::tokenize() |
|  string -> vetor    |
+----------+----------+
           |
           v
+---------------------+
|  tokens.empty()? ---+---> [continue] --> proxima iteracao
+----------+----------+
           |
           v
+---------------------+
|  e built-in?        |
|  (cd / exit)        |
+----+----+-----------+
     |    |
    sim  nao
     |    |
     v    v
+--------+  +-----------------------------+
| Executa |  | Executor::execute_external()|
| built-in|  |                             |
| (chdir  |  |  +-------+                  |
| / exit) |  |  | fork() |                 |
+----+----+  |  +---+---+                  |
     |       |      |                       |
     v       |   +--+--+                    |
+--------+   |   |     |                    |
| exit?  +---+  filho  pai                  |
| (false)|  |   |     |                     |
+--------+  |   v     v                     |
     |      | execvp  waitpid               |
    sim     |   |     |                     |
     |      |   v     v                     |
     v      | sai   status                  |
    FIM     +-----------------------------+
                       |
                       v
             (proxima iteracao do loop)
```

## Instalação

### Linux (Ubuntu/Debian)

```bash
# Instalar dependencias de compilacao
sudo apt update
sudo apt install g++ make python3

# Clonar e compilar
git clone https://github.com/seu-usuario/Mini-Shell-Interpretador-de-Comandos-C.git
cd Mini-Shell-Interpretador-de-Comandos-C
make
```

### macOS

```bash
# Instalar Command Line Tools (ja inclui clang e make)
xcode-select --install

# Compilar
cd Mini-Shell-Interpretador-de-Comandos-C
make
```

### Windows 11 com WSL

```bash
# 1. Instale o WSL (caso ainda nao tenha):
#    wsl --install -d Ubuntu

# 2. Abra o terminal WSL e execute:
sudo apt update
sudo apt install g++ make python3

# 3. Navegue ate o diretorio do projeto (montado em /mnt/c/...)
cd /mnt/c/Users/seu-usuario/caminho/para/Mini-Shell
make
```

### Compilacao Manual (sem Make)

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp parser.cpp executor.cpp -o minishell
```

## Manual do Usuario

### Iniciando o Mini-Shell

```bash
./minishell
mini-shell>
```

O prompt `mini-shell>` indica que o interpretador esta aguardando um comando.

### Comandos Suportados

#### Comandos Externos (Binarios do Sistema)

Qualquer binario presente nas pastas do PATH pode ser executado:

```bash
mini-shell> ls -la
mini-shell> pwd
mini-shell> echo "Ola, mundo!"
mini-shell> uname -a
mini-shell> whoami
mini-shell> date
mini-shell> cat arquivo.txt
```

#### Comando Interno: cd

Altera o diretorio de trabalho atual. Por ser uma operacao que depende do processo atual, nao pode ser executada como subprocesso — e implementada via chamada de sistema `chdir()` diretamente no processo pai.

```bash
mini-shell> cd /tmp
mini-shell> pwd
/tmp

mini-shell> cd        # sem argumento: vai para $HOME
mini-shell> pwd
/home/usuario

mini-shell> cd ..
mini-shell> pwd
/home
```

#### Comando Interno: exit

Encerra o interpretador com codigo de retorno 0.

```bash
mini-shell> exit
```

### Tratamento de Erros

Comandos inexistentes geram mensagem de erro sem interromper a execucao:

```bash
mini-shell> comando_inexistente_123
comando_inexistente_123: No such file or directory
mini-shell>
```

Diretorios invalidos no comando cd:

```bash
mini-shell> cd /pasta_que_nao_existe
cd: /pasta_que_nao_existe: No such file or directory
mini-shell>
```

## Testes

O projeto inclui uma suite de 10 testes de integracao automatizados em Python.

### Executando os Testes

```bash
make
python3 tests/shell_tester.py
```

### Cobertura dos Testes

| # | Teste | Descricao |
|---|-------|-----------|
| 1 | Exit | Comando exit retorna codigo 0 |
| 2 | Entrada vazia | Apenas Enter nao causa erros |
| 3 | echo | Execucao de comando externo simples |
| 4 | ls | Listagem de diretorio |
| 5 | pwd | Exibicao do diretorio atual |
| 6 | cd e pwd | Mudanca de diretorio com cd .. |
| 7 | cd sem argumento | cd sem argumento vai para HOME |
| 8 | Comando inexistente | Comando invalido reporta erro sem crash |
| 9 | Comandos multiplos | Sequencia de comandos |
| 10 | Espacos extras | Entrada com whitespace excessivo |

### Exemplo de Saida dos Testes

```
[TESTE] Exit... OK
[TESTE] Entrada vazia... OK
[TESTE] echo... OK
[TESTE] ls... OK
[TESTE] pwd... OK
[TESTE] cd e pwd... OK
[TESTE] cd sem argumento... OK
[TESTE] Comando inexistente... OK
[TESTE] Comandos multiplos... OK
[TESTE] Espacos extras... OK

========================================
RESUMO: 10/10 testes passaram
========================================
```

## Casos de Uso

- **Estudo academico de Sistemas Operacionais**: Compreensao pratica dos mecanismos de criacao, isolamento e sincronizacao de processos via chamadas de sistema POSIX
- **Engenharia reversa de shells**: Entendimento do fluxo interno de interpretadores de comando como Bash, Zsh e Fish
- **Base para experimentos**: Projeto modular que serve como ponto de partida para implementar pipes, redirecionamento de E/S, job control e expansao de variaveis
- **Aprendizado de C++ de baixo nivel**: Gerenciamento manual de memoria, conversao entre estruturas C++ modernas e APIs C/POSIX

## Possiveis Extensoes Futuras

- **Redirecionamento de E/S**: Suporte a operadores `>`, `<`, `>>` para redirecionar stdin/stdout para arquivos
- **Pipes (|)**: Conectar a saida de um processo a entrada de outro
- **Job Control**: Execucao em background com `&`, comandos `jobs`, `fg`, `bg`
- **Expansao de variaveis**: Substituicao de `$VAR` e `$HOME` nos argumentos
- **Globbing**: Expansao de curingas como `*.txt` para lista de arquivos
- **Historico de comandos**: Navegacao com setas para cima/baixo via biblioteca readline
- **Autocomplete**: Sugestao de comandos e caminhos com a tecla TAB

## Licenca

Este projeto esta licenciado sob a licenca MIT. Consulte o arquivo LICENSE para mais detalhes.
