#!/bin/bash

# Configuration
PORT=6667
PASSWORD="testpassword"
SERVER_BIN="./ircserv"
LOG_FILE="test/integration/server.log"

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "Starting Integration Tests..."

# 1. Start the server
$SERVER_BIN $PORT $PASSWORD > $LOG_FILE 2>&1 &
SERVER_PID=$!

# Ensure server is stopped on script exit
trap "kill $SERVER_PID 2>/dev/null" EXIT

# Wait for server to start
sleep 2

if ! kill -0 $SERVER_PID 2>/dev/null; then
    echo -e "${RED}FAILURE: Server failed to start. Check $LOG_FILE${NC}"
    exit 1
fi

echo "Server started with PID $SERVER_PID"

# 2. Function to send commands and capture output
send_commands() {
    local commands=$1
    echo -e "$commands" | nc -w 2 localhost $PORT
}

echo "Testing basic registration and JOIN..."

# 3. Perform test
RESPONSE=$(send_commands "PASS $PASSWORD\r\nNICK tester\r\nUSER tester 0 * :Tester Real\r\nJOIN #testchan\r\nQUIT\r\n")

# 4. Verify results
if echo "$RESPONSE" | grep -q "001 tester"; then
    echo -e "${GREEN}PASS: Welcome message (001) received.${NC}"
else
    echo -e "${RED}FAILURE: Welcome message (001) not found.${NC}"
    echo "Full response: $RESPONSE"
    exit 1
fi

if echo "$RESPONSE" | grep -q "JOIN #testchan"; then
    echo -e "${GREEN}PASS: JOIN command successful.${NC}"
else
    echo -e "${RED}FAILURE: JOIN command output not found.${NC}"
    echo "Full response: $RESPONSE"
    exit 1
fi

echo -e "${GREEN}All integration tests passed!${NC}"
exit 0
