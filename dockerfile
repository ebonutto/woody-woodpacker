FROM --platform=linux/amd64 debian:bullseye-slim as DEV

RUN apt-get update && apt-get install -y \
    gcc make \
    gdb valgrind strace \
    nasm xxd\
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app