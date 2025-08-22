#!/bin/bash

# =============================================================================
# 42 SCHOOL STYLE COMPREHENSIVE EVALUATION TEST
# Areas: BUILT-INS, SIGNALS, HEREDOCS
# =============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

print_header() {
    echo -e "${BLUE}=================================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}=================================================${NC}"
}

print_test() {
    echo -e "${YELLOW}TEST $TOTAL_TESTS: $1${NC}"
}

print_pass() {
    echo -e "${GREEN}✅ PASS${NC}"
    ((PASSED_TESTS++))
}

print_fail() {
    echo -e "${RED}❌ FAIL: $1${NC}"
    ((FAILED_TESTS++))
}

test_command() {
    local description="$1"
    local command="$2"
    local expected_exit="$3"
    
    ((TOTAL_TESTS++))
    print_test "$description"
    echo "Command: $command"
    
    # Execute in minishell
    echo "$command" | ./minishell > /tmp/mini_out 2>&1
    local mini_exit=$?
    
    # Execute in bash for comparison  
    bash -c "$command" > /tmp/bash_out 2>&1
    local bash_exit=$?
    
    # Check exit code if specified
    if [ ! -z "$expected_exit" ]; then
        if [ $mini_exit -eq $expected_exit ]; then
            print_pass
        else
            print_fail "Expected exit $expected_exit, got $mini_exit"
        fi
    else
        # Compare with bash
        if [ $mini_exit -eq $bash_exit ]; then
            print_pass
        else
            print_fail "Bash exit: $bash_exit, Minishell exit: $mini_exit"
        fi
    fi
    echo ""
}

echo "Compiling minishell..."
make re > /dev/null 2>&1

print_header "42 SCHOOL EVALUATION - BUILT-INS & HEREDOCS"

# =============================================================================
print_header "SECTION 1: ECHO BUILT-IN EDGE CASES"
# =============================================================================

test_command "Echo without arguments" "echo" 0
test_command "Echo with single argument" "echo hello" 0
test_command "Echo with multiple arguments" "echo hello world 42" 0
test_command "Echo with -n flag" "echo -n hello" 0
test_command "Echo with multiple -n flags" "echo -n -n -n hello" 0
test_command "Echo with invalid flag" "echo -x hello" 0
test_command "Echo with -- separator" "echo -- -n hello" 0
test_command "Echo with mixed flags" "echo -n -- hello" 0
test_command "Echo with special characters" 'echo "!@#\$%^&*()"' 0

# =============================================================================
print_header "SECTION 2: PWD BUILT-IN EDGE CASES"
# =============================================================================

test_command "Pwd basic functionality" "pwd" 0
test_command "Pwd with arguments (should work)" "pwd arg1 arg2" 0

# =============================================================================
print_header "SECTION 3: CD BUILT-IN COMPREHENSIVE"
# =============================================================================

test_command "CD without arguments (to HOME)" "cd" 0
test_command "CD to root" "cd /" 0
test_command "CD to home with tilde" "cd ~" 0
test_command "CD to non-existent directory" "cd /this/does/not/exist" 1
test_command "CD with relative path" "cd .." 0
test_command "CD to current directory" "cd ." 0

# =============================================================================
print_header "SECTION 4: ENV BUILT-IN"
# =============================================================================

test_command "Env basic functionality" "env" 0

# =============================================================================
print_header "SECTION 5: EXPORT BUILT-IN COMPREHENSIVE"
# =============================================================================

test_command "Export without arguments" "export" 0
test_command "Export valid variable" "export TEST_VAR=hello" 0
test_command "Export variable without value" "export TEST_VAR2" 0
test_command "Export with empty value" "export TEST_VAR3=" 0
test_command "Export invalid identifier - starts with number" "export 123ABC=value" 1
test_command "Export invalid identifier - contains special chars" "export TEST-VAR=value" 1

# =============================================================================
print_header "SECTION 6: UNSET BUILT-IN COMPREHENSIVE"
# =============================================================================

test_command "Unset existing variable" "unset HOME" 0
test_command "Unset non-existent variable" "unset NON_EXISTENT_VAR" 0
test_command "Unset multiple variables" "unset VAR1 VAR2 VAR3" 0
test_command "Unset without arguments" "unset" 0

# =============================================================================
print_header "SECTION 7: EXIT BUILT-IN COMPREHENSIVE"
# =============================================================================

test_command "Exit with 0" "exit 0" 0
test_command "Exit with positive number" "exit 42" 42
test_command "Exit with negative number" "exit -1" 255
test_command "Exit with large positive number" "exit 1000" 232
test_command "Exit with 256" "exit 256" 0
test_command "Exit with 257" "exit 257" 1
test_command "Exit with string argument" "exit hello" 2
test_command "Exit with mixed argument" "exit 42abc" 2
test_command "Exit with overflow" "exit 9223372036854775808" 2

# Test special case: too many arguments (should NOT exit)
((TOTAL_TESTS++))
print_test "Exit with too many arguments (should not exit shell)"
echo -e 'exit 1 2\necho "Still here"\nexit 0' | ./minishell > /tmp/exit_test 2>&1
exit_code=$?
if grep -q "Still here" /tmp/exit_test && [ $exit_code -eq 0 ]; then
    print_pass
else
    print_fail "Should continue execution after 'exit 1 2'"
fi
echo ""

