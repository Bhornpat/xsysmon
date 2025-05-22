Linux toolkit for **syscall-level tracing, injection, sandboxing, and ELF execution**

---

## Features
-  **Syscall Tracer** – Trace and log syscalls with arguments + return values using `ptrace()`
-  **Live Syscall Injector** – Rewrite syscall IDs and arguments on the fly (e.g., block `execve`, spoof `open()`)
-  **Syscall Name Mapper** – Decode raw syscall numbers to human-readable names (x86_64)
-  **Seccomp BPF Filter Engine** – Install minimal syscall firewalls to isolate or harden apps
-  **Manual Linux Sandbox** – Spawn processes in isolated namespaces (`PID`, `USER`, `MOUNT`, etc.) using `clone()`
-  **Minimal Container Runtime** – Chroot, UID drop, mount isolation, `proc` remount
-  *(WIP)* **ELF Loader** – Manually parse and map ELF64 binaries using `mmap()` and `exec()` them without `execve`
-  **All written in raw C** – No libc dependencies for core internals, works close to the metal.

---

### build & Run

Requires Linux, x86_64, and root privileges for some features

### Build All

```bash
### make
gcc -Wall -o syscall_tracer syscall_tracer.c
gcc -Wall -o sandbox sandbox.c
gcc -Wall -o seccomp_test seccomp_test.c

--

### Run Syscall Tracer
sudo ./syscall_tracer /bin/ls

### Run with Seccomp
./seccomp_test

### Spawn Sandbox Container
sudo ./sandbox

