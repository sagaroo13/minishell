#!/bin/bash

# =============================================================================
# TEST ESPECÍFICO PARA EVALUACIÓN 42 SCHOOL
# ÁREAS CRÍTICAS: BUILT-INS, SIGNALS, HEREDOCS
# =============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

print_header() {
    echo -e "${BLUE}=================================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}=================================================${NC}"
}

print_section() {
    echo -e "\n${PURPLE}--- $1 ---${NC}"
}

print_test() {
    ((TOTAL_TESTS++))
    echo -e "\n${YELLOW}TEST $TOTAL_TESTS: $1${NC}"
}

print_pass() {
    echo -e "${GREEN}✅ PASS${NC}"
    ((PASSED_TESTS++))
}

print_fail() {
    echo -e "${RED}❌ FAIL: $1${NC}"
    ((FAILED_TESTS++))
}

test_builtin_command() {
    local description="$1"
    local command="$2"
    local expected_exit="$3"
    
    print_test "$description"
    echo "Command: $command"
    
    # Ejecutar comando
    echo "$command" | ./minishell > /tmp/mini_out 2>&1
    local actual_exit=$?
    
    if [ ! -z "$expected_exit" ]; then
        if [ $actual_exit -eq $expected_exit ]; then
            print_pass
        else
            print_fail "Expected exit $expected_exit, got $actual_exit"
        fi
    else
        # Solo verificar que no crashee
        if [ $actual_exit -lt 128 ]; then
            print_pass
        else
            print_fail "Command crashed with exit code $actual_exit"
        fi
    fi
}

test_heredoc_functionality() {
    local description="$1"
    local heredoc_content="$2"
    local expected_output="$3"
    
    print_test "$description"
    
    # Crear heredoc temporal
    echo "$heredoc_content" > /tmp/heredoc_test_input
    
    # Ejecutar con minishell
    cat /tmp/heredoc_test_input | ./minishell > /tmp/heredoc_mini_out 2>&1
    
    # Ejecutar con bash para comparar
    cat /tmp/heredoc_test_input | bash > /tmp/heredoc_bash_out 2>&1
    
    # Comparar resultados (ignorando prompts)
    if grep -q "$expected_output" /tmp/heredoc_mini_out; then
        print_pass
    else
        print_fail "Expected output '$expected_output' not found"
        echo "--- MINISHELL OUTPUT ---"
        cat /tmp/heredoc_mini_out
        echo "--- BASH OUTPUT ---"
        cat /tmp/heredoc_bash_out
        echo "--- END ---"
    fi
    
    rm -f /tmp/heredoc_test_input /tmp/heredoc_mini_out /tmp/heredoc_bash_out
}

# Compilar minishell
echo "Compilando minishell..."
make re > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}ERROR: No se pudo compilar minishell${NC}"
    exit 1
fi

print_header "TEST EVALUACIÓN 42 - BUILT-INS, SIGNALS & HEREDOCS"

# =============================================================================
print_header "SECCIÓN 1: BUILT-INS CRÍTICOS"
# =============================================================================

print_section "ECHO - Casos Edge"
test_builtin_command "Echo básico" "echo hello world" 0
test_builtin_command "Echo con -n" "echo -n hello" 0
test_builtin_command "Echo sin argumentos" "echo" 0
test_builtin_command "Echo con múltiples -n" "echo -n -n -n hello" 0

print_section "PWD - Funcionalidad"
test_builtin_command "Pwd básico" "pwd" 0
test_builtin_command "Pwd con argumentos" "pwd arg1 arg2" 0

print_section "CD - Casos Críticos"
test_builtin_command "CD sin argumentos (HOME)" "cd" 0
test_builtin_command "CD a directorio existente" "cd /" 0
test_builtin_command "CD a directorio inexistente" "cd /directorio/que/no/existe" 1
test_builtin_command "CD con ~" "cd ~" 0
test_builtin_command "CD con .." "cd .." 0

print_section "ENV - Display"
test_builtin_command "Env básico" "env" 0

print_section "EXPORT - Validación Crítica"
test_builtin_command "Export sin argumentos" "export" 0
test_builtin_command "Export variable válida" "export TEST_VAR=hello" 0
test_builtin_command "Export variable sin valor" "export TEST_VAR2" 0
test_builtin_command "Export identificador inválido - número" "export 123ABC=value" 1
test_builtin_command "Export identificador inválido - guión" "export TEST-VAR=value" 1

print_section "UNSET - Funcionalidad"
test_builtin_command "Unset variable existente" "unset PATH" 0
test_builtin_command "Unset variable inexistente" "unset VAR_INEXISTENTE" 0

print_section "EXIT - Casos Críticos"
test_builtin_command "Exit con número válido" "exit 42" 42
test_builtin_command "Exit con 0" "exit 0" 0
test_builtin_command "Exit con número negativo" "exit -1" 255
test_builtin_command "Exit con overflow" "exit 256" 0
test_builtin_command "Exit con string" "exit hello" 2

# Test especial para exit con demasiados argumentos
print_test "Exit con demasiados argumentos (no debe salir)"
echo -e 'exit 1 2\necho "Still running"\nexit 0' | ./minishell > /tmp/exit_many_test 2>&1
if grep -q "Still running" /tmp/exit_many_test; then
    print_pass
