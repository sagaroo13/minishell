#!/bin/bash

# Test estricto de validación de redirecciones
# Debe comportarse exactamente como bash/zsh

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== STRICT REDIRECTION VALIDATION TEST ===${NC}"
echo "Testing minishell redirection syntax validation against bash behavior..."

passed=0
failed=0

# Función para testear comandos que deben mostrar error de sintaxis
test_syntax_error() {
    local cmd="$1"
    local description="$2"
    
    echo -e "\nTesting: $description"
    echo "Command: $cmd"
    
    # Test con minishell - buscar el mensaje de error
    local output=$(echo -e "$cmd\nexit" | timeout 5 ./minishell 2>&1)
    
    if echo "$output" | grep -q "syntax error near unexpected token"; then
        echo -e "✓ ${GREEN}PASS${NC} - Minishell correctly detects syntax error"
        ((passed++))
    else
        echo -e "✗ ${RED}FAIL${NC} - Minishell should detect syntax error"
        echo "Output: $output"
        ((failed++))
    fi
}

# Función para testear comandos que NO deben mostrar error de sintaxis
test_valid_syntax() {
    local cmd="$1"
    local description="$2"
    
    echo -e "\nTesting: $description"
    echo "Command: $cmd"
    
    # Test con minishell - NO debe haber mensaje de error de sintaxis
    local output=$(echo -e "$cmd\nexit" | timeout 5 ./minishell 2>&1)
    
    if echo "$output" | grep -q "syntax error near unexpected token"; then
        echo -e "✗ ${RED}FAIL${NC} - Minishell incorrectly reports syntax error"
        echo "Output: $output"
        ((failed++))
    else
        echo -e "✓ ${GREEN}PASS${NC} - Minishell accepts valid syntax"
        ((passed++))
    fi
}

# Tests de sintaxis inválida (deben mostrar error)
echo -e "\n${YELLOW}=== INVALID SYNTAX TESTS ===${NC}"

test_syntax_error "cat <<<" "Here-string without argument"
test_syntax_error "cat <<<<" "Four consecutive < operators"
test_syntax_error "cat <<<<<" "Five consecutive < operators"
test_syntax_error "cat <<<<<<<<<" "Many consecutive < operators"
test_syntax_error "cat >>>" "Three consecutive > operators"
test_syntax_error "cat >>>>" "Four consecutive > operators"
test_syntax_error "cat >>>>>" "Five consecutive > operators"
test_syntax_error "cat >>>>>>>>>>" "Many consecutive > operators"

# Tests de sintaxis válida (NO deben mostrar error)
echo -e "\n${YELLOW}=== VALID SYNTAX TESTS ===${NC}"

test_valid_syntax "echo hello > /tmp/test_output.txt" "Simple output redirection"
test_valid_syntax "echo hello >> /tmp/test_append.txt" "Append redirection"

# Test heredoc válido (más complejo)
echo -e "\nTesting: Basic heredoc"
echo "Command: cat << EOF + content + EOF"
echo "test content" > /tmp/test_input.txt
output=$(echo -e "cat << EOF\nhello world\nEOF\nexit" | timeout 5 ./minishell 2>&1)
if echo "$output" | grep -q "syntax error near unexpected token"; then
    echo -e "✗ ${RED}FAIL${NC} - Minishell incorrectly reports syntax error for heredoc"
    ((failed++))
else
    echo -e "✓ ${GREEN}PASS${NC} - Minishell accepts heredoc syntax"
    ((passed++))
fi

# Resumen
echo -e "\n${YELLOW}=== TEST RESULTS ===${NC}"
echo "Passed: $passed"
echo "Failed: $failed"
echo "Total: $((passed + failed))"

# Cleanup
rm -f /tmp/test_output.txt /tmp/test_append.txt /tmp/test_input.txt

if [ $failed -eq 0 ]; then
    echo -e "\n🎉 ${GREEN}ALL STRICT REDIRECTION TESTS PASSED!${NC} 🎉"
    echo "Minishell redirection validation matches expected behavior!"
    exit 0
else
    echo -e "\n❌ ${RED}Some strict redirection tests failed.${NC}"
    exit 1
fi
