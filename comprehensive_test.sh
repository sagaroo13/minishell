#!/bin/bash

# Script de testing para minishell
# Prueba funcionalidades básicas y avanzadas

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

MINISHELL="./minishell"
PASSED=0
FAILED=0

# Función para ejecutar test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_output="$3"
    local expected_exit_code="$4"
    
    echo -e "\n${YELLOW}Testing: $test_name${NC}"
    echo "Command: $command"
    
    # Crear archivo temporal con el comando
    echo -e "$command\necho \$?" > test_input.tmp
    
    # Ejecutar minishell
    output=$($MINISHELL < test_input.tmp 2>&1)
    exit_code=$?
    
    # Extraer el código de salida del output
    actual_exit_code=$(echo "$output" | tail -n 2 | head -n 1)
    
    # Verificar resultado
    if [[ "$actual_exit_code" == "$expected_exit_code" ]]; then
        echo -e "${GREEN}✓ PASS${NC} - Exit code: $actual_exit_code"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAIL${NC} - Expected exit code: $expected_exit_code, Got: $actual_exit_code"
        echo "Full output:"
        echo "$output"
        ((FAILED++))
    fi
    
    rm -f test_input.tmp
}

# Función para test interactivo
run_interactive_test() {
    local test_name="$1"
    local commands="$2"
    
    echo -e "\n${YELLOW}Interactive Test: $test_name${NC}"
    echo "Commands: $commands"
    
    echo -e "$commands" > test_input.tmp
    echo -e "\n${YELLOW}Output:${NC}"
    $MINISHELL < test_input.tmp 2>&1
    
    rm -f test_input.tmp
}

echo "=== MINISHELL COMPREHENSIVE TEST SUITE ==="
echo "Testing basic and advanced functionality..."

# ===== TESTS BÁSICOS =====
echo -e "\n${YELLOW}=== BASIC FUNCTIONALITY TESTS ===${NC}"

run_test "Simple command" "ls" "" "0"
run_test "Command with arguments" "echo hello world" "" "0" 
run_test "Non-existent command" "nonexistentcommand" "" "127"
run_test "Empty command" "" "" "0"

# ===== BUILT-INS =====
echo -e "\n${YELLOW}=== BUILT-IN COMMANDS TESTS ===${NC}"

run_test "echo without arguments" "echo" "" "0"
run_test "echo with -n flag" "echo -n hello" "" "0"
run_test "echo with multiple -n flags" "echo -nnn hello" "" "0"
run_test "pwd command" "pwd" "" "0"
run_test "env command" "env" "" "0"

# CD tests
run_test "cd to home" "cd" "" "0"
run_test "cd to home with ~" "cd ~" "" "0"
run_test "cd to non-existent directory" "cd /nonexistent" "" "1"

# Export/Unset tests
run_test "export new variable" "export TEST_VAR=hello" "" "0"
run_test "export without arguments" "export" "" "0"
run_test "unset variable" "unset TEST_VAR" "" "0"
run_test "export invalid identifier" "export 123VAR=test" "" "1"

# Exit tests
# Test exit commands - NOTE: These tests will exit the shell, so they need special handling
echo "Testing: exit with code"
echo 'exit 42' | timeout 5 ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 42 ]; then
    echo "✓ PASS - Exit code: $exit_code"
    ((passed++))
else
    echo "✗ FAIL - Expected exit code: 42, Got: $exit_code" 
    ((failed++))
fi

echo "Testing: exit without code"
echo 'exit' | timeout 5 ./minishell >/dev/null 2>&1  
exit_code=$?
if [ $exit_code -eq 0 ]; then
    echo "✓ PASS - Exit code: $exit_code"
    ((passed++))
else
    echo "✗ FAIL - Expected exit code: 0, Got: $exit_code"
    ((failed++))
fi

# ===== VARIABLE EXPANSION =====
echo -e "\n${YELLOW}=== VARIABLE EXPANSION TESTS ===${NC}"

# Note: Minishell doesn't support command separation with ';', so we test $? differently
echo "Testing: Exit status expansion"
echo -e "nonexistentcommand\necho \$?" | timeout 5 ./minishell 2>/dev/null | tail -1 > test_output.tmp
if grep -q "127" test_output.tmp; then
    echo "✓ PASS - Exit code: 0"
    ((passed++))
else
    echo "✗ FAIL - Expected exit code: 0, Got: shell didn't capture exit status correctly"
    echo "Output: $(cat test_output.tmp)"
    ((failed++))
fi
rm -f test_output.tmp

echo "Testing: Multiple exit status expansion"  
echo -e "nonexistentcommand\necho \$?+\$?" | timeout 5 ./minishell 2>/dev/null | tail -1 > test_output.tmp
if grep -q "127+127" test_output.tmp; then
    echo "✓ PASS - Exit code: 0"
    ((passed++))
