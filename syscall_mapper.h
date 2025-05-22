#ifndef SYSCALL_MAPPER_H
#define SYSCALL_MAPPER_H

const char *syscall_name_from_id(long id) {
    switch (id) {
        case 0: return "read";
        case 1: return "write";
        case 2: return "open";
        case 3: return "close";
        case 4: return "stat";
        case 5: return "fstat";
        case 6: return "lstat";
        case 9: return "mmap";
        case 10: return "mprotect";
        case 11: return "munmap";
        case 12: return "brk";
        case 20: return "getpid";
        case 39: return "getpid";
        case 57: return "fork";
        case 59: return "execve";
        case 60: return "exit";
        case 61: return "wait4";
        case 62: return "kill";
        case 231: return "exit_group";
        default: return "unknown";
    }
}

#endif // SYSCALL_MAPPER_H

