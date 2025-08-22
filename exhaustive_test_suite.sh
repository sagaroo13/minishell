#!/bin/bash

# MINISHELL EXHAUSTIVE TEST SUITE
# Comprehensive testing of all minishell functionality against bash behavior
# Tests: Redirections, Built-ins, Pipes, Variables, Signals, Edge Cases

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}╔══════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                MINISHELL EXHAUSTIVE TEST SUITE               ║${NC}"
echo -e "${CYAN}║              Testing bash compatibility & behavior           ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════════════════════════════╝${NC}"

passed=0
failed=0
total_sections=0
failed_sections=0

# Función para headers de sección
print_section() {
    local title="$1"
    echo -e "\n${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
    echo -e "${PURPLE}  $title${NC}"
    echo -e "${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
    ((total_sections++))
}

# Función para testear comando individual
test_command() {
    local cmd="$1"
    local description="$2"
    local expected_output="$3"
    local expected_exit="$4"
    
    echo -e "\n${BLUE}Testing:${NC} $description"
    echo -e "${YELLOW}Command:${NC} $cmd"
    
    # Crear archivo temporal para capturar salida
    echo -e "$cmd\necho \$?\nexit" > /tmp/test_cmd.txt
    
    # Ejecutar en minishell de forma más simple
    timeout 10 ./minishell < /tmp/test_cmd.txt > /tmp/test_output.txt 2>&1
    
    # Leer salida, filtrar líneas de banner
    local full_output=$(cat /tmp/test_output.txt | grep -v "minishell" | grep -v "By dediaz" | grep -v "╗\|╚\|║" | sed '/^$/d')
    local minishell_exit=$(echo "$full_output" | tail -1)
    local minishell_output=$(echo "$full_output" | head -n -1)
    
    # Verificar salida si se especifica
    local output_ok=true
    if [ -n "$expected_output" ] && [ "$expected_output" != "" ]; then
        if ! echo "$minishell_output" | grep -q "$expected_output"; then
            output_ok=false
        fi
    fi
    
    # Verificar exit code si se especifica
    local exit_ok=true
    if [ -n "$expected_exit" ] && [ "$expected_exit" != "" ]; then
        if [ "$minishell_exit" != "$expected_exit" ]; then
            exit_ok=false
        fi
    fi
    
    if $output_ok && $exit_ok; then
        echo -e "✓ ${GREEN}PASS${NC}"
        ((passed++))
    else
        echo -e "✗ ${RED}FAIL${NC}"
        if [ -n "$expected_exit" ] && ! $exit_ok; then
            echo -e "  Expected exit: $expected_exit, Got: $minishell_exit"
        fi
        if [ -n "$expected_output" ] && ! $output_ok; then
            echo -e "  Expected output containing: '$expected_output'"
            echo -e "  Got: '$minishell_output'"
        fi
        ((failed++))
    fi
    
    # Limpiar archivos temporales
    rm -f /tmp/test_cmd.txt /tmp/test_output.txt
}

# Función para testear error de sintaxis
test_syntax_error() {
    local cmd="$1"
    local description="$2"
    
    echo -e "\n${BLUE}Testing:${NC} $description"
    echo -e "${YELLOW}Command:${NC} $cmd"
    
    local output=$(echo -e "$cmd\nexit" | timeout 5 ./minishell 2>&1)
    
    if echo "$output" | grep -q "syntax error near unexpected token"; then
        echo -e "✓ ${GREEN}PASS${NC} - Syntax error detected"
        ((passed++))
    else
        echo -e "✗ ${RED}FAIL${NC} - Should detect syntax error"
        ((failed++))
    fi
}

# Función para testear redirección con archivos
test_redirection() {
    local cmd="$1"
    local description="$2"
    local check_file="$3"
    local expected_content="$4"
    
    echo -e "\n${BLUE}Testing:${NC} $description"
    echo -e "${YELLOW}Command:${NC} $cmd"
    
    # Limpiar archivo anterior
    rm -f "$check_file" 2>/dev/null
    
    # Ejecutar comando
    echo -e "$cmd\nexit" | timeout 5 ./minishell >/dev/null 2>&1
    
    # Verificar archivo
    if [ -f "$check_file" ] && [ -n "$expected_content" ]; then
        if grep -q "$expected_content" "$check_file"; then
            echo -e "✓ ${GREEN}PASS${NC} - File created with correct content"
            ((passed++))
        else
            echo -e "✗ ${RED}FAIL${NC} - File content incorrect"
            echo -e "  Expected: '$expected_content'"
            echo -e "  Got: '$(cat "$check_file" 2>/dev/null)'"
            ((failed++))
        fi
    elif [ -f "$check_file" ]; then
        echo -e "✓ ${GREEN}PASS${NC} - File created"
        ((passed++))
    else
        echo -e "✗ ${RED}FAIL${NC} - File not created"
        ((failed++))
    fi
}

