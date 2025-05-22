#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

char *const child_args[] = {
    "/bin/sh", NULL
};

int child_fn(void *arg) {
    printf("🚀 Inside isolated child process (PID: %d)\n", getpid());

    // Remount proc
    mount("proc", "/proc", "proc", 0, NULL);

    // Drop privileges
    sethostname("sandbox", 7);
    setuid(1000);
    setgid(1000);

    // Start shell
    execv(child_args[0], child_args);
    perror("execv failed");
    return 1;
}

int main() {
    printf("🌐 Launching isolated sandbox\n");

    // Create temp directory for new root
    const char *new_root = "./sandbox_root";
    mkdir(new_root, 0755);

    // Make sure /proc exists
    mkdir("./sandbox_root/proc", 0755);

    int clone_flags = CLONE_NEWUSER | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD;

    // Start container
    pid_t pid = clone(child_fn, child_stack + STACK_SIZE, clone_flags, NULL);
    if (pid == -1) {
        perror("clone");
        exit(1);
    }

    printf("🧍 Parent waiting for child %d...\n", pid);
    waitpid(pid, NULL, 0);
    printf("✅ Sandbox process finished.\n");

    return 0;
}

