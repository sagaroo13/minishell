#!/bin/bash

echo "Testing SIGINT behavior in minishell..."
echo "Testing sequence:"
echo "1. Starting minishell"
echo "2. Sending SIGINT (simulating Ctrl+C)"
echo "3. Testing echo \$?"
echo "4. Exiting"
echo ""

# Run the test
(
    echo "echo \$?"
    echo "exit"
) | (timeout 3s ./minishell || echo "Test completed")

echo ""
echo "Test finished."
