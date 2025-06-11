#!/bin/bash

# Colores para output más claro
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función para mostrar mensajes con color
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Función para mostrar ayuda
show_help() {
    echo "Uso: $0 [opciones]"
    echo ""
    echo "Opciones:"
    echo "  -h, --help     Mostrar esta ayuda"
    echo "  -m, --message  Especificar mensaje de commit directamente"
    echo "  -f, --files    Especificar archivos a añadir (separados por espacios)"
    echo "  --no-clean     Omitir make fclean"
    echo "  --multi        Modo múltiples commits (interactivo)"
    echo ""
    echo "Ejemplos:"
    echo "  $0                                    # Modo interactivo un commit"
    echo "  $0 -m \"Fix bug in parser\"            # Con mensaje directo"
    echo "  $0 -f \"src/main.c src/utils.c\"       # Archivos específicos"
    echo "  $0 -m \"Update docs\" --no-clean      # Sin limpiar archivos"
    echo "  $0 --multi                            # Múltiples commits interactivos"
}

# Función para verificar si estamos en un repo git
check_git_repo() {
    if ! git rev-parse --git-dir > /dev/null 2>&1; then
        print_error "No estás en un repositorio Git"
        exit 1
    fi
}

# Función para hacer make fclean
do_clean() {
    if [[ $NO_CLEAN != true ]]; then
        print_status "Ejecutando make fclean..."
        if make fclean > /dev/null 2>&1; then
            print_success "Archivos .o eliminados"
        else
            print_warning "make fclean falló o no hay Makefile (continuando...)"
        fi
    else
        print_status "Omitiendo make fclean"
    fi
}

# Función para mostrar estado actual
show_git_status() {
    print_status "Estado actual del repositorio:"
    echo ""
    git status --short
    echo ""
    
    # Mostrar archivos no trackeados si los hay
    untracked=$(git ls-files --others --exclude-standard)
    if [[ -n "$untracked" ]]; then
        print_warning "Archivos no trackeados encontrados:"
        echo "$untracked" | sed 's/^/  /'
        echo ""
    fi
}

# Función para listar archivos disponibles para commit
list_available_files() {
    echo ""
    print_status "Archivos disponibles para commit:"
    
    # Archivos modificados
    modified=$(git diff --name-only)
    if [[ -n "$modified" ]]; then
        echo -e "${YELLOW}Modificados:${NC}"
        echo "$modified" | sed 's/^/  M /'
    fi
    
    # Archivos no trackeados
    untracked=$(git ls-files --others --exclude-standard)
    if [[ -n "$untracked" ]]; then
        echo -e "${GREEN}No trackeados:${NC}"
        echo "$untracked" | sed 's/^/  ? /'
    fi
    
    # Archivos en staging
    staged=$(git diff --cached --name-only)
    if [[ -n "$staged" ]]; then
        echo -e "${BLUE}En staging area:${NC}"
        echo "$staged" | sed 's/^/  A /'
    fi
    
    echo ""
}

# Función para añadir archivos específicos
add_specific_files() {
    local files="$1"
    
    if [[ -z "$files" ]]; then
        print_error "No se especificaron archivos"
        return 1
    fi
    
    print_status "Añadiendo archivos: $files"
    
    # Verificar que los archivos existen
    for file in $files; do
        if [[ ! -e "$file" ]]; then
            print_error "El archivo '$file' no existe"
            return 1
        fi
    done
    
    git add $files
    
    if [[ $? -eq 0 ]]; then
        print_success "Archivos añadidos al staging area"
        return 0
    else
        print_error "Error al añadir archivos"
        return 1
    fi
}

# Función para hacer un commit individual
do_single_commit() {
    local files="$1"
    local message="$2"
    
    # Limpiar staging area primero
    git reset > /dev/null 2>&1
    
    # Añadir archivos específicos
    if ! add_specific_files "$files"; then
        return 1
    fi
    
    # Verificar que hay cambios para commitear
    if git diff --cached --quiet; then
        print_warning "No hay cambios para hacer commit con estos archivos"
        return 1
    fi
    
    # Mostrar qué se va a commitear
    print_status "Archivos en este commit:"
    git diff --cached --name-status | sed 's/^/  /'
    
    # Hacer commit
    print_status "Haciendo commit con mensaje: \"$message\""
    git commit -m "$message"
    
    if [[ $? -eq 0 ]]; then
        print_success "Commit realizado exitosamente"
        return 0
    else
        print_error "Error al hacer commit"
        return 1
    fi
}

