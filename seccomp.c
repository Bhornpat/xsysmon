#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>

void install_basic_seccomp() {
    struct sock_filter filter[] = {
        // Validate architecture
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(struct seccomp_data, arch)),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, AUDIT_ARCH_X86_64, 0, 1),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL),

        // Load syscall number
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, offsetof(struct seccomp_data, nr)),

        // ALLOW list: read, write, exit, exit_group
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_read,     0, 3),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_write,    0, 2),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_exit,     0, 1),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_exit_group, 0, 1),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),

        // DEFAULT: kill
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL)
    };

    struct sock_fprog prog = {
        .len = sizeof(filter)/sizeof(filter[0]),
        .filter = filter,
    };

    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
        perror("prctl(NO_NEW_PRIVS)");
        exit(1);
    }

    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog)) {
        perror("prctl(SECCOMP)");
        exit(1);
    }

    printf("🔐 Seccomp filter installed: only [read, write, exit] allowed\n");
}

