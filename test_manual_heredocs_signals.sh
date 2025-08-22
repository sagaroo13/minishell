#!/bin/bash

# Script de pruebas específicas para heredocs y signals
echo "==============================================="
echo "PRUEBAS ESPECÍFICAS - HEREDOCS Y SIGNALS"
echo "==============================================="

echo ""
echo "--- PRUEBA 1: Heredoc con cat ---"
echo "Comando: cat << EOF"
echo "linea1"
echo "linea2" 
echo "EOF"
echo ""

echo "--- PRUEBA 2: Heredoc con expansión de variables ---"
echo "export TEST_VAR=hello"
echo "cat << EOF"
echo "Variable: \$TEST_VAR"
echo "EOF"
echo ""

echo "--- PRUEBA 3: Heredoc sin expansión (quoted delimiter) ---"
echo "cat << 'EOF'"
echo "Variable: \$TEST_VAR"  
echo "EOF"
echo ""

echo "--- PRUEBA 4: Export y verificación ---"
echo "export TEST_EXPORT=valor_test"
echo "export | grep TEST_EXPORT"
echo "env | grep TEST_EXPORT"
echo ""

echo "--- PRUEBA 5: Unset y verificación ---"
echo "unset TEST_EXPORT"
echo "env | grep TEST_EXPORT"
echo ""

echo "--- PRUEBA 6: Exit codes específicos ---"
echo "false; echo \$?"
echo "true; echo \$?"
echo "/bin/ls /directorio_inexistente; echo \$?"
echo ""

echo "==============================================="
echo "NOTA: Para probar signals (Ctrl+C, Ctrl+\\, Ctrl+D)"
echo "debes hacerlo manualmente en el shell interactivo"
echo "==============================================="
