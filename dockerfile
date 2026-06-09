FROM debian:bullseye-slim AS dev

RUN apt-get update && apt-get install -y \
    gcc make \
    gdb valgrind strace \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app