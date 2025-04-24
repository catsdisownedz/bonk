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

echo "Deploying Bonk game server to Google Cloud Run..."
echo "Project ID: $PROJECT_ID"

# Enable required APIs
echo "Enabling required Google Cloud APIs..."
gcloud services enable run.googleapis.com artifactregistry.googleapis.com cloudbuild.googleapis.com

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
  --min-instances 0 \
  --max-instances 10 \
  --set-env-vars=PORT=8080 \
  --http2 \
  --session-affinity \
  --concurrency=100 \
  --timeout=300

# Get service URL
SERVICE_URL=$(gcloud run services describe ${SERVICE_NAME} \
  --platform managed \
  --region us-central1 \
  --format="value(status.url)")

echo "✅ Deployment successful!"
echo "Your Bonk game server is now running at: ${SERVICE_URL}"
echo ""
echo "To connect to the server with a client, use the following URL: ${SERVICE_URL}"
echo ""
echo "Note: The service is deployed without a health check, as Cloud Run HTTP health checks"
echo "don't work well with non-HTTP protocols like ENet. However, the service has a built-in"
echo "health check mechanism to ensure it stays responsive." 