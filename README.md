*This project has been created as part of the 42 curriculum by rpontici, faiello.*

# Minishell

> *As beautiful as a shell.*

## Description

`minishell` is a small POSIX-style command interpreter built from scratch in C
as a 42 cursus project. It reproduces a meaningful subset of `bash`: a REPL
prompt with command history, environment variable management, expansion,
quoting rules, redirections, here-documents, pipelines, signal handling, and a
collection of built-in commands. The goal of the project is to gain practical
mastery over processes, file descriptors, and the Unix execution model.

The shell is composed of the following pipeline:

1. **Lexer** — turns a raw input line into a linked list of typed tokens
   (words, pipes, redirections).
2. **Expansion** — substitutes `$VAR` and `$?` references outside single quotes.
3. **Quote stripping** — collapses single/double-quoted segments into literal
   text, applying the appropriate escape rules.
4. **Syntax check** — rejects malformed input (leading pipe, double pipe,
   redirection without target).
5. **Command builder** — groups tokens into a linked list of `t_cmd` nodes,
   one per pipeline segment, attaching redirection file descriptors and a
   builtin flag to each node.
6. **Execution** — forks child processes for external commands, wires the
   pipes, applies redirections, and runs builtins either in the parent (single
   command) or in the child (pipelines), then waits for completion.

A single global variable (`volatile sig_atomic_t g_signal`) is used, exactly
as the subject mandates, and stores only the signal number raised by the most
recent asynchronous interruption. Every other piece of mutable state lives
behind static accessor functions (`status_ref`, `env_ref`).

## Instructions

### Requirements

- GNU/Linux or macOS
- `cc` (GCC or Clang) with C99 support
- GNU `make`
- GNU `readline` development headers (`libreadline-dev` on Debian/Ubuntu,
  `brew install readline` on macOS)

### Build

```bash
make            # build minishell, libft and libftprintf
make clean      # remove object files
make fclean     # also remove the executable and library archives
make re         # full rebuild
```

The Makefile compiles all sources with `-Wall -Wextra -Werror` and avoids
unnecessary relinking. The bundled `libft` and `libftprintf` are compiled
through their own Makefiles.

### Run

```bash
./minishell
```

The shell starts in interactive mode and displays the `minishell$ ` prompt.
Press `Ctrl-D` to exit cleanly, or type `exit`.

## Features

### Built-in commands

| Command  | Notes                                                          |
|----------|----------------------------------------------------------------|
| `echo`   | Supports `-n`, including stacked forms such as `-nnn`.         |
| `cd`     | Accepts a relative or absolute path, `~`, `-`, or no argument. |
| `pwd`    | Prints the current working directory.                          |
| `export` | With no argument lists exported variables in `declare -x` form.|
| `unset`  | Removes one or more variables from the environment.            |
| `env`    | Prints the environment, hiding variables marked as unset-value.|
| `exit`   | Accepts an optional numeric status (`exit code & 0xFF`).       |

### Operators and expansion

- Pipes `|` chaining an arbitrary number of commands.
- Input `<`, output `>`, append `>>`, here-document `<<` (delimiter
  honours quoting to enable/disable expansion inside the body).
- Variable expansion `$VAR` and exit-status expansion `$?`.
- Single quotes `'…'` are fully literal; double quotes `"…"` allow `$`
  expansion and a small set of escapes (`\\`, `\"`, `\$`, `\n`, `\t`).

### Signals

| Key       | Mode       | Behaviour                                            |
|-----------|------------|------------------------------------------------------|
| `Ctrl-C`  | interactive| Displays a fresh prompt on a new line, sets `$?=130`.|
| `Ctrl-D`  | interactive| Exits the shell with the last exit status.           |
| `Ctrl-\`  | interactive| Ignored (no effect).                                 |

Child processes restore the default signal disposition before `execve`, so
`SIGINT` and `SIGQUIT` behave normally inside spawned programs.

## Source layout

```
include/minishell.h          Public types and prototypes
src/core/                    REPL entry point, signal setup, state accessors
src/lex/                     Tokenizer
src/quote/                   Quote stripping and escape handling
src/expand/                  Variable / exit-status expansion
src/parser/                  Syntax check, command builder, redirections, heredoc
src/exec/                    Pipeline orchestration, child setup, PATH lookup
src/env/                     Environment storage and manipulation
src/builtins/                Implementation of each shell built-in
src/utils/                   Memory cleanup helpers
libft/                       School-issued C library (re-used as authorised)
printf/                      In-house `ft_printf` / `ft_fprintf`
```

## Resources

Documentation and references consulted while building this project:

- `man bash`, in particular the sections about REDIRECTION, PIPELINES,
  EXPANSION and SHELL BUILTIN COMMANDS.
- GNU Readline manual: <https://tiswww.case.edu/php/chet/readline/readline.html>
- "Advanced Programming in the Unix Environment", W. Richard Stevens —
  chapters about process control, file descriptors and signal handling.
- POSIX.1-2017 Shell Command Language specification:
  <https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html>
- Linux man-pages: `readline(3)`, `execve(2)`, `fork(2)`, `pipe(2)`,
  `dup2(2)`, `waitpid(2)`, `sigaction(2)`.

### Use of AI

AI assistance (Claude Code) was used as a pair-programming aid in three
specific ways:

1. **Brainstorming the architecture.** We asked the model to suggest a clean
   separation of concerns (lexer → expand → quote-strip → syntax-check →
   build → exec) and we then reviewed the proposal together before adopting it.
2. **Norminette compliance reviews.** After each significant change we ran
   `norminette` ourselves and used the model to discuss how to break down
   functions that exceeded the 25-line / 5-variable / 4-functions-per-file
   limits. Every refactor was reviewed line by line before committing.
3. **Documentation polish.** The structure and wording of this README and of
   the project ARCHITECTURE notes were drafted with model assistance, then
   edited by hand.

No piece of code was accepted without understanding what it does and being
able to defend it in evaluation: AI was a sparring partner, never the author
of record.

## Authors

- **rpontici** — 42 student
- **faiello** — 42 student
