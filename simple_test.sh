#!/bin/bash

echo "#!/bin/bash
echo 'Starting minishell test...'
echo \$?
echo 'Test completed.'
exit" > /tmp/test_input.sh

chmod +x /tmp/test_input.sh

echo "Executing test..."
timeout 5s bash -c './minishell < /tmp/test_input.sh'
echo "Exit code: $?"

rm -f /tmp/test_input.sh
