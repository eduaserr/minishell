<div>
    <img align="left" alt="minishell" src="https://github.com/eduaserr/42-project-badges/blob/a48e677fd4871e6999a9564101dca26091ec18ef/badges/minishelle.png" width="75px"><img align="right" alt="minishell" src="https://github.com/eduaserr/42-project-badges/blob/a48e677fd4871e6999a9564101dca26091ec18ef/badges/minishelln.png" width="75px">
    <h1 align="center"> ~MINISHELL~ <br><img alt="success 100/100" src="https://img.shields.io/badge/100%2F100-green?style=plastic&logoColor=green&label=success"></h1>
	
</div>
<p align="center">My own shell developed in C.<br></p>








<!--
---------
Funciones necesarias para el tratamiento de los nodos:
Creación de nodos:

Recomendable hacer malloc 1 vez y liberar cuadno sea necesario
t_env *create_env_node(char *env_var)
Crea un nuevo nodo a partir de una cadena env_var en formato KEY=VALUE.
Inicialización de la lista:

t_env *init_env_list(char **envp)
Inicializa la lista enlazada a partir del array envp de variables de entorno.
Búsqueda de nodos:


manejar intro en mshell | intro -> promp == NULL -> (!promp)
liberar intro !!!! free();

[39] : XMODIFIERS=@im=ibus

[56] : LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:


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
-->