else
    print_fail "Should continue execution after 'exit 1 2'"
fi

# =============================================================================
print_header "SECCIÓN 2: HEREDOCS FUNCIONALES"
# =============================================================================

print_section "Heredoc Básico"
test_heredoc_functionality "Heredoc simple" \
    'cat << EOF
Hello World
EOF' \
    "Hello World"

print_section "Heredoc con Múltiples Líneas"
test_heredoc_functionality "Heredoc múltiples líneas" \
    'cat << END
Línea 1
Línea 2
Línea 3
END' \
    "Línea 1"

print_section "Heredoc con Delimitador Personalizado"
test_heredoc_functionality "Heredoc delimitador custom" \
    'cat << DELIM
Contenido personalizado
DELIM' \
    "Contenido personalizado"

# =============================================================================
print_header "SECCIÓN 3: ERROR HANDLING"
# =============================================================================

print_section "Comandos Inexistentes"
test_builtin_command "Comando inexistente" "comando_que_no_existe_xyz" 127

print_section "Sintaxis Inválida"
test_builtin_command "Redirección inválida" "echo hello >" 2

# =============================================================================
print_header "SECCIÓN 4: INTEGRACIÓN COMPLEJA"
# =============================================================================

print_section "Variables y Built-ins"
print_test "Persistencia de variables entre comandos"
echo -e 'export TEST_PERSIST=value\necho $TEST_PERSIST' | ./minishell > /tmp/persist_test 2>&1
if grep -q "value" /tmp/persist_test; then
    print_pass
else
    print_fail "Variables not persisting"
fi

print_section "Redirecciones con Built-ins"
print_test "Echo con redirección a archivo"
echo 'echo "test_content" > /tmp/redir_test' | ./minishell > /dev/null 2>&1
if [ -f /tmp/redir_test ] && grep -q "test_content" /tmp/redir_test; then
    print_pass
    rm -f /tmp/redir_test
else
    print_fail "Redirection with built-in failed"
fi

# =============================================================================
print_header "SECCIÓN 5: CASOS EDGE ESPECÍFICOS"
# =============================================================================

print_section "Echo Edge Cases"
test_builtin_command "Echo con espacios" 'echo "   espacios   "' 0
test_builtin_command "Echo con caracteres especiales" 'echo "!@#$%^&*()"' 0

print_section "CD Edge Cases"  
test_builtin_command "CD con múltiples slashes" "cd ///" 0
test_builtin_command "CD a archivo (debe fallar)" "cd /etc/passwd" 1

print_section "Export Edge Cases"
test_builtin_command "Export con equals en valor" 'export TEST="val=ue"' 0
test_builtin_command "Export variable vacía" 'export EMPTY=""' 0

# =============================================================================
print_header "RESUMEN FINAL"
# =============================================================================

echo ""
echo -e "${BLUE}==================== RESUMEN DE EVALUACIÓN ====================${NC}"
echo -e "${GREEN}PASSED: $PASSED_TESTS${NC}"
echo -e "${RED}FAILED: $FAILED_TESTS${NC}"
echo -e "${YELLOW}TOTAL:  $TOTAL_TESTS${NC}"
echo ""

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡PERFECTO! TODOS LOS TESTS CRÍTICOS PASARON 🎉${NC}"
    echo -e "${GREEN}Tu minishell está LISTO para evaluación de 42${NC}"
    echo ""
    echo -e "${BLUE}✅ Built-ins: FUNCIONANDO${NC}"
    echo -e "${BLUE}✅ Heredocs: FUNCIONANDO${NC}"
    echo -e "${BLUE}✅ Error handling: CORRECTO${NC}"
    echo -e "${BLUE}✅ Exit codes: PERFECTOS${NC}"
    exit 0
else
    PERCENTAGE=$(( PASSED_TESTS * 100 / TOTAL_TESTS ))
    echo -e "${YELLOW}⚠️  $FAILED_TESTS tests fallaron de $TOTAL_TESTS${NC}"
    echo -e "${BLUE}SCORE: $PERCENTAGE%${NC}"
    
    if [ $PERCENTAGE -ge 90 ]; then
        echo -e "${GREEN}¡EXCELENTE SCORE! Casi perfecto.${NC}"
    elif [ $PERCENTAGE -ge 80 ]; then
        echo -e "${YELLOW}BUEN SCORE. Ajustes menores necesarios.${NC}"
    elif [ $PERCENTAGE -ge 70 ]; then
        echo -e "${YELLOW}SCORE DECENTE. Algunas correcciones necesarias.${NC}"
    else
        echo -e "${RED}SCORE BAJO. Trabajo adicional requerido.${NC}"
    fi
    
    echo ""
    echo -e "${BLUE}Areas evaluadas críticas para 42:${NC}"
    echo "• Built-ins (echo, pwd, cd, env, export, unset, exit)"
    echo "• Heredocs básicos y funcionales"
    echo "• Error handling y exit codes"
    echo "• Casos edge y validaciones"
    
    exit $FAILED_TESTS
fi

# Cleanup
rm -f /tmp/mini_out /tmp/persist_test /tmp/exit_many_test
