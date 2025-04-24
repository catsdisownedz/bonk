FROM alpine:3.18 AS build

# Install build dependencies
RUN apk update && \
    apk add --no-cache \
    build-base \
    cmake \
    make \
    git \
    enet-dev \
    linux-headers \
    pkgconfig

# Create working directory
WORKDIR /app

# Copy source files
COPY . .

# Build the server
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make

# Final image
FROM alpine:3.18

# Install runtime dependencies
RUN apk add --no-cache \
    libstdc++ \
    enet

# Create working directory
WORKDIR /app

# Copy the built server executable from build stage
COPY --from=build /app/build/server /app/

# Cloud Run requires the PORT environment variable
ENV PORT=8080

# Expose the port used by both the game server and web interface
EXPOSE 8080

# Command to run the server
CMD ["/app/server"] 