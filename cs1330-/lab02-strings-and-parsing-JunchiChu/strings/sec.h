#ifndef SEC_H_
#define SEC_H_

#ifdef __linux__
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/prctl.h>
#include <sys/syscall.h>

#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>

#define ALLOW(syscall)                                         \
    BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, __NR_##syscall, 0, 1), \
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ALLOW)

#define KILL_PROCESS BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_TRAP)

#if defined(__i386__)
#define ARCH_NR AUDIT_ARCH_I386
#elif defined(__x86_64__)
#define ARCH_NR AUDIT_ARCH_X86_64
#endif

#define VALIDATE_ARCH                                                          \
    BPF_STMT(BPF_LD + BPF_W + BPF_ABS, (offsetof(struct seccomp_data, arch))), \
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, ARCH_NR, 1, 0),                    \
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_KILL)

#define EXAMINE_SYSCALL \
    BPF_STMT(BPF_LD + BPF_W + BPF_ABS, offsetof(struct seccomp_data, nr))

struct sock_filter filter[] = {
    /* validate architecture */
    VALIDATE_ARCH,

    /* load syscall */
    EXAMINE_SYSCALL,

    /* list of allowed syscalls */
    ALLOW(exit_group), ALLOW(write), ALLOW(read), ALLOW(fstat), ALLOW(mmap),
    ALLOW(fork), ALLOW(clone), ALLOW(wait4), ALLOW(clock_gettime),

    /* if we don't match the list above, kill process */
    KILL_PROCESS};
struct sock_fprog filterprog = {
    .len = (unsigned short)(sizeof(filter) / sizeof(filter[0])),
    .filter = filter};

void install_seccomp() {
    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
        perror("prctl(NO_NEW_PRIVS)");
        exit(1);
    }

    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &filterprog)) {
        perror("prctl(SECCOMP)");
        exit(1);
    }
}
#else   // __linux__
void install_seccomp() {}
#endif  // __linux__
#endif  // SEC_H_
