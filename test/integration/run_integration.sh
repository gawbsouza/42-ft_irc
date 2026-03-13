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

# --- Test 2: Channel interaction (TOPIC, PRIVMSG, PART) with two users ---
echo ""
echo "Testing channel interaction (TOPIC, PRIVMSG, PART)..."

# User1: create channel, set topic, wait for messages
mkfifo /tmp/irc_user1_pipe 2>/dev/null || true
(
    sleep 0.5
    echo -e "PASS $PASSWORD\r"
    echo -e "NICK alice\r"
    echo -e "USER alice 0 * :Alice\r"
    sleep 1
    echo -e "JOIN #integtest\r"
    sleep 0.5
    echo -e "TOPIC #integtest :Hello World Topic\r"
    sleep 2
    echo -e "PART #integtest\r"
    sleep 0.5
    echo -e "QUIT\r"
) | nc -w 8 localhost $PORT > /tmp/irc_user1_out 2>&1 &
USER1_PID=$!

# User2: join same channel, send PRIVMSG
(
    sleep 2
    echo -e "PASS $PASSWORD\r"
    echo -e "NICK bob\r"
    echo -e "USER bob 0 * :Bob\r"
    sleep 1
    echo -e "JOIN #integtest\r"
    sleep 0.5
    echo -e "PRIVMSG #integtest :Hi from Bob\r"
    sleep 2
    echo -e "QUIT\r"
) | nc -w 8 localhost $PORT > /tmp/irc_user2_out 2>&1 &
USER2_PID=$!

wait $USER1_PID 2>/dev/null
wait $USER2_PID 2>/dev/null

RESP1=$(cat /tmp/irc_user1_out 2>/dev/null)
RESP2=$(cat /tmp/irc_user2_out 2>/dev/null)

# Verify TOPIC was set (alice should get topic confirmation)
if echo "$RESP1" | grep -q "Hello World Topic"; then
    echo -e "${GREEN}PASS: TOPIC set successfully.${NC}"
else
    echo -e "${RED}FAILURE: TOPIC not confirmed.${NC}"
    echo "Alice output: $RESP1"
    rm -f /tmp/irc_user1_out /tmp/irc_user2_out /tmp/irc_user1_pipe
    exit 1
fi

# Verify bob joined the channel (alice should see bob's JOIN)
if echo "$RESP1" | grep -q "bob.*JOIN"; then
    echo -e "${GREEN}PASS: Second user JOIN visible to channel.${NC}"
else
    echo -e "${RED}FAILURE: Second user JOIN not seen by channel member.${NC}"
    echo "Alice output: $RESP1"
    rm -f /tmp/irc_user1_out /tmp/irc_user2_out /tmp/irc_user1_pipe
    exit 1
fi

# Verify PRIVMSG delivered to alice from bob
if echo "$RESP1" | grep -q "Hi from Bob"; then
    echo -e "${GREEN}PASS: Channel PRIVMSG delivered.${NC}"
else
    echo -e "${RED}FAILURE: Channel PRIVMSG not delivered.${NC}"
    echo "Alice output: $RESP1"
    rm -f /tmp/irc_user1_out /tmp/irc_user2_out /tmp/irc_user1_pipe
    exit 1
fi

# Verify bob got welcome
if echo "$RESP2" | grep -q "001 bob"; then
    echo -e "${GREEN}PASS: Second user registered successfully.${NC}"
else
    echo -e "${RED}FAILURE: Second user registration failed.${NC}"
    echo "Bob output: $RESP2"
    rm -f /tmp/irc_user1_out /tmp/irc_user2_out /tmp/irc_user1_pipe
    exit 1
fi

rm -f /tmp/irc_user1_out /tmp/irc_user2_out /tmp/irc_user1_pipe

echo ""
echo -e "${GREEN}All integration tests passed!${NC}"
exit 0
