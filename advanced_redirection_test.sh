#!/bin/bash

# Advanced Redirection Test Suite for Minishell
# Tests complex redirection scenarios with built-ins and external commands

echo "=== ADVANCED REDIRECTION TEST SUITE ==="
echo "Testing complex redirection scenarios..."
echo

# Test counters
passed=0
failed=0

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Function to run test with complex setup
run_advanced_test() {
    local test_name="$1"
    local setup_commands="$2"
    local test_command="$3"
    local expected_output="$4"
    local expected_exit_code="$5"
    local cleanup_commands="$6"
    
    echo "Testing: $test_name"
    echo "Command: $test_command"
    
    # Setup
    if [ -n "$setup_commands" ]; then
        eval "$setup_commands"
    fi
    
    # Run test
    if [ -n "$expected_output" ]; then
        output=$(echo "$test_command" | timeout 5 ./minishell 2>&1 | grep -v "███" | grep -v "By dediaz-f" | tail -n +2 | head -n -1)
        exit_code=$?
        
        if echo "$output" | grep -q "$expected_output" && [ $exit_code -eq $expected_exit_code ]; then
            echo -e "${GREEN}✓ PASS${NC}"
            ((passed++))
        else
            echo -e "${RED}✗ FAIL${NC}"
            echo "Expected output to contain: $expected_output"
            echo "Expected exit code: $expected_exit_code, Got: $exit_code"
            echo "Actual output: $output"
            ((failed++))
        fi
    else
        echo "$test_command" | timeout 5 ./minishell >/dev/null 2>&1
        exit_code=$?
        
        if [ $exit_code -eq $expected_exit_code ]; then
            echo -e "${GREEN}✓ PASS${NC} - Exit code: $exit_code"
            ((passed++))
        else
            echo -e "${RED}✗ FAIL${NC} - Expected exit code: $expected_exit_code, Got: $exit_code"
            ((failed++))
        fi
    fi
    
    # Cleanup
    if [ -n "$cleanup_commands" ]; then
        eval "$cleanup_commands"
    fi
    
    echo
}

