FROM alpine:3.18 AS build

# Install build dependencies
RUN apk update && \
    apk add --no-cache \
    build-base \
    cmake \
    make \
    git \
    boost-dev \
    boost-system \
    boost-thread \
    openssl-dev \
    linux-headers \
    pkgconfig

# Create working directory
WORKDIR /app

# Copy source files
COPY . .

# Clone WebSocket++ directly into our include directory
RUN mkdir -p include/network && \
    git clone https://github.com/zaphoyd/websocketpp.git include/network/websocketpp

# Print directory structure for debugging
RUN ls -la include/network/

# Build the server
RUN mkdir -p build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc)

# Final image
FROM alpine:3.18

# Install runtime dependencies and networking tools for debugging
RUN apk add --no-cache \
    libstdc++ \
    boost \
    boost-system \
    boost-thread \
    openssl \
    curl \
    busybox-extras \
    netcat-openbsd \
    tcpdump \
    strace \
    ltrace \
    lsof

# Create working directory
WORKDIR /app

# Copy the built server executable from build stage
COPY --from=build /app/build/server /app/

# Expose the port used by the server (TCP for WebSockets)
# Cloud Run will set the PORT env var and we need to listen on that port
EXPOSE 8080/tcp

# Add a healthcheck that tests the HTTP server inside the container
HEALTHCHECK --interval=5s --timeout=3s --start-period=5s --retries=3 \
  CMD wget -O- http://localhost:8080/health || exit 1

# Command to run the server
CMD ["/app/server"] 