#!/bin/bash

# Test específico para BUILT-INS y HEREDOCS
# Área de responsabilidad: built-ins, heredoc, exit codes, redirecciones

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Función para ejecutar test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_behavior="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -e "\n${YELLOW}TEST $TOTAL_TESTS: $test_name${NC}"
    echo "Command: $command"
    echo "Expected: $expected_behavior"
    
    # Ejecutar en bash para comparación
    echo "=== BASH OUTPUT ==="
    timeout 5 bash -c "$command" 2>&1
    local bash_exit=$?
    echo "Bash exit code: $bash_exit"
    
    # Ejecutar en minishell
    echo "=== MINISHELL OUTPUT ==="
    echo "$command" | timeout 5 ./minishell 2>&1
    local mini_exit=$?
    echo "Minishell exit code: $mini_exit"
    
    echo "----------------------------------------"
}

echo "==================================="
echo "BUILT-INS & HEREDOCS TEST SUITE"
echo "==================================="

# Compilar minishell
echo "Compiling minishell..."
make re > /dev/null 2>&1

# 1. BUILT-INS CON REDIRECCIONES
echo -e "\n${GREEN}=== BUILT-INS CON REDIRECCIONES ===${NC}"

run_test "Echo con redirección simple" \
    "echo hello > test_file.txt && cat test_file.txt && rm -f test_file.txt" \
    "Debe crear archivo con 'hello'"

run_test "Pwd con redirección" \
    "pwd > pwd_test.txt && cat pwd_test.txt && rm -f pwd_test.txt" \
    "Debe guardar directorio actual en archivo"

run_test "Env con redirección" \
    "env > env_test.txt && echo 'Lines:' && wc -l < env_test.txt && rm -f env_test.txt" \
    "Debe guardar todas las variables de entorno"

run_test "Export con redirección" \
    "export TEST_VAR=value > /dev/null && echo \$TEST_VAR" \
    "Debe exportar variable sin mostrar output"

# 2. EXIT CON DIFERENTES ARGUMENTOS
echo -e "\n${GREEN}=== EXIT BEHAVIOR ===${NC}"

run_test "Exit sin argumentos" \
    "echo 'false; exit' | ./minishell; echo \$?" \
    "Debe salir con último código de error (1)"

run_test "Exit con número válido" \
    "echo 'exit 42' | ./minishell; echo \$?" \
    "Debe salir con código 42"

run_test "Exit con número negativo" \
    "echo 'exit -5' | ./minishell; echo \$?" \
    "Debe salir con código 251 (256-5)"

run_test "Exit con número grande" \
    "echo 'exit 512' | ./minishell; echo \$?" \
    "Debe salir con código 0 (512 % 256)"

run_test "Exit con texto" \
    "echo 'exit hello' | ./minishell; echo \$?" \
    "Debe mostrar error y salir con código 2"

run_test "Exit con demasiados argumentos" \
    "echo -e 'exit 1 2\necho still here\nexit 0' | ./minishell; echo \$?" \
    "Debe mostrar error, NO salir, continuar ejecución"

# 3. HEREDOC BÁSICO
echo -e "\n${GREEN}=== HEREDOC FUNCTIONALITY ===${NC}"

run_test "Heredoc simple" \
    "cat << EOF
Hello World
This is a test
EOF" \
    "Debe mostrar las líneas del heredoc"

run_test "Heredoc con variable" \
    "export USER_TEST=john && cat << EOF
Hello \$USER_TEST
EOF" \
    "Debe expandir variables en heredoc"

run_test "Heredoc quoted delimiter" \
    "cat << 'EOF'
Hello \$USER
EOF" \
    "NO debe expandir variables (delimitador quoted)"

# 4. CÓDIGOS DE SALIDA
echo -e "\n${GREEN}=== EXIT CODES ===${NC}"

run_test "Comando exitoso" \
    "echo hello > /dev/null; echo \$?" \
    "Debe retornar 0"

run_test "Comando inexistente" \
    "nonexistent_command 2>/dev/null; echo \$?" \
    "Debe retornar 127"

run_test "Built-in exitoso" \
    "pwd > /dev/null; echo \$?" \
    "Debe retornar 0"

run_test "Built-in con error" \
    "cd /nonexistent/directory 2>/dev/null; echo \$?" \
    "Debe retornar 1"