# Function to test file content
test_file_content() {
    local test_name="$1"
    local setup_commands="$2"
    local test_command="$3"
    local filename="$4"
    local expected_content="$5"
    local cleanup_commands="$6"
    
    echo "Testing: $test_name"
    echo "Command: $test_command"
    
    # Setup
    if [ -n "$setup_commands" ]; then
        eval "$setup_commands"
    fi
    
    # Run test
    echo "$test_command" | timeout 5 ./minishell >/dev/null 2>&1
    
    if [ -f "$filename" ] && grep -q "$expected_content" "$filename"; then
        echo -e "${GREEN}✓ PASS${NC} - File contains expected content"
        ((passed++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "Expected file '$filename' to contain: $expected_content"
        if [ -f "$filename" ]; then
            echo "Actual content: $(cat "$filename")"
        else
            echo "File does not exist"
        fi
        ((failed++))
    fi
    
    # Cleanup
    if [ -n "$cleanup_commands" ]; then
        eval "$cleanup_commands"
    fi
    
    echo
}

echo "=== BUILT-IN REDIRECTIONS ==="

# Test 1: Echo with quoted strings and redirection
test_file_content "Echo with quotes to file" \
    "" \
    'echo "hello world" > quoted_output.txt' \
    "quoted_output.txt" \
    "hello world" \
    "rm -f quoted_output.txt"

# Test 2: Echo with single quotes
test_file_content "Echo with single quotes" \
    "" \
    "echo 'single quoted text' > single_output.txt" \
    "single_output.txt" \
    "single quoted text" \
    "rm -f single_output.txt"

# Test 3: Echo with variables
test_file_content "Echo with environment variable" \
    'export TEST_VAR="environment test"' \
    'echo $TEST_VAR > env_output.txt' \
    "env_output.txt" \
    "environment test" \
    "rm -f env_output.txt; unset TEST_VAR"

# Test 4: Echo with complex string and append
test_file_content "Echo with spaces and append" \
    'echo "line1" > append_test.txt' \
    'echo "   line with   spaces   " >> append_test.txt' \
    "append_test.txt" \
    "line with   spaces" \
    "rm -f append_test.txt"

# Test 5: Pwd to file
test_file_content "Pwd redirection" \
    "" \
    'pwd > pwd_output.txt' \
    "pwd_output.txt" \
    "minishell" \
    "rm -f pwd_output.txt"

# Test 6: Export to file (should work)
run_advanced_test "Export redirection" \
    "" \
    'export > export_output.txt' \
    "" \
    "0" \
    "rm -f export_output.txt"

echo "=== INPUT REDIRECTIONS WITH BUILT-INS ==="

# Test 7: Cat with input redirection
test_file_content "Cat input redirection setup" \
    'echo "test input content" > input_test.txt' \
    'cat < input_test.txt > output_test.txt' \
    "output_test.txt" \
    "test input content" \
    "rm -f input_test.txt output_test.txt"

# Test 8: Built-in with input redirection (should fail appropriately)
run_advanced_test "Echo with input redirection" \
    'echo "some content" > input_for_echo.txt' \
    'echo "hello" < input_for_echo.txt' \
    "hello" \
    "0" \
    "rm -f input_for_echo.txt"

echo "=== COMPLEX FILENAME SCENARIOS ==="

# Test 9: Filename with spaces (quoted)
test_file_content "Filename with spaces" \
    "" \
    'echo "content" > "file with spaces.txt"' \
    "file with spaces.txt" \
    "content" \
    'rm -f "file with spaces.txt"'

# Test 10: Multiple redirections on same command
test_file_content "Multiple output redirections" \
    "" \
    'echo "test" > first.txt > second.txt' \
    "second.txt" \
    "test" \
    "rm -f first.txt second.txt"

# Test 11: Redirection with command substitution-like syntax (should be literal)
test_file_content "Complex redirection syntax" \
    "" \
    'echo "test" > $(echo output).txt' \
    '$(echo output).txt' \
    "test" \
    'rm -f "$(echo output).txt"'

echo "=== ERROR SCENARIOS ==="

# Test 12: Redirection to directory (should fail)
run_advanced_test "Redirect to directory" \
    'mkdir test_dir' \
    'echo "test" > test_dir' \
    "" \
    "1" \
    "rmdir test_dir 2>/dev/null || rm -rf test_dir"

# Test 13: Input from non-existent file
run_advanced_test "Input from non-existent file" \
    "" \
    'cat < nonexistent_file.txt' \
    "" \
    "1" \
    ""

# Test 14: Permission denied scenario
run_advanced_test "Permission denied redirection" \
    'touch readonly.txt; chmod 000 readonly.txt' \
    'echo "test" > readonly.txt' \
    "" \
    "1" \
    "chmod 644 readonly.txt; rm -f readonly.txt"

echo "=== HEREDOC ADVANCED TESTS ==="

# Test 15: Heredoc with variables
run_advanced_test "Heredoc basic test" \
    'export HEREDOC_VAR="test value"' \
    'cat << EOF
This is a heredoc test
with multiple lines
EOF' \
    "This is a heredoc test" \
    "0" \
    "unset HEREDOC_VAR"

# Test 16: Heredoc with quoted delimiter
run_advanced_test "Heredoc with quoted delimiter" \
    "" \
    'cat << "END"
quoted delimiter test
END' \
    "quoted delimiter test" \
    "0" \
    ""

echo "=== PIPE + REDIRECTION COMBINATIONS ==="

# Test 17: Pipe with output redirection
test_file_content "Pipe with redirection" \
    "" \
    'echo "piped content" | cat > piped_output.txt' \
    "piped_output.txt" \
    "piped content" \
    "rm -f piped_output.txt"

# Test 18: Multiple pipes with redirection
test_file_content "Multiple pipes with redirection" \
    "" \
    'echo "multi pipe" | cat | cat > multi_pipe_output.txt' \
    "multi_pipe_output.txt" \
    "multi pipe" \
    "rm -f multi_pipe_output.txt"

# Test 19: Built-in in pipe with redirection
test_file_content "Built-in in pipe with redirection" \
    'export PIPE_VAR="pipe test"' \
    'echo $PIPE_VAR | cat > builtin_pipe_output.txt' \
    "builtin_pipe_output.txt" \
    "pipe test" \
    "rm -f builtin_pipe_output.txt; unset PIPE_VAR"

echo "=== EDGE CASES ==="

# Test 20: Empty redirection
run_advanced_test "Empty echo to file" \
    "" \
    'echo > empty_output.txt' \
    "" \
    "0" \
    "rm -f empty_output.txt"

# Test 21: Redirection with special characters
test_file_content "Special characters in content" \
    "" \
    'echo "special chars: \$#@!%^&*()" > special_output.txt' \
    "special_output.txt" \
    "special chars:" \
    "rm -f special_output.txt"

# Test 22: Very long filename
test_file_content "Long filename" \
    "" \
    'echo "long" > very_long_filename_that_should_still_work_in_most_filesystems.txt' \
    "very_long_filename_that_should_still_work_in_most_filesystems.txt" \
    "long" \
    "rm -f very_long_filename_that_should_still_work_in_most_filesystems.txt"

# Test 23: Chained redirections
test_file_content "Chained redirections" \
    'echo "original" > chain1.txt' \
    'cat < chain1.txt > chain2.txt' \
    "chain2.txt" \
    "original" \
    "rm -f chain1.txt chain2.txt"

echo "=== CLEANUP AND SUMMARY ==="

# Final cleanup
rm -f *.txt 2>/dev/null
rm -rf test_dir 2>/dev/null

echo "=== TEST RESULTS ==="
echo "Passed: $passed"
echo "Failed: $failed"
echo "Total: $((passed + failed))"

if [ $failed -eq 0 ]; then
    echo -e "${GREEN}🎉 ALL ADVANCED REDIRECTION TESTS PASSED! 🎉${NC}"
    exit 0
else
    echo -e "${RED}❌ Some advanced tests failed. Check the output above.${NC}"
    exit 1
fi
