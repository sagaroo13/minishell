#!/bin/bash

# MINISHELL FINAL VALIDATION TEST
# Tests principales de funcionalidad y compatibilidad con bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${CYAN}╔══════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                 MINISHELL FINAL VALIDATION                   ║${NC}"
echo -e "${CYAN}║              Comprehensive functionality test                ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════════════════════════════╝${NC}"

passed=0
failed=0

# Función para test simple
simple_test() {
    local description="$1"
    local cmd="$2"
    local expected_exit="$3"
    
    echo -e "\n${BLUE}Testing:${NC} $description"
    echo -e "${YELLOW}Command:${NC} $cmd"
    
    # Ejecutar comando
    echo -e "$cmd\nexit" | timeout 5 ./minishell >/dev/null 2>&1
    local actual_exit=$?
    
    if [ "$actual_exit" -eq "$expected_exit" ]; then
        echo -e "✓ ${GREEN}PASS${NC} - Exit code: $actual_exit"
        ((passed++))
    else
        echo -e "✗ ${RED}FAIL${NC} - Expected: $expected_exit, Got: $actual_exit"
        ((failed++))
    fi
}

# Función para test de archivo
file_test() {
    local description="$1" 
    local cmd="$2"
    local file="$3"
    local expected_content="$4"
    
    echo -e "\n${BLUE}Testing:${NC} $description"
    echo -e "${YELLOW}Command:${NC} $cmd"
    
    rm -f "$file" 2>/dev/null
    echo -e "$cmd\nexit" | timeout 5 ./minishell >/dev/null 2>&1
    
    if [ -f "$file" ] && grep -q "$expected_content" "$file" 2>/dev/null; then
        echo -e "✓ ${GREEN}PASS${NC} - File created with correct content"
        ((passed++))
    else
        echo -e "✗ ${RED}FAIL${NC} - File not created or wrong content"
        ((failed++))
    fi
}

