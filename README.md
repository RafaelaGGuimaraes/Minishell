This project was created as part of the 42 curriculum by rgomes-g, grodrig2.

Minishell

📌 Description

    Minishell is a simplified UNIX shell developed in C, as part of the 42 São Paulo curriculum.

    The goal of this project is to recreate the core behavior of a shell like Bash, focusing on command parsing, process execution, and environment management, while respecting constraints such as memory management, signal handling, and modular architecture.

    This implementation supports command execution, pipes, redirections, environment variable expansion, and built-in commands.

    The project was developed without bonus features (no &&, ||, or wildcards).

⚙️ Instructions

Compilation

    make

Execution

    ./minishell

Cleaning

    make clean
    make fclean
    make re

🧠 How it works

    The shell follows this execution pipeline:

    readline → tokenize → syntax_check → parse → execute

1. Input
    User input is read using readline
    Signals are handled (Ctrl+C, Ctrl+D, Ctrl+)

2. Tokenization (Lexer)
    The input is split into a linked list of t_token
    Each token contains:
    value
    type (WORD, PIPE, REDIR, etc.)
    quoted (NONE, SINGLE, DOUBLE)

    Tokens are temporary and are freed before execution.

3. Syntax Check
    Validates:
    Invalid pipe usage
    Missing files after redirections
    Unclosed quotes
    Empty input

4. Parsing
    Transforms tokens into executable structures

    Core structures:

        t_cmd → command (args + redirections + next)
        t_redir → redirection list
        t_shell → global shell state

        The result is a linked list of t_cmd, which is passed to the executor.

5. Expansion

    Handled by:
        expand(char *str, t_quote_type quoted, t_shell *shell)

    Rules:
        QUOTE_SINGLE → no expansion
        QUOTE_DOUBLE → expand variables
        QUOTE_NONE → expand variables

    Supports:
        $VAR
        $?

6. Execution
    The executor works only with t_cmd
    Tokens are not used at this stage

    Execution behavior:
        Single command → executed directly
        Multiple commands → pipeline using fork and pipe

    Each process:
        Applies redirections
        Executes:
            Builtin functions
            External commands via execve

🧱 Project Structure

    minishell/
    ├── Makefile
    ├── main.c
    │
    ├── includes/
    │ └── minishell.h
    │
    ├── libft/
    │
    ├── src/
    │ ├── parsing/
    │ │ ├── lexer.c
    │ │ ├── parser.c
    │ │ ├── expand.c
    │ │ └── syntax.c
    │ │
    │ ├── execution/
    │ │ ├── executor.c
    │ │ ├── redirs.c
    │ │ └── path.c
    │ │
    │ ├── builtins/
    │ │ └── builtins.c
    │ │
    │ └── shared/
    │ ├── env.c
    │ ├── utils.c
    │ └── signals.c

⚠️ Important rules
    Tokens are temporary and never reach the executor
    Executor only uses t_cmd
    Only one global variable is allowed:

        extern int g_signal;

    minishell.h is treated as a strict contract:
    Structs and enums must not be modified

⚠️ Limitations
No support for:
&&
||
Wildcards (*)

📚 Resources
Documentation
    man bash
    man 2 fork
    man 2 execve
    man 2 pipe
    man 2 dup2
    man 3 readline
Learning materials
    42 subject and intra resources
    UNIX process and file descriptor documentation

🤖 AI Usage
AI tools were used as support during development:

    Concept clarification:
    fork, execve, pipes, signals
    Debugging:
    edge cases and logic issues
    Documentation:
    structuring this README

    All code was written and understood by the authors.

👩‍💻 Authors
Rafaela Guimarães
Gabriel Pestana