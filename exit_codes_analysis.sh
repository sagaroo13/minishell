#!/bin/bash

echo "=== ANÁLISIS EXHAUSTIVO DE EXIT CODES ==="
echo "Comparando minishell vs bash en cada caso"

# Función para probar exit codes
test_exit_code() {
    local test_name="$1"
    local command="$2"
    
    echo ""
    echo "=== $test_name ==="
    echo "Comando: $command"
    
    # Test en bash
    echo "BASH:"
    bash -c "$command"
    bash_code=$?
    echo "Exit code: $bash_code"
    
    # Test en minishell
    echo "MINISHELL:"
    echo "$command" | ./minishell
    mini_code=$?
    echo "Exit code: $mini_code"
    
    # Comparación
    if [ $bash_code -eq $mini_code ]; then
        echo "✅ MATCH: $bash_code"
    else
        echo "❌ MISMATCH: bash=$bash_code, minishell=$mini_code"
    fi
    echo "----------------------------------------"
}

echo "Compilando minishell..."
make re > /dev/null 2>&1

# Tests de exit codes
test_exit_code "EXIT sin argumentos" "exit"
test_exit_code "EXIT con 0" "exit 0"
test_exit_code "EXIT con número positivo" "exit 42"
test_exit_code "EXIT con número negativo" "exit -5"
test_exit_code "EXIT con 256" "exit 256"
test_exit_code "EXIT con 257" "exit 257"
test_exit_code "EXIT con número grande" "exit 1000"
test_exit_code "EXIT con texto" "exit hello"
test_exit_code "EXIT con número y texto" "exit 42abc"
test_exit_code "EXIT con espacios" "exit '  42  '"
test_exit_code "EXIT con signo y espacios" "exit ' -5 '"
test_exit_code "EXIT con overflow positivo" "exit 9223372036854775808"
test_exit_code "EXIT con overflow negativo" "exit -9223372036854775809"

# Test especial para demasiados argumentos (no debe salir)
echo ""
echo "=== TEST ESPECIAL: EXIT con demasiados argumentos ==="
echo "BASH:"
bash -c 'exit 1 2; echo "No debería salir: $?"'
bash_code=$?
echo "Exit code final bash: $bash_code"

echo "MINISHELL:"
echo -e 'exit 1 2\necho "No debería salir: $?"' | ./minishell
mini_code=$?
echo "Exit code final minishell: $mini_code"

# Tests de built-ins
test_exit_code "ECHO exitoso" "echo hello > /dev/null"
test_exit_code "PWD exitoso" "pwd > /dev/null"
test_exit_code "ENV exitoso" "env > /dev/null"
test_exit_code "CD exitoso" "cd /tmp"
test_exit_code "CD error" "cd /directorio_inexistente"
test_exit_code "EXPORT válido" "export TEST_VAR=value"
test_exit_code "EXPORT inválido" "export 123INVALID=value"
test_exit_code "UNSET existente" "export TEST=1; unset TEST"
test_exit_code "UNSET inexistente" "unset INEXISTENTE"

# Tests de comandos externos
test_exit_code "Comando inexistente" "comando_que_no_existe_xyz"
test_exit_code "LS exitoso" "ls /dev/null"
test_exit_code "LS error" "ls /directorio_inexistente"

echo ""
echo "=== RESUMEN COMPLETO ==="
