# Earthfile
VERSION 0.7
FROM ubuntu:22.04

# Configure apt to be noninteractive
ENV DEBIAN_FRONTEND noninteractive
ENV DEBCONF_NONINTERACTIVE_SEEN true

# Prepare cache mount for apt cache
RUN --mount=type=cache,id=apt-cache,target=/var/cache/apt \
    apt-get update && apt-get install -y \
        build-essential \
        cmake \
        git \
        libopencv-dev \
        gcc \
        g++ \
        clang \
        llvm \
        lcov \
        doxygen \
        graphviz \
        expect \
        flatbuffers-compiler \
        clang-tidy

WORKDIR /project
COPY . .

# Define the build target
build:
    RUN rm -rf build
    RUN mkdir -p build
    # Configure the CMake project
    RUN --mount=type=cache,id=cmake-build,target=/root/.cache/cmake \
        cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    # Run the actual build command
    RUN cmake --build build -- -j $(nproc)
    # Save the built artifact
    SAVE ARTIFACT build/AIMM AS LOCAL ./generated/AIMM
