#!/bin/bash

# Default port
PORT=${1:-8080}

echo "Testing connection to WebSocket server on port $PORT..."
echo "----------------------------------------"

# Test the HTTP health endpoint
echo "Testing HTTP health endpoint..."
curl -s http://localhost:$PORT/health

echo ""
echo "----------------------------------------"
echo "Server appears to be running correctly if you see a health response above."
echo "To open the web interface in your browser, visit: http://localhost:$PORT"
echo ""
echo "For WebSocket testing, use a WebSocket client or open the web page and click 'Connect to WebSocket'" 