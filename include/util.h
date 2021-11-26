#ifndef UTIL_H
#define UTIL_H

#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>

struct process{
    pid_t pid;
    int child_in, child_out;
};

struct process *process_open(struct process *dst, const char *cmd);
void process_close(struct process *dst);

ssize_t process_read(struct process *src, void *buf, size_t count);
ssize_t process_write(struct process *dst, void *buf, size_t count);
int process_printf(struct process *dst, const char *format, ...);

#endif //UTIL_H