# Función para test de error de sintaxis
syntax_test() {
    local description="$1"
    local cmd="$2"
    
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

echo -e "\n${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${PURPLE}  1. BASIC COMMAND TESTS${NC}"
echo -e "${PURPLE}═══════════════════════════════════════════════════════════════${NC}"

simple_test "Echo command" "echo hello world" 0
simple_test "Echo with -n flag" "echo -n test" 0
simple_test "PWD command" "pwd" 0
simple_test "ENV command" "env | head -1" 0
simple_test "Non-existent command" "nonexistent_cmd" 127
simple_test "Empty command" "" 0

echo -e "\n${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${PURPLE}  2. BUILT-IN COMMANDS${NC}"
echo -e "${PURPLE}═══════════════════════════════════════════════════════════════${NC}"

simple_test "Export variable" "export TEST_VAR=hello" 0
simple_test "Export without args" "export" 0
simple_test "Invalid export" "export 123VAR=test" 1
simple_test "Unset variable" "unset TEST_VAR" 0
simple_test "CD to valid directory" "cd /tmp" 0
simple_test "CD to invalid directory" "cd /nonexistent" 1

echo -e "\n${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${PURPLE}  3. REDIRECTION TESTS${NC}"
echo -e "${PURPLE}═══════════════════════════════════════════════════════════════${NC}"

file_test "Output redirection" "echo 'test output' > test_out.txt" "test_out.txt" "test output"
file_test "Append redirection" "echo 'append test' >> test_append.txt" "test_append.txt" "append test"
file_test "PWD redirection" "pwd > pwd_out.txt" "pwd_out.txt" "minishell"
simple_test "Input from non-existent file" "cat < nonexistent.txt" 1

echo -e "\n${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${PURPLE}  4. PIPE TESTS${NC}"
echo -e "${PURPLE}═══════════════════════════════════════════════════════════════${NC}"

simple_test "Simple pipe" "echo hello | cat" 0
simple_test "Multiple pipes" "echo test | cat | cat" 0
simple_test "Pipe with grep" "echo test | grep test" 0
file_test "Pipe with redirection" "echo 'piped' | cat > pipe_out.txt" "pipe_out.txt" "piped"

echo -e "\n${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${PURPLE}  5. SYNTAX ERROR VALIDATION${NC}"
echo -e "${PURPLE}═══════════════════════════════════════════════════════════════${NC}"

syntax_test "Triple greater than" "cat >>>"
syntax_test "Quadruple less than" "cat <<<<<"
syntax_test "Five consecutive >" "cat >>>>>"
syntax_test "Many consecutive <" "cat <<<<<<<<"

echo -e "\n${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${PURPLE}  6. EXIT CODE TESTS${NC}"
echo -e "${PURPLE}═══════════════════════════════════════════════════════════════${NC}"

echo -e "\n${BLUE}Testing:${NC} Exit with specific code"
echo "exit 42" | timeout 5 ./minishell >/dev/null 2>&1
if [ $? -eq 42 ]; then
    echo -e "✓ ${GREEN}PASS${NC} - Exit code 42"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Wrong exit code"
    ((failed++))
fi

echo -e "\n${BLUE}Testing:${NC} Exit without arguments"
echo "exit" | timeout 5 ./minishell >/dev/null 2>&1
if [ $? -eq 0 ]; then
    echo -e "✓ ${GREEN}PASS${NC} - Default exit code 0"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Wrong default exit code"
    ((failed++))
fi

echo -e "\n${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${PURPLE}  7. SPECIAL FUNCTIONALITY${NC}"
echo -e "${PURPLE}═══════════════════════════════════════════════════════════════${NC}"

# Test cd - functionality
echo -e "\n${BLUE}Testing:${NC} cd - functionality"
output=$(echo -e "cd /tmp\ncd /\ncd -\npwd\nexit" | timeout 5 ./minishell 2>&1 | grep "/tmp")
if [ -n "$output" ]; then
    echo -e "✓ ${GREEN}PASS${NC} - cd - works correctly"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - cd - not working"
    ((failed++))
fi

# Test variable expansion
echo -e "\n${BLUE}Testing:${NC} Variable expansion"
echo "test content" > var_test_input.txt
output=$(echo -e "export VAR=var_test_input.txt\ncat < \$VAR\nexit" | timeout 5 ./minishell 2>&1 | grep "test content")
if [ -n "$output" ]; then
    echo -e "✓ ${GREEN}PASS${NC} - Variable expansion works"
    ((passed++))
else
    echo -e "✗ ${RED}FAIL${NC} - Variable expansion failed"
    ((failed++))
fi

echo -e "\n${PURPLE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${PURPLE}  CLEANUP${NC}"
echo -e "${PURPLE}═══════════════════════════════════════════════════════════════${NC}"

rm -f test_out.txt test_append.txt pwd_out.txt pipe_out.txt var_test_input.txt
echo "Test files cleaned up"

echo -e "\n${CYAN}╔══════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                       FINAL RESULTS                         ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════════════════════════════╝${NC}"

echo -e "\n${GREEN}Tests Passed:${NC} $passed"
echo -e "${RED}Tests Failed:${NC} $failed"
echo -e "${YELLOW}Total Tests:${NC} $((passed + failed))"

percentage=$((passed * 100 / (passed + failed)))
echo -e "${PURPLE}Success Rate:${NC} $percentage%"

if [ $failed -eq 0 ]; then
    echo -e "\n🎉 ${GREEN}ALL TESTS PASSED! MINISHELL IS FULLY FUNCTIONAL!${NC} 🎉"
    echo -e "${GREEN}✓ Redirection syntax validation${NC}"
    echo -e "${GREEN}✓ Built-in commands compatibility${NC}"
    echo -e "${GREEN}✓ Variable expansion${NC}"
    echo -e "${GREEN}✓ Pipe functionality${NC}"
    echo -e "${GREEN}✓ Error handling${NC}"
    echo -e "${GREEN}✓ cd - functionality${NC}"
    echo -e "${GREEN}✓ Exit code management${NC}"
    exit 0
else
    echo -e "\n❌ ${RED}Some tests failed.${NC}"
    exit 1
fi
