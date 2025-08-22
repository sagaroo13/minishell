#!/bin/bash

# Test suite enfocado en built-ins sin usar && que no está soportado
echo "=== BUILT-INS TEST SUITE (SIN OPERADORES LOGICOS) ==="

echo ""
echo "=== TEST 1: Echo con redirección ==="
echo 'echo hello > output.txt' | ./minishell
echo "Archivo creado:"
cat output.txt 2>/dev/null || echo "No se creó el archivo"
rm -f output.txt

echo ""
echo "=== TEST 2: Pwd con redirección ==="
echo 'pwd > pwd_output.txt' | ./minishell
echo "Contenido del archivo:"
cat pwd_output.txt 2>/dev/null || echo "No se creó el archivo"
rm -f pwd_output.txt

echo ""
echo "=== TEST 3: Env con redirección ==="
echo 'env > env_output.txt' | ./minishell
echo "Líneas en archivo env:"
wc -l < env_output.txt 2>/dev/null || echo "No se creó el archivo"
rm -f env_output.txt

echo ""
echo "=== TEST 4: Export funcional ==="
echo -e 'export TEST_VAR=hello\necho $TEST_VAR' | ./minishell

echo ""
echo "=== TEST 5: Exit con número ==="
echo 'exit 42' | ./minishell
echo "Exit code: $?"

echo ""
echo "=== TEST 6: Exit con texto inválido ==="
echo 'exit hello' | ./minishell
echo "Exit code: $?"

echo ""
echo "=== TEST 7: Exit con demasiados argumentos ==="
echo -e 'exit 1 2\necho should_not_see_this\nexit 0' | ./minishell
echo "Exit code: $?"

echo ""
echo "=== TEST 8: CD a directorio existente ==="
echo -e 'cd /tmp\npwd' | ./minishell

echo ""
echo "=== TEST 9: CD a directorio inexistente ==="
echo -e 'cd /nonexistent_directory_test\npwd' | ./minishell

echo ""
echo "=== TEST 10: Echo con -n ==="
echo 'echo -n hello' | ./minishell
echo " <- debería estar en la misma línea"

echo ""
echo "=== TEST 11: Heredoc simple ==="
echo -e 'cat << EOF\nHello World\nThis is a test\nEOF' | ./minishell

echo ""
echo "=== TEST 12: Unset variable ==="
echo -e 'export TEMP_VAR=value\necho $TEMP_VAR\nunset TEMP_VAR\necho empty:$TEMP_VAR' | ./minishell

echo ""
echo "=== TEST 13: Redirección de error ==="
echo 'ls /nonexistent 2> error.txt' | ./minishell
echo "Archivo de error creado:"
cat error.txt 2>/dev/null || echo "No se creó archivo de error"
rm -f error.txt

echo ""
echo "=== TEST 14: Multiple redirecciones ==="
echo -e 'echo first > test.txt\necho second >> test.txt\ncat test.txt' | ./minishell
rm -f test.txt

echo ""
echo "=== FIN DE TESTS ==="
