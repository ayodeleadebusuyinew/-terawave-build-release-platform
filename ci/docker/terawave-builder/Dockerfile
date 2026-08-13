FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        ca-certificates \
        curl \
        wget \
        git \
        unzip \
        zip \
        jq \
        file \
        build-essential \
        gcc \
        g++ \
        cmake \
        ninja-build \
        python3 \
        python3-pip \
        python3-venv \
        gcc-aarch64-linux-gnu \
        g++-aarch64-linux-gnu \
        binutils-aarch64-linux-gnu \
        gcc-riscv64-linux-gnu \
        g++-riscv64-linux-gnu \
        binutils-riscv64-linux-gnu \
        docker.io && \
    rm -rf /var/lib/apt/lists/*

# ---------------------------------------------------------
# Install Bazelisk
# ---------------------------------------------------------

RUN curl -L \
    https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64 \
    -o /usr/local/bin/bazel && \
    chmod +x /usr/local/bin/bazel

# ---------------------------------------------------------
# Install Go
# ---------------------------------------------------------

ARG GO_VERSION=1.25.0

RUN curl -L \
    https://go.dev/dl/go${GO_VERSION}.linux-amd64.tar.gz \
    -o /tmp/go.tar.gz && \
    rm -rf /usr/local/go && \
    tar -C /usr/local -xzf /tmp/go.tar.gz && \
    rm /tmp/go.tar.gz

ENV PATH="/usr/local/go/bin:/usr/local/bin:${PATH}"

WORKDIR /workspace

CMD ["/bin/bash"]