# Función para múltiples commits interactivos
do_multiple_commits() {
    local commit_count=0
    
    echo ""
    print_status "=== MODO MÚLTIPLES COMMITS ==="
    print_status "Podrás crear varios commits de forma separada"
    print_status "Escribe 'done' cuando termines, 'skip' para omitir, 'quit' para salir"
    
    while true; do
        echo ""
        echo -e "${BLUE}--- Commit #$((commit_count + 1)) ---${NC}"
        
        # Mostrar archivos disponibles
        list_available_files
        
        # Verificar si quedan archivos por commitear
        if git diff --quiet && git diff --cached --quiet && [[ -z "$(git ls-files --others --exclude-standard)" ]]; then
            print_success "No quedan archivos pendientes por commitear"
            break
        fi
        
        # Pedir archivos
        echo -n "Archivos para este commit (separados por espacios, o 'done'/'skip'/'quit'): "
        read -r files_input
        
        case "$files_input" in
            "done"|"DONE")
                print_status "Finalizando múltiples commits..."
                break
                ;;
            "quit"|"QUIT"|"exit"|"EXIT")
                print_warning "Saliendo sin completar commits restantes..."
                exit 0
                ;;
            "skip"|"SKIP")
                print_warning "Omitiendo este commit..."
                continue
                ;;
            "")
                print_warning "Debes especificar archivos o escribir 'done'"
                continue
                ;;
        esac
        
        # Pedir mensaje de commit
        echo -n "Mensaje para este commit: "
        read -r commit_msg
        
        if [[ -z "$commit_msg" ]]; then
            print_error "El mensaje de commit no puede estar vacío"
            continue
        fi
        
        # Realizar commit
        if do_single_commit "$files_input" "$commit_msg"; then
            commit_count=$((commit_count + 1))
            print_success "Commits realizados hasta ahora: $commit_count"
        else
            print_error "Error en el commit, intentando siguiente..."
        fi
    done
    
    if [[ $commit_count -gt 0 ]]; then
        print_success "Total de commits realizados: $commit_count"
    else
        print_warning "No se realizaron commits"
    fi
}

# Función para añadir archivos (modo original)
do_git_add() {
    if [[ -n "$FILES" ]]; then
        print_status "Añadiendo archivos especificados: $FILES"
        git add $FILES
    else
        echo -n "¿Qué archivos quieres añadir? (Enter para todos '.', o especifica): "
        read -r files_input
        
        if [[ -z "$files_input" ]]; then
            print_status "Añadiendo todos los archivos..."
            git add .
        else
            print_status "Añadiendo: $files_input"
            git add $files_input
        fi
    fi
    
    # Verificar que se añadieron archivos
    if git diff --cached --quiet; then
        print_warning "No hay cambios para hacer commit"
        echo "Estado actual:"
        git status --short
        exit 0
    fi
    
    print_success "Archivos añadidos al staging area"
    
    # Mostrar qué se va a commitear
    print_status "Archivos listos para commit:"
    git diff --cached --name-status | sed 's/^/  /'
}

# Función para hacer commit (modo original)
do_git_commit() {
    if [[ -n "$COMMIT_MESSAGE" ]]; then
        print_status "Haciendo commit con mensaje: \"$COMMIT_MESSAGE\""
        git commit -m "$COMMIT_MESSAGE"
    else
        echo ""
        echo -n "Escribe el mensaje del commit: "
        read -r commit_msg
        
        if [[ -z "$commit_msg" ]]; then
            print_error "El mensaje de commit no puede estar vacío"
            exit 1
        fi
        
        print_status "Haciendo commit..."
        git commit -m "$commit_msg"
    fi
    
    if [[ $? -eq 0 ]]; then
        print_success "Commit realizado exitosamente"
    else
        print_error "Error al hacer commit"
        exit 1
    fi
}

# Función para mostrar estado final
show_final_status() {
    echo ""
    print_success "¡Proceso completado!"
    
    # Mostrar últimos commits
    print_status "Últimos commits:"
    git log --oneline -5
    
    # Mostrar estado final
    echo ""
    print_status "Estado final del repositorio:"
    git status --short
    
    if git status --porcelain | grep -q .; then
        print_warning "Aún hay archivos sin commitear"
    else
        print_success "Working directory limpio"
    fi
    
    echo ""
    print_status "📝 Nota: Los commits están listos localmente"
    print_status "💡 Usa 'git push' cuando quieras subir los cambios al remoto"
}

# Parsear argumentos
COMMIT_MESSAGE=""
FILES=""
NO_CLEAN=false
MULTI_MODE=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -m|--message)
            COMMIT_MESSAGE="$2"
            shift 2
            ;;
        -f|--files)
            FILES="$2"
            shift 2
            ;;
        --no-clean)
            NO_CLEAN=true
            shift
            ;;
        --multi)
            MULTI_MODE=true
            shift
            ;;
        *)
            print_error "Opción desconocida: $1"
            show_help
            exit 1
            ;;
    esac
done

# Script principal
main() {
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}    🔄 SCRIPT DE COMMITS MÚLTIPLES 🔄${NC}"
    echo -e "${BLUE}================================================${NC}"
    echo ""
    
    # Verificaciones iniciales
    check_git_repo
    
    # Mostrar estado inicial
    show_git_status
    
    # Ejecutar limpieza
    do_clean
    
    # Decidir modo de operación
    if [[ $MULTI_MODE == true ]]; then
        # Modo múltiples commits
        do_multiple_commits
    else
        # Modo original (un solo commit)
        do_git_add
        do_git_commit
    fi
    
    # Mostrar estado final
    show_final_status
    
    echo ""
    echo -e "${GREEN}✅ ¡Commits completados! No se realizó push${NC}"
}

# Ejecutar script principal
main