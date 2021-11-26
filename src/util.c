#include "util.h"


struct process *process_open(struct process *dst, const char *cmd){
    pid_t p;
    int pipe_stdin[2], pipe_stdout[2];

    if(pipe(pipe_stdin)) return NULL;
    if(pipe(pipe_stdout)) return NULL;

    p = fork();
    if(p < 0)
        return NULL;
    if(p == 0){
        close(pipe_stdin[1]);
        dup2(pipe_stdin[0], 0);
        close(pipe_stdout[0]);
        dup2(pipe_stdout[1], 1);
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        perror("execl"); exit(99);
    }
    dst->pid = p;
    dst->child_in = pipe_stdin[1];
    dst->child_out = pipe_stdout[0];
    close(pipe_stdin[0]);
    close(pipe_stdout[1]);
    return dst;
}
void process_close(struct process *dst){
    close(dst->child_in);
    close(dst->child_out);
    kill(dst->pid, SIGTERM);
    dst->pid = 0;
    dst->child_in = 0;
    dst->child_out = 0;
}

ssize_t process_read(struct process *src, void *buf, size_t count){
    return read(src->child_out, buf, count);
}
ssize_t process_write(struct process *dst, void *buf, size_t count){
    return write(dst->child_in, buf, count);
}
int process_printf(struct process *dst, const char *format, ...){
    va_list vl;
    va_start(vl, format);
    int ret = vdprintf(dst->child_in, format, vl);
    va_end(vl);
    return ret;
}
