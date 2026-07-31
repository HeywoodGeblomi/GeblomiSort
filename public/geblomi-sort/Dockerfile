# GeblomiSort — optimized multi-stage image
# Builder: full gcc:14  |  Runtime: debian:bookworm-slim (much smaller)
#
# Usage:
#   docker build -t geblomisort -f public/geblomi-sort/Dockerfile public/geblomi-sort
#   docker run --rm geblomisort
#   docker run --rm -v "$PWD":/work -w /work geblomisort \
#     g++ -O3 -std=c++20 -I/opt/geblomi your_code.cpp -o /tmp/a && /tmp/a
#
# Note: the runtime image does NOT contain a compiler (size-optimized).
# For compile-inside-container workflows use the builder stage or a plain gcc:14 image.

# ---------- Stage 1: build ----------
FROM gcc:14 AS builder
WORKDIR /src
COPY GeblomiSort.hpp geblomi_decision.hpp ./
COPY examples/ ./examples/
RUN g++ -O3 -std=c++20 -I. -static-libstdc++ -static-libgcc \
        examples/demo.cpp -o /geblomi-demo

# ---------- Stage 2: slim runtime ----------
FROM debian:bookworm-slim
WORKDIR /opt/geblomi

# Headers (so users can still mount & compile against them from a gcc container if desired)
COPY GeblomiSort.hpp geblomi_decision.hpp ./
COPY LICENSE ./

# Only the static-ish demo binary from the builder
COPY --from=builder /geblomi-demo /usr/local/bin/geblomi-demo

# Default: run the demo
CMD ["geblomi-demo"]
