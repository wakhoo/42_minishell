# 42 minishell with bonus
  > minishell is a 42 school team project to recreate our own simple shell based on Bash.
> We need to recreate: cd, pwd, unset, export, env, echo, exit
  >
> Also we have bonus part to get more points on the project.
  >
> && and || with parenthesis for priorities and wildcard(*)

## Grade
Validated: **20/02/2025**

Grade: ✅ **125%** ✅ (Full Score)

![Grade](https://github.com/wakhoo/42_minishell/blob/main/assets/Screenshot%202025-07-21%20at%2011-40-19%20Intra%20Projects%20minishell.png?raw=true)

## Minishell is structured into several independent modules to make the shell

- **Lexer**: Splits the user input into tokens while respecting quotes.
- **Parser**: Builds an AST (Abstract Syntax Tree) that supports priorities,
  parentheses, `&&`, `||`, pipes, and redirections.
  > In this project we doesn't use the exact word Lexer and Parser as I didn't know the key words at the moment, but still the logic was quite simliar.
- **Executor**: Executes the AST using a recursive strategy and handles
  pipelines, logical operators, and command grouping.
- **Environment Manager**: Stores and updates environment variables using a
  custom linked-list structure.
- **Builtins**: Our own implementation of cd, pwd, export, unset, env, echo, exit.
- **Error Handling**: Centralized error reporting with consistent messages and
  exit codes.
- **Memory Management**: Full cleanup of tokens, AST nodes, and environment
  structures to avoid leaks (validated with valgrind).

## Why these design decisions?

- **Readline** was chosen for a smoother user experience (history navigation,
  editable prompt, Ctrl-D handling).
- **Recursive AST execution** allows clean support for logical operators
  (`&&`, `||`) and parentheses.
- **Separate lexer → parser → executor** makes debugging easier and follows
  real-shell architecture.
- **Linked lists** were used for environment variables to efficiently insert,
  delete, and update elements without reallocations.
- **Custom signal handling** replicates Bash behavior for Ctrl-C and Ctrl-\.
- **No global variables** (except where the subject allows), improving testability
  and readability.

## Compile

### Requirements
- Linux (tested on Ubuntu 22+)
- readline library

### Install readline
sudo apt install libreadline-dev

### Build
```
make        # normal build

make bonus  # build with &&, ||, parentheses, wildcard

make clean  # remove objects
```

### Run
./minishell

## Demo
[![Minishell Demo](https://github.com/wakhoo/42_minishell/blob/main/assets/IMG_1209.jpg?raw=true)](https://youtu.be/BffJ4qGTwbY?si=FAUldBPeFqbBJ3CN)

## Subject

[Subject pdf](https://github.com/wakhoo/42_minishell/blob/main/minishell.pdf)

## Author

**chajeon(Chaewon JEON) and dancel, 42 Mulhouse students**
* Github: [@chajeon](https://github.com/wakhoo)
* Github: [@dancel](https://github.com/Tht4-x)