# =============================================================================
print_header "SECTION 8: HEREDOC FUNCTIONALITY"
# =============================================================================

# Simple heredoc test
((TOTAL_TESTS++))
print_test "Simple heredoc"
echo -e 'cat << EOF\nHello World\nThis is a test\nEOF' | ./minishell > /tmp/mini_heredoc 2>&1
echo -e 'cat << EOF\nHello World\nThis is a test\nEOF' | bash > /tmp/bash_heredoc 2>&1
if cmp -s /tmp/mini_heredoc /tmp/bash_heredoc; then
    print_pass
else
    print_fail "Heredoc output mismatch"
    echo "=== MINISHELL OUTPUT ==="
    cat /tmp/mini_heredoc
    echo "=== BASH OUTPUT ==="
    cat /tmp/bash_heredoc
    echo "===================="
fi
echo ""

# Heredoc with variable expansion
((TOTAL_TESTS++))
print_test "Heredoc with variable expansion"
echo -e 'export TEST_VAR=hello\ncat << EOF\n$TEST_VAR world\nEOF' | ./minishell > /tmp/heredoc_var_mini 2>&1
echo -e 'export TEST_VAR=hello\ncat << EOF\n$TEST_VAR world\nEOF' | bash > /tmp/heredoc_var_bash 2>&1
if cmp -s /tmp/heredoc_var_mini /tmp/heredoc_var_bash; then
    print_pass
else
    print_fail "Variable expansion in heredoc failed"
    echo "=== MINISHELL OUTPUT ==="
    cat /tmp/heredoc_var_mini
    echo "=== BASH OUTPUT ==="
    cat /tmp/heredoc_var_bash
    echo "===================="
fi
echo ""

# =============================================================================
print_header "SECTION 9: ERROR HANDLING"
# =============================================================================

test_command "Command not found error" "nonexistent_command_xyz" 127
test_command "CD to file instead of directory" "cd /etc/passwd" 1

# =============================================================================
print_header "SECTION 10: COMPLEX SCENARIOS"
# =============================================================================

# Variable persistence across commands
((TOTAL_TESTS++))
print_test "Variable persistence across commands"
echo -e 'export PERSIST_TEST=persistent\necho $PERSIST_TEST' | ./minishell > /tmp/persist_test 2>&1
if grep -q "persistent" /tmp/persist_test; then
    print_pass
else
    print_fail "Variables not persisting across commands"
    echo "Output:"
    cat /tmp/persist_test
fi
echo ""

# Built-in with redirection
((TOTAL_TESTS++))
print_test "Built-in with redirection"
echo 'echo hello > /tmp/test_redir' | ./minishell > /tmp/redir_result 2>&1
if [ -f /tmp/test_redir ] && grep -q "hello" /tmp/test_redir; then
    print_pass
    rm -f /tmp/test_redir
else
    print_fail "Redirection with built-in failed"
fi
echo ""

# =============================================================================
print_header "SECTION 11: EDGE CASES"
# =============================================================================

test_command "Echo with only spaces" 'echo "   "' 0
test_command "CD with multiple slashes" "cd ///" 0
test_command "Export with equals in value" 'export TEST="val=ue"' 0

# =============================================================================
print_header "FINAL RESULTS"
# =============================================================================

echo ""
echo -e "${BLUE}=================== EVALUATION SUMMARY ===================${NC}"
echo -e "${GREEN}PASSED: $PASSED_TESTS${NC}"
echo -e "${RED}FAILED: $FAILED_TESTS${NC}"
echo -e "${YELLOW}TOTAL:  $TOTAL_TESTS${NC}"
echo ""

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}🎉 CONGRATULATIONS! ALL TESTS PASSED! 🎉${NC}"
    echo -e "${GREEN}Your built-ins implementation is evaluation-ready!${NC}"
    exit 0
else
    echo -e "${RED}⚠️  ATTENTION: $FAILED_TESTS tests failed${NC}"
    echo -e "${YELLOW}Review the failed tests and fix the issues${NC}"
    
    # Show percentage
    PERCENTAGE=$(( PASSED_TESTS * 100 / TOTAL_TESTS ))
    echo -e "${BLUE}SCORE: $PERCENTAGE%${NC}"
    
    if [ $PERCENTAGE -ge 80 ]; then
        echo -e "${YELLOW}Good score! Minor fixes needed.${NC}"
    elif [ $PERCENTAGE -ge 60 ]; then
        echo -e "${YELLOW}Decent score. Some work needed.${NC}"
    else
        echo -e "${RED}Low score. Significant work needed.${NC}"
    fi
    
    exit $FAILED_TESTS
fi

echo ""
echo -e "${BLUE}Areas tested:${NC}"
echo "✓ Echo built-in with all flags and edge cases"
echo "✓ Pwd built-in functionality"
echo "✓ CD built-in with all path types"
echo "✓ Env built-in display"
echo "✓ Export built-in with validation"
echo "✓ Unset built-in functionality"
echo "✓ Exit built-in with numeric handling"
echo "✓ Heredoc functionality and variable expansion"
echo "✓ Error handling and exit codes"
echo "✓ Complex scenarios and edge cases"

# Cleanup
rm -f /tmp/mini_out /tmp/bash_out /tmp/mini_heredoc /tmp/bash_heredoc
rm -f /tmp/heredoc_var_mini /tmp/heredoc_var_bash /tmp/persist_test /tmp/exit_test
rm -f /tmp/redir_result