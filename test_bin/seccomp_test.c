#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "../seccomp.c"

int main() {
    install_basic_seccomp();

    write(STDOUT_FILENO, "✅ write allowed\n", 17);
    open("/etc/passwd", O_RDONLY);  // ❌ should crash
    return 0;
}

