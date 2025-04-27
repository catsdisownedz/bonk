#!/bin/bash
set -e

# Default server URL
SERVER_URL=${1:-"http://localhost:8080"}

echo "Testing Bonk game server at $SERVER_URL"
echo "----------------------------------------"

# Check if the server is reachable
echo "1. Testing HTTP health endpoint..."
if curl -s "$SERVER_URL/health" | grep -q "healthy"; then
    echo "✅ Health check passed"
else
    echo "❌ Health check failed"
    exit 1
fi

# Test WebSocket connection
echo ""
echo "2. Testing WebSocket connection..."
echo "To test manually, open a browser and visit: $SERVER_URL"
echo "Connect using the 'Connect to WebSocket' button"

# Test complete
echo ""
echo "✅ Basic tests completed successfully"
echo "Server is running at: $SERVER_URL" 