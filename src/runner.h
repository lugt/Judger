#ifndef JUDGER_RUNNER_H
#define JUDGER_RUNNER_H

#include <sys/types.h>
#include <stdio.h>

// (ver >> 16) & 0xff, (ver >> 8) & 0xff, ver & 0xff  -> real version
#define VERSION 0x020000

#define UNLIMITED -1

#define LOG_ERROR(error_code) LOG_FATAL(log_fp, "Error: "#error_code);

#define ERROR_EXIT(error_code)\
        {\
            LOG_ERROR(error_code);  \
            _result->error = error_code; \
            log_close(log_fp);  \
            return; \
        }


enum {
    SUCCESS = 0,
    INVALID_CONFIG = -1,
    CLONE_FAILED = -2,
    PTHREAD_FAILED = -3,
    WAIT_FAILED = -4,
    ROOT_REQUIRED = -5,
    LOAD_SECCOMP_FAILED = -6,
    SETRLIMIT_FAILED = -7,
    DUP2_FAILED = -8,
    SETUID_FAILED = -9,
    EXECVE_FAILED = -10
};


struct config {
    int max_cpu_time;
    int max_real_time;
    long max_memory;
    int max_process_number;
    long max_output_size;
    char *exe_path;
    char *input_path;
    char *output_path;
    char *error_path;
    char *args[256];
    char *env[256];
    char *log_path;
    char *seccomp_rule_so_path;
    uid_t uid;
    gid_t gid;
};


enum {
    CPU_TIME_LIMITED = 1,
    REAL_TIME_LIMIT_EXCEEDED = 2,
    MEMORY_LIMIT_EXCEEDED = 3,
    RUNTIME_ERROR = 4,
    SYSTEM_ERROR = 5
};


struct result {
    int cpu_time;
    int real_time;
    long memory;
    int signal;
    int exit_code;
    int error;
    int result;
};


typedef struct child_args {
    FILE *log_fp;
    struct config *_config;
} child_args;


void run(struct config *, struct result *);
#endif //JUDGER_RUNNER_H