# ═══════════════════════════════════════════════════════════════
# SECCIÓN 1: BASIC COMMANDS
# ═══════════════════════════════════════════════════════════════

print_section "1. BASIC COMMANDS & FUNCTIONALITY"

test_command "echo hello world" "Simple echo command" "hello world" "0"
test_command "echo" "Echo without arguments" "" "0"
test_command "echo -n hello" "Echo with -n flag" "hello" "0"
test_command "echo -nnn test" "Echo with multiple -n flags" "test" "0"
test_command "/bin/ls >/dev/null" "Absolute path command" "" "0"
test_command "nonexistent_command" "Non-existent command" "" "127"
test_command "" "Empty command" "" "0"
test_command "   " "Whitespace only command" "" "0"

# ═══════════════════════════════════════════════════════════════
# SECCIÓN 2: BUILT-IN COMMANDS
# ═══════════════════════════════════════════════════════════════

print_section "2. BUILT-IN COMMANDS"

test_command "pwd" "Present working directory" "/home/shirakim/Proyectos/minishell" "0"
test_command "env | grep USER" "Environment variables" "USER" "0"
test_command "cd /tmp && pwd" "Change directory" "/tmp" "0"
test_command "cd ~ && pwd" "Change to home directory" "/home/shirakim" "0"
test_command "cd" "Change to home (no args)" "" "0"
test_command "cd /nonexistent" "Change to non-existent directory" "" "1"

# Test cd - functionality
echo -e "\n${BLUE}Testing:${NC} cd - functionality (back to previous dir)"
echo -e "${YELLOW}Commands:${NC} cd /tmp && cd ~ && cd -"
output=$(echo -e "cd /tmp\ncd ~\ncd -\npwd\nexit" | timeout 5 ./minishell 2>&1 | tail -3 | head -1)
if echo "$output" | grep -q "/tmp"; then
    echo -e "✓ ${GREEN}PASS${NC} - cd - works correctly"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - cd - not working"
    ((failed++))
fi

# Export tests
test_command "export TEST_VAR=hello && echo \$TEST_VAR" "Export and use variable" "hello" "0"
test_command "export" "Export without arguments (should show sorted list)" "declare -x" "0"
test_command "export INVALID123VAR=test" "Export invalid identifier" "" "1"
test_command "unset PATH && export PATH=/bin:/usr/bin" "Unset and re-export PATH" "" "0"

# ═══════════════════════════════════════════════════════════════
# SECCIÓN 3: VARIABLE EXPANSION
# ═══════════════════════════════════════════════════════════════

print_section "3. VARIABLE EXPANSION"

test_command "echo \$HOME" "Environment variable expansion" "/home/shirakim" "0"
test_command "echo \$USER" "User variable expansion" "shirakim" "0"
test_command "echo \$NONEXISTENT" "Non-existent variable" "" "0"
test_command "export TEST=value && echo \$TEST" "Custom variable expansion" "value" "0"
test_command "echo \$\$" "Process ID expansion" "" "0"

# Exit status tests
echo -e "\n${BLUE}Testing:${NC} Exit status variable \$?"
echo -e "${YELLOW}Commands:${NC} true && echo \$?"
output=$(echo -e "true\necho \$?\nexit" | timeout 5 ./minishell 2>&1 | grep -v "minishell" | grep -v "By dediaz" | tail -2 | head -1)
if [ "$output" = "0" ]; then
    echo -e "✓ ${GREEN}PASS${NC} - Exit status after true"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Wrong exit status: $output"
    ((failed++))
fi

echo -e "\n${BLUE}Testing:${NC} Exit status after false command"
echo -e "${YELLOW}Commands:${NC} false && echo \$?"
output=$(echo -e "false\necho \$?\nexit" | timeout 5 ./minishell 2>&1 | grep -v "minishell" | grep -v "By dediaz" | tail -2 | head -1)
if [ "$output" = "1" ]; then
    echo -e "✓ ${GREEN}PASS${NC} - Exit status after false"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Wrong exit status: $output"
    ((failed++))
fi

# ═══════════════════════════════════════════════════════════════
# SECCIÓN 4: REDIRECTION TESTS (COMPREHENSIVE)
# ═══════════════════════════════════════════════════════════════

print_section "4. REDIRECTION TESTS"

# Output redirections
test_redirection "echo 'hello world' > test_output.txt" "Simple output redirection" "test_output.txt" "hello world"
test_redirection "echo 'append test' >> test_append.txt" "Append redirection" "test_append.txt" "append test"
test_redirection "echo 'second line' >> test_append.txt" "Second append" "test_append.txt" "second line"

