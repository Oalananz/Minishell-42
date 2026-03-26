# minishell

This is a small Unix shell implementation (a 42-school style "minishell"). It implements a subset of typical shell features: parsing and tokenizing input, environment management, built-in commands, basic redirections, here-documents, and simple pipeline execution.

This README documents the project structure, features, build and run instructions, known limitations, and pointers to the main source files.

## Features

- Interactive prompt with readline support and history.
- Signal handling for Ctrl-C and ignored Ctrl-\ (SIGQUIT).
- Command tokenization and parsing (quotes, escapes, and simple operators).
- Environment variable handling and expansion (including in here-documents when appropriate).
- Built-in commands: `cd`, `pwd`, `echo`, `env`, `export`, `unset`, `exit`.
- Execution of external commands using fork/exec, with simple pipeline support.
- Redirections: input (`<`), output (`>`), append (`>>`), and here-doc (`<<`).
- Basic memory and resource cleanup between loop iterations.

## Architecture

At a high level, the shell follows a classic pipeline:

1. **Read input** (interactive loop using `readline`)
2. **Tokenize** (split into tokens while respecting quotes/operators)
3. **Parse** (turn tokens into command structures / AST-like representation)
4. **Expand** (environment variables, quote-aware expansion, heredoc expansion rules)
5. **Execute** (builtins in-process when applicable; otherwise fork/exec + pipes + redirections)
6. **Cleanup** (free per-iteration resources, close FDs, unlink heredoc temp files)

Main “modules” and responsibilities:

- **Tokenizer (`tokenizer/`)**: Converts the raw line into tokens while handling quotes and special characters/operators.
- **Parser (`parser/`)**: Classifies tokens and builds the command representation for execution (including pipeline segments).
- **Expander (`expander/`)**: Applies `$VAR` expansion rules and quote-aware substitution.
- **Execution (`execution/`)**: Sets up redirections and pipes, spawns processes, manages exit status, and handles here-documents.
- **Builtins (`built-in/`)**: Implements internal commands executed without `execve` when possible.
- **Signals (`signals.c`)**: Configures the interactive signal behavior to match typical shell UX.

## How to run

### Requirements

- POSIX-like environment (Linux/macOS).
- GCC or Clang for building C sources.
- Make (uses the provided `Makefile`).
- The code uses the readline library; ensure `readline` (development headers) are installed. On Debian/Ubuntu: `libreadline-dev`.

### Build

From the project root run:

```sh
make
```

To clean build artifacts:

```sh
make fclean
```

### Run

Start the shell by running the produced binary from the repository root:

```sh
./minishell
```

The prompt will appear as:

	Arab Spring 🐣🐥 ->

Type commands as you would in a normal shell. Use Ctrl-D to exit (EOF), and Ctrl-C to interrupt the current input (sets exit status 130).

## Project layout

Key files and directories (top-level):

- `main.c` - program entry point, main loop, initialization, and basic executor dispatch for builtins.
- `main_utils.c` - helpers for freeing resources, handling EOF (Ctrl-D) and per-loop cleanup.
- `signals.c` - signal handlers for SIGINT and SIGQUIT.
- `Headers/minishell.h` - project-wide types, structs and prototypes.
- `tokenizer/` - tokenization logic: splitting the prompt into tokens while handling quotes and special characters.
- `parser/` - parser implementation that detects token types and prepares commands for execution.
- `expander/` - variable expansion, dollar handling and quote-aware expansion.
- `execution/` - command execution, redirection handling, pipeline creation, child process management, and here-doc support.
- `built-in/` - implementations of shell builtins (`cd`, `pwd`, `env`, `export`, `unset`, `exit`, `echo` etc.).
- `libft/` - small helper library used across the project (string utilities, lists) and `ft_printf`, `get_next_line` implementations.

Subdirectories under `execution/`:
- `HereDoc/` - here-document helpers and expansion handling.
- `redirection/` - functions to open files and set up redirections.

Examples of important source files:
- Builtins: `built-in/cd.c`, `built-in/pwd.c`, `built-in/unset.c`, `built-in/exit/exit.c`, `built-in/export/export.c`, `built-in/echo/echo_command.c`.
- Execution: `execution/execution.c`, `execution/child_process.c`, `execution/redirect_utils.c`, `execution/open_files.c`.
- Tokenizer/Parser: `tokenizer/tokenizer.c`, `parser/parser.c`, `tokenizer/token_utils.c`.

## Built-ins

The shell provides implementations for the following builtins. These are executed internally and do not spawn a new process:

- `cd [dir]` - change working directory.
- `pwd` - print current working directory.
- `echo [-n] [args...]` - display arguments to stdout; supports `-n` to omit newline.
- `env` - print the current environment variables.
- `export [KEY=VALUE]` - add or modify environment variables.
- `unset KEY` - remove an environment variable.
- `exit [n]` - exit the shell with status `n` (or last exit status if omitted).

Builtin source code lives under `built-in/` and its subfolders. See `main.c` for how builtins are detected and dispatched.

## Signals

- Ctrl-C (SIGINT) interrupts the current input and sets the global `g_signal` and the shell exit status (130).
- Ctrl-\ (SIGQUIT) is ignored.

Signal handling is implemented in `signals.c` and is wired up during initialization.

## Testing and usage tips

- Test pipelines and redirections: `ls -l | grep src > out.txt`.
- Test here-documents: `cat << EOF` followed by lines and `EOF` on its own line.
- Test environment variable expansion: `echo $HOME` or `echo "$PATH"`.
- Use `export` to add variables used by child processes.

Edge cases to be mindful of (known complexity areas):

- Complex parsing with nested quotes or mixed quoting and escaping may have subtle differences from bash.
- Some positional parameter and subshell features are out of scope.
- Advanced job control (background `&`, fg/bg) and command substitution are not implemented (unless added elsewhere in the code).

## What I learned

- **Lexing/parsing is most of the work:** handling quotes, operators, and edge cases is where shells get tricky.
- **Process orchestration fundamentals:** pipes, redirections, `fork()`, `execve()`, and exit-status propagation are the core building blocks of a shell.
- **Signals & interactive UX:** getting Ctrl-C/Ctrl-\ behavior to feel “bash-like” requires careful signal setup and restoring terminal state.
- **File descriptor discipline:** correct `dup2()` usage and closing unused FDs prevents deadlocks/leaks in pipelines.
- **Memory/resource cleanup patterns:** freeing per-loop allocations and un-linking heredoc temp files keeps the shell stable over long sessions.
- **Separation of concerns matters:** splitting responsibilities into tokenizer → parser → expander → executor made debugging and extending the project far easier.

## Sequence Diagram

The following Mermaid diagram shows **how minishell processes a command**:

```mermaid
sequenceDiagram
    participant User
    participant Minishell
    participant Tokenizer
    participant Parser
    participant Expander
    participant Executor
    participant Builtin/External

    User->>Minishell: Type command
    Minishell->>Tokenizer: Tokenize input
    Tokenizer->>Parser: Send tokens
    Parser->>Expander: Expand variables
    Expander->>Executor: Prepare commands and arguments
    Executor->>Builtin/External: Execute command
    Builtin/External-->>Executor: Return status/output
    Executor-->>Minishell: Return status/output
    Minishell-->>User: Display output
```

### Contributors
- [@Oalananz](https://github.com/Oalananz)
- [@Qhatahet](https://github.com/Qhatahet)
