# Linux.MemRunner

 * This is a simple Linux "fileless" loader.
 * Uses memfd_create() and fexecve() syscall.
 * Encode itself with xor using obfuscate.h
 * It can be used in Linux 3.17/4.x/5.x/6.x...
 ---

**Observation**: I wrote this project in May 2023 while exploring various fascinating Linux topics.
(I know that there are alternative methods, such as memfd_create() and dlopen(), for achieving a true "fileless" dropper).
Although I choose not to publish the code, you can use it as a reference for studying and developing on your own.

---

- **Requirements**:
    - zig-c++
    - musl x86-64

## Compile with:
```
$ make
```
 ---

## How to use?

 * Write the loader in /dev/shm and:
```
$ ./memrunner {FILE}
```
 ---

- **Reference**:
    - https://upris.in/memrunner
    - https://x-c3ll.github.io/posts/fileless-memfd_create/