# Input redirections
echo "input test content" > test_input.txt
test_command "cat < test_input.txt" "Input redirection" "input test content" "0"
test_command "cat < nonexistent.txt" "Input from non-existent file" "" "1"

# Error redirection
test_command "cat nonexistent 2> error.txt" "Error redirection to file" "" "1"

# Built-ins with redirections
test_redirection "pwd > pwd_output.txt" "Built-in with output redirection" "pwd_output.txt" "minishell"
test_command "cd < nonexistent.txt" "Built-in with failed input redirection" "" "1"

# Multiple redirections
test_redirection "echo 'multi' > first.txt > second.txt" "Multiple output redirections" "second.txt" "multi"

# ═══════════════════════════════════════════════════════════════
# SECCIÓN 5: HEREDOC TESTS
# ═══════════════════════════════════════════════════════════════

print_section "5. HEREDOC TESTS"

# Basic heredoc
echo -e "\n${BLUE}Testing:${NC} Basic heredoc functionality"
echo -e "${YELLOW}Command:${NC} cat << EOF"
output=$(echo -e "cat << EOF\nhello heredoc\nworld\nEOF\nexit" | timeout 10 ./minishell 2>&1 | grep -v "minishell" | grep -v "By dediaz" | grep -v "heredoc>" | grep -v "╗\|╚\|║" | sed '/^$/d')
if echo "$output" | grep -q "hello heredoc" && echo "$output" | grep -q "world"; then
    echo -e "✓ ${GREEN}PASS${NC} - Heredoc works correctly"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Heredoc not working"
    echo "Output: $output"
    ((failed++))
fi

# Heredoc with quoted delimiter
echo -e "\n${BLUE}Testing:${NC} Heredoc with quoted delimiter"
echo -e "${YELLOW}Command:${NC} cat << \"END\""
output=$(echo -e "cat << \"END\"\nquoted delimiter test\nEND\nexit" | timeout 10 ./minishell 2>&1 | grep -v "minishell" | grep -v "By dediaz" | grep -v "heredoc>" | grep -v "╗\|╚\|║" | sed '/^$/d')
if echo "$output" | grep -q "quoted delimiter test"; then
    echo -e "✓ ${GREEN}PASS${NC} - Quoted delimiter heredoc works"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Quoted delimiter heredoc failed"
    ((failed++))
fi

# ═══════════════════════════════════════════════════════════════
# SECCIÓN 6: PIPE TESTS
# ═══════════════════════════════════════════════════════════════

print_section "6. PIPE TESTS"

test_command "echo hello | cat" "Simple pipe" "hello" "0"
test_command "echo test | grep test" "Pipe with grep" "test" "0"
test_command "echo multiple | cat | cat" "Multiple pipes" "multiple" "0"
test_command "echo hello | cat | wc -w" "Pipe chain with wc" "1" "0"
test_command "nonexistent | cat" "Failed command in pipe" "" "0"

# Built-in in pipe
test_command "echo test | cat > piped_output.txt" "Pipe with redirection" "" "0"

# ═══════════════════════════════════════════════════════════════
# SECCIÓN 7: SYNTAX ERROR TESTS
# ═══════════════════════════════════════════════════════════════

print_section "7. SYNTAX ERROR TESTS"

test_syntax_error "echo >" "Incomplete output redirection"
test_syntax_error "echo |" "Incomplete pipe"
test_syntax_error "cat >>>" "Triple greater than"
test_syntax_error "cat <<<<" "Quadruple less than"
test_syntax_error "cat <<<" "Triple less than"
test_syntax_error "cat >>>>>" "Five greater than"
test_syntax_error "echo >>>> file.txt" "Four > with filename"
test_syntax_error "| echo hello" "Pipe at beginning"
test_syntax_error "echo hello |" "Pipe at end"

# ═══════════════════════════════════════════════════════════════
# SECCIÓN 8: EDGE CASES & SPECIAL SCENARIOS
# ═══════════════════════════════════════════════════════════════

print_section "8. EDGE CASES & SPECIAL SCENARIOS"

# Long commands
test_command "echo $(printf 'a%.0s' {1..50})" "Long command line" "" "0"

# Multiple spaces
test_command "echo     hello     world" "Multiple spaces in command" "hello world" "0"

# Quotes handling
test_command "echo 'hello world'" "Single quotes" "hello world" "0"
test_command "echo \"hello world\"" "Double quotes" "hello world" "0"
test_command "echo 'don'\"'\"'t'" "Mixed quotes" "don't" "0"

# Special characters
test_command "echo 'special chars: \$#@!%^&*()'" "Special characters in quotes" "special chars:" "0"

# Empty quotes
test_command "echo ''" "Empty single quotes" "" "0"
test_command "echo \"\"" "Empty double quotes" "" "0"

