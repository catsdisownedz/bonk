#!/bin/bash
set -e

# Check if PROJECT_ID is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <project_id>"
  exit 1
fi

PROJECT_ID=$1
IMAGE_NAME="bonk-server"
SERVICE_NAME="bonk-server"

echo "Redeploying Bonk game server to Google Cloud Run..."
echo "Project ID: $PROJECT_ID"

# Build and push Docker image
echo "Building and pushing Docker image to Google Container Registry..."
gcloud builds submit --tag gcr.io/${PROJECT_ID}/${IMAGE_NAME}

# Deploy to Cloud Run
echo "Deploying to Cloud Run..."
gcloud run deploy ${SERVICE_NAME} \
  --image gcr.io/${PROJECT_ID}/${IMAGE_NAME} \
  --platform managed \
  --allow-unauthenticated \
  --port 8080 \
  --region us-central1 \
  --cpu 1 \
  --memory 512Mi \
  --min-instances 1 \
  --max-instances 10 \
  --use-http2 \
  --session-affinity \
  --set-env-vars="DEBUG=true" \
  --concurrency=100 \
  --timeout=300 \
  --no-cpu-throttling \
  --execution-environment=gen2

# Get service URL
SERVICE_URL=$(gcloud run services describe ${SERVICE_NAME} \
  --platform managed \
  --region us-central1 \
  --format="value(status.url)")

echo "✅ Redeployment successful!"
echo "Your Bonk game server is now running at: ${SERVICE_URL}"
echo ""
echo "To connect to the server with a client, use the following URL: ${SERVICE_URL}"
echo ""
echo "To check server logs, run:"
echo "gcloud logging read \"resource.type=cloud_run_revision AND resource.labels.service_name=${SERVICE_NAME}\" --limit=50" 