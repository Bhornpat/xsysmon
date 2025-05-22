#define _GNU_SOURCE
#include "injector.c"
#include "syscall_mapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <errno.h>

void trace(pid_t child) {
    int status;
    struct user_regs_struct regs;
    int insyscall = 0;

    while (1) {
        waitpid(child, &status, 0);
        if (WIFEXITED(status)) break;

        // Intercept syscall entry + exit
        ptrace(PTRACE_GETREGS, child, 0, &regs);

        if (!insyscall) {
    insyscall = 1;
    printf("➡️ syscall(%lld [%s]) called with RDI=%lld RSI=%lld RDX=%lld\n",
           regs.orig_rax,
           syscall_name_from_id(regs.orig_rax),
           regs.rdi, regs.rsi, regs.rdx);

    inject_syscall(child, &regs);

        } else {
            insyscall = 0;
            printf("⬅️  return value: %lld\n\n", regs.rax);
        }

        ptrace(PTRACE_SYSCALL, child, 0, 0);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program> [args...]\n", argv[0]);
        exit(1);
    }

    pid_t child = fork();

    if (child == 0) {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        kill(getpid(), SIGSTOP); // allow parent to attach
        execvp(argv[1], &argv[1]);
        perror("execvp failed");
    } else {
        waitpid(child, NULL, 0);
        ptrace(PTRACE_SYSCALL, child, 0, 0);
        trace(child);
    }

    return 0;
}

