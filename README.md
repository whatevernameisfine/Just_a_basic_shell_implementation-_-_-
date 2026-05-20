# Minishell

A simplified Unix shell implemented in C, built as a project at 42 Antananarivo.  
The goal is to recreate a minimal but functional version of **bash**, meaning voluntarily reimplementing decades of terminal behavior just to understand why shells are held together by signals, pipes, and unresolved emotional trauma.

All of this was built from scratch on top of a custom `libft`, because apparently suffering through one standard library rewrite was not enough.

---

## Screenshots

![MShell](screenshots/Quieres.png)

---

## Description

**minishell** is an interactive command-line interpreter that replicates core behaviors of bash.

It reads user input through `readline`, parses and expands it, then executes the resulting commands — whether they are built-ins or external binaries discovered somewhere inside the dark abyss known as `$PATH`.

In simpler terms:
- user types questionable command,
- parser attempts to survive it,
- shell forks aggressively,
- and eventually something executes.

Usually.

---

## Supported features

| Feature | Details |
|---|---|
| **Prompt** | Displays `minishell$>` and patiently waits for chaos |
| **Command execution** | Runs external programs available in `$PATH` |
| **Pipelines** | Chain commands with `\|` because one process suffering alone is not enough |
| **Redirections** | Output: `>` (truncate), `>>` (append) — Input: `<` |
| **Here-document** | `<< DELIMITER` for inline input and temporary keyboard-based life choices |
| **Environment variables** | Expansion of `$VAR` and `$?` (the shell's emotional state encoded as an exit code) |
| **Quoting** | Single quotes preserve literal values, double quotes selectively betray them |
| **Signal handling** | `Ctrl-C` (new prompt), `Ctrl-D` (exit), `Ctrl-\` (ignored like a professional shell) |

---

## Built-in commands

| Command | Description |
|---|---|
| `echo` | Print text to stdout (supports `-n`) |
| `cd` | Change the current working directory and occasionally your sanity |
| `pwd` | Print the current working directory |
| `export` | Set or display environment variables |
| `unset` | Remove environment variables from existence |
| `env` | Display the current environment |
| `exit` | Exit the shell and temporarily reconnect with reality |
| `minifetch` | Easter egg — displays a stylized system info card because every shell deserves unnecessary features |

---

# Instructions

## Prerequisites

- A Unix-like operating system (Linux / macOS)
- `cc` (or any C compiler)
- `make`
- The **readline** library (`libreadline-dev` on Debian/Ubuntu, `readline` on macOS via Homebrew)
- A basic tolerance for debugging file descriptors at 2AM

---

## Compilation

Clone the repository and build with `make`:

```bash
git clone <repository-url> minishell
cd minishell
make
```

Available Makefile targets:

| Target | Description |
|---|---|
| `make` | Compile the project (and `libft`) |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and the binary |
| `make re` | Destroy and rebuild everything from scratch |

---

## Running

```bash
./minishell
```

The shell accepts no arguments.

Once launched, type commands at the `minishell$>` prompt just as you would in bash, except now every pipe, redirection, and signal handling issue becomes your personal responsibility.

---

## Usage examples

```bash
minishell$> echo "Hello, world!"
Hello, world!

minishell$> ls -la | grep Makefile
-rw-r--r-- 1 user user 1234 Jan 10 15:20 Makefile

minishell$> cat << EOF
> line one
> line two
> EOF
line one
line two

minishell$> export NAME=minishell && echo $NAME
minishell

minishell$> exit
```

---

## Authors

- **riramana**
- **andyandr**