# 5. CD FUNCTIONALITY
echo -e "\n${GREEN}=== CD BUILT-IN ===${NC}"

run_test "CD a directorio home" \
    "cd && pwd" \
    "Debe ir al directorio home"

run_test "CD con path relativo" \
    "cd /tmp && pwd && cd - > /dev/null && pwd" \
    "Debe cambiar a /tmp y luego volver"

run_test "CD a directorio inexistente" \
    "cd /nonexistent/path 2>&1 && echo \$?" \
    "Debe mostrar error y retornar 1"

# 6. EXPORT/UNSET
echo -e "\n${GREEN}=== EXPORT/UNSET ===${NC}"

run_test "Export nueva variable" \
    "export NEW_VAR=test && echo \$NEW_VAR" \
    "Debe crear y mostrar variable"

run_test "Export sin argumentos" \
    "export | grep -q PATH && echo 'PATH found'" \
    "Debe mostrar todas las variables exportadas"

run_test "Unset variable existente" \
    "export TEMP_VAR=value && echo \$TEMP_VAR && unset TEMP_VAR && echo \"empty:\$TEMP_VAR\"" \
    "Debe mostrar valor y luego vacío"

run_test "Unset variable inexistente" \
    "unset NONEXISTENT_VAR; echo \$?" \
    "Debe ser silencioso y retornar 0"

# 7. ENV BUILT-IN
echo -e "\n${GREEN}=== ENV BUILT-IN ===${NC}"

run_test "Env sin argumentos" \
    "env | wc -l" \
    "Debe mostrar todas las variables de entorno"

run_test "Env con redirección" \
    "env > env_out.txt && wc -l < env_out.txt && rm -f env_out.txt" \
    "Debe guardar env en archivo"

# 8. PWD BUILT-IN
echo -e "\n${GREEN}=== PWD BUILT-IN ===${NC}"

run_test "Pwd básico" \
    "pwd" \
    "Debe mostrar directorio actual"

run_test "Pwd con redirección" \
    "pwd > pwd_out.txt && cat pwd_out.txt && rm -f pwd_out.txt" \
    "Debe guardar pwd en archivo"

# 9. ECHO BUILT-IN
echo -e "\n${GREEN}=== ECHO BUILT-IN ===${NC}"

run_test "Echo simple" \
    "echo hello world" \
    "Debe mostrar: hello world"

run_test "Echo con -n flag" \
    "echo -n hello && echo world" \
    "Debe mostrar: helloworld (sin newline)"

run_test "Echo con múltiples -n" \
    "echo -n -n -n hello" \
    "Debe reconocer múltiples -n como uno solo"

run_test "Echo con -- para terminar opciones" \
    "echo -- -n hello" \
    "Debe mostrar: -- -n hello"

# 10. REDIRECCIONES CON BUILT-INS
echo -e "\n${GREEN}=== REDIRECCIONES AVANZADAS ===${NC}"

run_test "Multiple redirecciones" \
    "echo test > file1.txt && echo test2 >> file1.txt && cat file1.txt && rm -f file1.txt" \
    "Debe tener dos líneas en archivo"

run_test "Redirección de error" \
    "pwd 2> error.txt && echo 'No error file created' || echo 'Error file exists' && rm -f error.txt" \
    "Built-ins no deben crear archivos de error normalmente"

# 11. MEMORY Y PERFORMANCE
echo -e "\n${GREEN}=== MEMORY & PERFORMANCE ===${NC}"

run_test "Exit no debe tener leaks" \
    "echo 'exit 0' | valgrind --leak-check=full --error-exitcode=1 ./minishell 2>&1 | grep -q 'no leaks are possible'" \
    "Exit debe limpiar memoria correctamente"

# Resumen
echo -e "\n${GREEN}==================================="
echo "RESUMEN BUILT-INS & HEREDOCS"
echo "===================================${NC}"
echo "Total tests ejecutados: $TOTAL_TESTS"
echo -e "${BLUE}Área de enfoque: Built-ins, Heredocs, Exit codes${NC}"

# Cleanup
rm -f test_*.txt pwd_*.txt env_*.txt *_out.txt file*.txt

echo -e "\n${GREEN}Test suite completado.${NC}"
