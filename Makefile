CC=zig c++
CFLAGS=-target x86_64-linux-musl -s -Os -Oz -static -lstdc++

all: src/main.cpp
	$(CC) $(CFLAGS) -o memrunner $<
