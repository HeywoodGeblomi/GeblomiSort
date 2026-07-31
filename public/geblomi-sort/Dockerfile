# GeblomiSort — simple C++20 demo / test image
# Usage:
#   docker build -t geblomisort -f public/geblomi-sort/Dockerfile public/geblomi-sort
#   docker run --rm geblomisort
#   docker run --rm -v "$PWD":/work -w /work geblomisort g++ -O3 -std=c++20 -I/opt/geblomi your_code.cpp -o /tmp/a && /tmp/a

FROM gcc:14

WORKDIR /opt/geblomi

# Package headers + demo
COPY GeblomiSort.hpp geblomi_decision.hpp ./
COPY examples/ ./examples/
COPY LICENSE ./

# Build the official demo at image build time
RUN g++ -O3 -std=c++20 -I. examples/demo.cpp -o /usr/local/bin/geblomi-demo

# Default: run the demo
CMD ["geblomi-demo"]
