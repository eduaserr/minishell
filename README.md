<div>
    <img align="left" alt="minishell" src="https://github.com/eduaserr/42-project-badges/blob/a48e677fd4871e6999a9564101dca26091ec18ef/badges/minishelle.png" width="75px"><img align="right" alt="minishell" src="https://github.com/eduaserr/42-project-badges/blob/a48e677fd4871e6999a9564101dca26091ec18ef/badges/minishelln.png" width="75px">
    <h1 align="center"> ~MINISHELL~ <br><img alt="success 100/100" src="https://img.shields.io/badge/100%2F100-green?style=plastic&logoColor=green&label=success"></h1>
	
</div>
<p align="center">My own shell developed in C.<br></p>

### PROJECT STRUCTURE
```
minishell/
├── Makefile
├── main.c
├── inc/
│   └── minishell.h
├── lib/
│   └── libft/
│       └── [archivos de libft]
└── src/
    ├── builtins/
    │   ├── builtin.c
    │   ├── cd.c
    │   ├── cd_utils.c
    │   ├── echo.c
    │   ├── env.c
    │   ├── env_utils.c
    │   ├── exit.c
    │   └── pwd.c
    ├── command/
    │   └── cmd.c
    ├── executor/
    │   ├── execute.c
    │   ├── heredoc.c
    │   ├── heredoc_utils.c
    │   ├── pipes.c
    │   ├── redir.c
    │   └── redir_utils.c
    ├── init/
    │   ├── init.c
    │   ├── init_cmd.c
    │   ├── init_env.c
    │   ├── init_redir.c
    │   └── init_tkn.c
    ├── parser/
    │   ├── parser.c
    │   └── promp.c
    ├── quotes/
    │   ├── quotes_check.c
    │   ├── quotes_expand.c
    │   └── quotes_utils.c
    ├── redirs/
    │   └── redirs.c
    ├── signals/
    │   ├── sig_init.c
    │   └── signal2.c
    ├── tokens/
    │   └── tokens.c
    └── utils/
        ├── utils.c
        ├── utils_error.c
        ├── utils_free.c
        ├── utils_print.c
        └── utils_split.c

```

### Organización por módulos:
📁 **Archivos principales**
- [main.c](https://github.com/eduaserr/minishell/blob/main/main.c) - Función principal y bucle del shell
- [minishell.h](https://github.com/eduaserr/minishell/blob/main/inc/minishell.h) - Header con todas las definiciones.

📁 **[src/builtins/](https://github.com/eduaserr/minishell/blob/main/src/builtins)**
- Implementación de comandos internos (cd, echo, env, exit, pwd, export, unset).

📁 **[src/command/](https://github.com/eduaserr/minishell/blob/main/src/command)**
- Gestión y parseo de comandos.

📁 **[src/executor/](https://github.com/eduaserr/minishell/blob/main/src/executor)**
- Ejecución de comandos, manejo de pipes, redirecciones y heredocs.

📁 **[src/init/](https://github.com/eduaserr/minishell/blob/main/src/init)**
- Inicialización de estructuras (shell, env, tokens, comandos, redirecciones).

📁 **[src/parser/](https://github.com/eduaserr/minishell/blob/main/src/parser)**
- Parseo del input del usuario y generación del prompt.

📁 **[src/quotes/](https://github.com/eduaserr/minishell/blob/main/src/quotes)**
- Manejo de comillas (validación, expansión, utilidades).

📁 **[src/redirs/](https://github.com/eduaserr/minishell/blob/main/src/redirs)**
- Gestión de redirecciones de entrada y salida.

📁 **[src/signals/](https://github.com/eduaserr/minishell/blob/main/src/signals)**
- Gestión de señales (SIGINT, SIGQUIT, etc.).

📁 **[src/tokens/](https://github.com/eduaserr/minishell/blob/main/src/tokens)**
- Tokenización del input.

📁 **[src/utils/](https://github.com/eduaserr/minishell/blob/main/src/utils)**
- Funciones auxiliares (errores, liberación de memoria, impresión, split).

📁 **[lib/libft/](https://github.com/eduaserr/minishell/blob/main/lib/libft)**
- Biblioteca de funciones personalizadas con ft_printf y get_next_line.

Esta estructura modular facilita el mantenimiento y desarrollo del proyecto, separando claramente las responsabilidades de cada componente.


## CÓDIGOS DE EXIT ESTÁNDAR

| Código | Significado | Ejemplo |
|--------|-------------|---------|
| 0 | Éxito | `echo hello` |
| 1 | Error general | `cd directorio_inexistente` |
| 2 | Uso incorrecto de builtin | Error de sintaxis |
| 126 | Comando no ejecutable | Permisos denegados |
| 127 | Comando no encontrado | `comando_inexistente` |
| 128+n | Señal fatal | Ctrl+C = 130 (128+2) |
| 130 | Ctrl+C (SIGINT) | Interrumpir comando |
| 131 | Ctrl+\ (SIGQUIT) | Quit signal |

### ESTRUCTURA DE DATOS

Después del parsing, la estructura de comandos queda así:
```c

❗ Ejemplo: ls -l -a > file.txt

cmd->args[0] = "ls"
cmd->args[1] = "-l"
cmd->args[2] = "-a"
cmd->args[3] = NULL
cmd->redirs->type = REDIR_OUT
cmd->redirs->file = "file.txt"
```

### COMPILACIÓN Y USO

```bash
make
./minishell
```

### CARACTERÍSTICAS IMPLEMENTADAS

- ✅ Prompt interactivo
- ✅ Historial de comandos
- ✅ Ejecución de ejecutables
- ✅ Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- ✅ Redirecciones: `<`, `>`, `>>`
- ✅ Heredocs: `<<`
- ✅ Pipes: `|`
- ✅ Variables de entorno: `$VAR`
- ✅ Exit status: `$?`
- ✅ Expansión de comillas simples y dobles
- ✅ Manejo de señales: Ctrl+C, Ctrl+D, Ctrl+\

---
*Proyecto desarrollado como parte del curriculum de 42.*