#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <string.h>
#include <unistd.h>

void inject_syscall(pid_t pid, struct user_regs_struct *regs) {
    if (regs->orig_rax == SYS_openat) {
        printf("🚨 openat detected! Replacing with harmless syscall...\n");

        // Change syscall ID to 'getpid' (harmless)
        regs->orig_rax = SYS_getpid;

        // Overwrite arguments (for fun/demo)
        regs->rdi = 0;
        regs->rsi = 0;

        ptrace(PTRACE_SETREGS, pid, NULL, regs);
    }

    // You can block execve like this:
    if (regs->orig_rax == SYS_execve) {
        printf("❌ execve blocked!\n");

        // Replace with exit syscall
        regs->orig_rax = SYS_exit;
        regs->rdi = 123;  // exit code

        ptrace(PTRACE_SETREGS, pid, NULL, regs);
    }
}