else
    echo "✗ FAIL - Expected exit code: 0, Got: incorrect expansion"
    echo "Output: $(cat test_output.tmp)"
    ((failed++))
fi
rm -f test_output.tmp
run_test "Environment variable expansion" "echo \$HOME" "" "0"
run_test "Non-existent variable expansion" "echo \$NONEXISTENT" "" "0"

# ===== REDIRECCIONES =====
echo -e "\n${YELLOW}=== REDIRECTION TESTS ===${NC}"

# Crear archivos de prueba
echo "test content" > test_file.txt
echo "append content" > append_file.txt

run_test "Input redirection existing file" "cat < test_file.txt" "" "0"
run_test "Input redirection non-existent file" "cat < nonexistent.txt" "" "1"
run_test "Output redirection" "echo hello > output.txt" "" "0"
run_test "Append redirection" "echo world >> append_file.txt" "" "0"
run_test "Built-in with failed redirection" "cd < nonexistent.txt" "" "1"
run_test "Echo with failed input redirection" "echo < nonexistent.txt" "" "1"

# ===== PIPES =====
echo -e "\n${YELLOW}=== PIPE TESTS ===${NC}"

run_test "Simple pipe" "echo hello | cat" "" "0"
run_test "Multiple pipes" "echo hello | cat | cat" "" "0"
run_test "Pipe with built-in" "echo test | grep test" "" "0"
run_test "Failed command in pipe" "nonexistent | cat" "" "0"

# ===== HEREDOC =====
echo -e "\n${YELLOW}=== HEREDOC TESTS ===${NC}"

# Crear un test de heredoc más simple
echo -e "cat << EOF\nhello\nworld\nEOF" > heredoc_test.tmp
run_interactive_test "Basic heredoc" "$(cat heredoc_test.tmp)"
rm -f heredoc_test.tmp

# ===== SIGNALS =====
echo -e "\n${YELLOW}=== SIGNAL HANDLING TESTS ===${NC}"

echo "Signal tests require manual testing (Ctrl+C, Ctrl+\\, Ctrl+D)"

# ===== SYNTAX ERRORS =====
echo -e "\n${YELLOW}=== SYNTAX ERROR TESTS ===${NC}"

run_test "Incomplete redirection" "echo >" "" "2"
run_test "Incomplete pipe" "echo |" "" "2"
run_test "Invalid syntax" "echo >>>" "" "1"

# ===== EDGE CASES =====
echo -e "\n${YELLOW}=== EDGE CASE TESTS ===${NC}"

run_test "Long command line" "echo $(printf 'a%.0s' {1..100})" "" "0"
run_test "Multiple spaces" "echo     hello     world" "" "0"
run_test "Quotes in command" "echo 'hello world'" "" "0"
run_test "Double quotes" 'echo "hello world"' "" "0"

# ===== TESTS AVANZADOS =====
echo -e "\n${YELLOW}=== ADVANCED TESTS ===${NC}"

# CD con OLDPWD
echo -e "\n${YELLOW}Testing cd - functionality${NC}"
echo -e "mkdir test_dir\ncd test_dir\ncd ..\ncd -\npwd" > cd_test.tmp
run_interactive_test "cd - functionality" "$(cat cd_test.tmp)"
rm -rf test_dir cd_test.tmp

# Export sin argumentos (debe mostrar variables ordenadas)
echo -e "\n${YELLOW}Testing export without arguments (should be sorted)${NC}"
echo -e "export TEST_Z=last\nexport TEST_A=first\nexport" > export_test.tmp
run_interactive_test "export sorted output" "$(cat export_test.tmp)"
rm -f export_test.tmp

# Combinación compleja
echo -e "\n${YELLOW}Testing complex combination${NC}"
echo -e "export TEST=value\necho \$TEST | cat > output.txt\ncat output.txt" > complex_test.tmp
run_interactive_test "complex combination" "$(cat complex_test.tmp)"
rm -f complex_test.tmp output.txt

# ===== CLEANUP =====
echo -e "\n${YELLOW}=== CLEANUP ===${NC}"
rm -f test_file.txt append_file.txt output.txt

# ===== RESULTADOS =====
echo -e "\n${YELLOW}=== TEST RESULTS ===${NC}"
echo -e "Passed: ${GREEN}$PASSED${NC}"
echo -e "Failed: ${RED}$FAILED${NC}"
echo -e "Total: $((PASSED + FAILED))"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
    exit 0
else
    echo -e "\n${RED}❌ Some tests failed. Check the output above.${NC}"
    exit 1
fi
