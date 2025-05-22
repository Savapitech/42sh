# 🐚 42sh – A Unix Shell Inspired by TCSH

**42sh** is a custom Unix shell written in C as part of the EPITECH curriculum.
Inspired by `tcsh`, this project aims to recreate a powerful, interactive command-line interface, complete with many built-in features, scripting capabilities, and user-friendly enhancements.

## ✨ Features

- **Built-in commands**:
  - `cd`, `echo`, `exit`, `alias`, `setenv`, `unsetenv`, `env`, `printenv`, `which`, `where`, `repeat`, `yes`, `termname`, `expr`, and more...
- **Redirections**:
  - Input/output redirection support: `>`, `<`, `>>`, `<<` (here-documents)
- **Pipelines**:
  - Command chaining using pipes (`|`)
- **Globbing**:
  - Wildcard expansion (`*`, `?`, etc.)
- **Job control**:
  - Background job handling with `&` `fg` `bg` `Ctrl+Z`
- **History**:
  - Command history navigation using arrow keys
- **Interractive reverse search**:
  - Search in the history with `Ctrl+R`
- **Line editing**:
  - Interactive line editing and cursor movement
- **Auto completion**:
  - Interactive auto completion with `TAB`
- **Git integration**:
  - Dynamic prompt with branch name, ahead and behind indicator
- **Alias system**:
  - Define and expand custom aliases
- **Environment variables**:
  - Manage the shell’s environment
- And more...

## ⚙️ Build & Run

```bash
git clone https://github.com/Savapitech/42sh.git
cd 42sh
make
./42sh
```
- Requires a POSIX-compliant system and a C compiler (tested with gcc on Linux).

## 📁 Project Structure

```txt
42sh/
├── include/            # Header files
├── src/                # Shell source code
├── ulib/               # Custom utility libraries
├── validation_tests.py # Functional tests
├── Makefile            # Build system
└── README.md           # You're reading it!
```

## 🧠 Concepts & Architecture

- **Lexer/Parser**: Tokenizes user input into an Abstract Syntax Tree (AST)
- **Executor**: Traverses the AST to execute commands
- **Builtins**: Custom implementation of standard shell commands

## 🧪 Testing

You can run the test suite using:
```bash
make tests_run
```
(Requires `python3`)

## ❄️ Nix Support
This project provides full support for Nix:
- You can run the shell directly with:
```bash
nix run github:Savapitech/42sh
```
- If you are working on the project, a development environment is available via the flake:
```bash
nix develop
```
This gives you access to a reproducible devShell with all required dependencies.


## 🛠️ Development Notes

Written in **C99**

Follows the **EPITECH coding style**

No memory leaks (only if no errors occured)

Handles signals (e.g., `Ctrl+C`, `Ctrl+D`)

Compatible with **tcsh-style** interactive behavior

## 📜 License

This project is part of the Epitech curriculum and is shared for educational purposes.

## 👥 Authors

[@savalet](https://github.com/savalet), [@tekClovis](https://github.com/tekClovis), [@Arthur-gtr](https://github.com/Arthur-gtr), [@Titouanhct](https://github.com/Titouanhct), [@eiffis](https://github.com/eiffis)

![CI](https://github.com/Savapitech/42sh/actions/workflows/main.yml/badge.svg)
