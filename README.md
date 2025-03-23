<div>
    <img align="left" alt="minishell" src="https://github.com/eduaserr/42-project-badges/blob/a48e677fd4871e6999a9564101dca26091ec18ef/badges/minishelle.png" width="75px"><img align="right" alt="minishell" src="https://github.com/eduaserr/42-project-badges/blob/a48e677fd4871e6999a9564101dca26091ec18ef/badges/minishelln.png" width="75px">
    <h1 align="center"> ~MINISHELL~ <br><img alt="success 100/100" src="https://img.shields.io/badge/100%2F100-green?style=plastic&logoColor=green&label=success"></h1>
	
</div>
<p align="center">My own shell developed in C.<br></p>








---------
Funciones necesarias para el tratamiento de los nodos:
Creación de nodos:

t_env *create_env_node(char *env_var)
Crea un nuevo nodo a partir de una cadena env_var en formato KEY=VALUE.
Inicialización de la lista:

t_env *init_env_list(char **envp)
Inicializa la lista enlazada a partir del array envp de variables de entorno.
Búsqueda de nodos:

t_env *find_env_key(t_env *lstenv, const char *key)
Busca un nodo en la lista que coincida con una clave específica (key).
Añadir nodos:

void add_env_node(t_env **lstenv, const char *key, const char *value)
Añade un nuevo nodo al final de la lista con la clave y el valor proporcionados.
Actualizar nodos:

void update_env_node(t_env *lstenv, const char *key, const char *value)
Actualiza el valor de un nodo existente con la clave proporcionada.
Añadir o actualizar nodos:

void add_or_update_env(t_env **lstenv, const char *key, const char *value)
Añade un nuevo nodo si la clave no existe o actualiza el valor si la clave ya está presente.
Eliminar nodos:

void remove_env_key(t_env **lstenv, const char *key)
Elimina un nodo de la lista que coincida con una clave específica.
Liberar la lista:

void ft_free_env(t_env *lstenv)
Libera toda la memoria asociada a la lista enlazada.
Recorrer la lista:

void print_env_list(t_env *lstenv)
Recorre la lista y muestra todas las claves y valores (útil para depuración)

---------