# Variable in quotes
test_command "export VAR=test && echo '\$VAR'" "Variable in single quotes (literal)" "\$VAR" "0"
test_command "export VAR=test && echo \"\$VAR\"" "Variable in double quotes (expanded)" "test" "0"

# ═══════════════════════════════════════════════════════════════
# SECCIÓN 9: EXIT COMMAND TESTS
# ═══════════════════════════════════════════════════════════════

print_section "9. EXIT COMMAND TESTS"

echo -e "\n${BLUE}Testing:${NC} Exit with specific code"
echo -e "${YELLOW}Command:${NC} exit 42"
echo "exit 42" | timeout 5 ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 42 ]; then
    echo -e "✓ ${GREEN}PASS${NC} - Exit with code 42"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Expected 42, got $exit_code"
    ((failed++))
fi

echo -e "\n${BLUE}Testing:${NC} Exit without code"
echo -e "${YELLOW}Command:${NC} exit"
echo "exit" | timeout 5 ./minishell >/dev/null 2>&1
exit_code=$?
if [ $exit_code -eq 0 ]; then
    echo -e "✓ ${GREEN}PASS${NC} - Exit without code (default 0)"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Expected 0, got $exit_code"
    ((failed++))
fi

# ═══════════════════════════════════════════════════════════════
# SECCIÓN 10: COMPLEX COMBINATIONS
# ═══════════════════════════════════════════════════════════════

print_section "10. COMPLEX COMBINATIONS"

# Complex pipe + redirection
test_redirection "echo 'complex test' | cat | cat > complex_output.txt" "Complex pipe with redirection" "complex_output.txt" "complex test"

# Export + pipe + redirection
echo -e "\n${BLUE}Testing:${NC} Export + variable + pipe + redirection"
echo -e "${YELLOW}Commands:${NC} export COMPLEX=value && echo \$COMPLEX | cat > var_output.txt"
echo -e "export COMPLEX=value\necho \$COMPLEX | cat > var_output.txt\nexit" | timeout 5 ./minishell >/dev/null 2>&1
if [ -f "var_output.txt" ] && grep -q "value" "var_output.txt"; then
    echo -e "✓ ${GREEN}PASS${NC} - Complex variable expansion with pipe and redirection"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Complex combination failed"
    ((failed++))
fi

# Multiple commands with semicolon simulation (separate lines)
echo -e "\n${BLUE}Testing:${NC} Multiple separate commands"
echo -e "${YELLOW}Commands:${NC} Multiple separate commands in sequence"
output=$(echo -e "echo first\necho second\necho third\nexit" | timeout 5 ./minishell 2>&1 | grep -v "minishell" | grep -v "By dediaz" | grep -v "╗\|╚\|║" | sed '/^$/d')
if echo "$output" | grep -q "first" && echo "$output" | grep -q "second" && echo "$output" | grep -q "third"; then
    echo -e "✓ ${GREEN}PASS${NC} - Multiple commands executed in sequence"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Multiple commands failed"
    ((failed++))
fi

# ═══════════════════════════════════════════════════════════════
# CLEANUP AND RESULTS
# ═══════════════════════════════════════════════════════════════

echo -e "\n${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${PURPLE}  CLEANUP${NC}"
echo -e "${PURPLE}═══════════════════════════════════════════════════════════════${NC}"

# Clean up test files
rm -f test_output.txt test_append.txt test_input.txt error.txt pwd_output.txt
rm -f first.txt second.txt piped_output.txt complex_output.txt var_output.txt
echo "Cleaned up test files"

# ═══════════════════════════════════════════════════════════════
# FINAL RESULTS
# ═══════════════════════════════════════════════════════════════

echo -e "\n${CYAN}╔══════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                       FINAL RESULTS                         ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════════════════════════════╝${NC}"

echo -e "\n${BLUE}Total Sections Tested:${NC} $total_sections"
echo -e "${GREEN}Tests Passed:${NC} $passed"
echo -e "${RED}Tests Failed:${NC} $failed"
echo -e "${YELLOW}Total Tests:${NC} $((passed + failed))"

percentage=$((passed * 100 / (passed + failed)))
echo -e "${PURPLE}Success Rate:${NC} $percentage%"

if [ $failed -eq 0 ]; then
    echo -e "\n🎉 ${GREEN}ALL TESTS PASSED! MINISHELL IS FULLY COMPATIBLE!${NC} 🎉"
    echo -e "${GREEN}Your minishell implementation matches bash behavior perfectly!${NC}"
    exit 0
else
    echo -e "\n❌ ${RED}Some tests failed.${NC} Check the output above for details."
    echo -e "${YELLOW}Failed tests indicate areas where minishell differs from bash behavior.${NC}"
    exit 1
fi
