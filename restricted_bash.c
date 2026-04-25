#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>

/* Parse arguments from a string, respecting quotes
 * Returns number of arguments parsed, fills argv array
 * argv should have enough space for all arguments
 */
int parse_args(const char *str, char **argv, int max_args) {
    int argc = 0;
    const char *p = str;
    
    while (*p && argc < max_args) {
        // Skip whitespace
        while (isspace((unsigned char)*p)) p++;
        if (!*p) break;
        
        // Check for quoted string
        if (*p == '"') {
            p++;
            const char *start = p;
            // Find closing quote
            while (*p && *p != '"') p++;
            size_t len = p - start;
            argv[argc] = malloc(len + 1);
            if (argv[argc]) {
                strncpy(argv[argc], start, len);
                argv[argc][len] = '\0';
                argc++;
            }
            if (*p == '"') p++;
        } else if (*p == '\'') {
            // Single quotes
            p++;
            const char *start = p;
            while (*p && *p != '\'') p++;
            size_t len = p - start;
            argv[argc] = malloc(len + 1);
            if (argv[argc]) {
                strncpy(argv[argc], start, len);
                argv[argc][len] = '\0';
                argc++;
            }
            if (*p == '\'') p++;
        } else {
            // Unquoted argument
            const char *start = p;
            while (*p && !isspace((unsigned char)*p)) p++;
            size_t len = p - start;
            argv[argc] = malloc(len + 1);
            if (argv[argc]) {
                strncpy(argv[argc], start, len);
                argv[argc][len] = '\0';
                argc++;
            }
        }
    }
    
    return argc;
}

int main(int argc, char *argv[], char *envp[]) {
    const char *pwd = getenv("PWD");
    if (!pwd) {
        fprintf(stderr, "PWD environment variable not set\n");
        return 1;
    }
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "HOME environment variable not set\n");
        return 1;
    }

    // Parse extra bwrap arguments from environment variable
    const char *extra_args_str = getenv("EXTRA_BWRAP_ARGS");
    char *extra_args[256];
    int extra_argc = 0;
    
    if (extra_args_str) {
        extra_argc = parse_args(extra_args_str, extra_args, 256);
    }

    // Base args array - we'll build dynamically
    // Maximum possible size: base args + extra args + argv from command line
    char **args = malloc((256 + extra_argc + argc) * sizeof(char *));
    if (!args) {
        perror("malloc");
        return 1;
    }
    
    int arg_idx = 0;
    
    // Standard bwrap setup
    args[arg_idx++] = "bwrap";
    args[arg_idx++] = "--unshare-pid";
    args[arg_idx++] = "--unshare-uts";
    args[arg_idx++] = "--unshare-ipc";
    args[arg_idx++] = "--unshare-net"; /* No network for shell commands */
    args[arg_idx++] = "--die-with-parent";
    args[arg_idx++] = "--new-session";
    args[arg_idx++] = "--dev";
    args[arg_idx++] = "/dev";
    args[arg_idx++] = "--ro-bind";
    args[arg_idx++] = "/usr";
    args[arg_idx++] = "/usr";
    args[arg_idx++] = "--ro-bind";
    args[arg_idx++] = "/bin";
    args[arg_idx++] = "/bin";
    args[arg_idx++] = "--ro-bind";
    args[arg_idx++] = "/etc";
    args[arg_idx++] = "/etc";
    args[arg_idx++] = "--ro-bind";
    args[arg_idx++] = "/lib";
    args[arg_idx++] = "/lib";
    
    // Insert extra args from config (before filesystem binds that might depend on them)
    for (int i = 0; i < extra_argc; i++) {
        args[arg_idx++] = extra_args[i];
    }
    
    // Check for git worktree .git file and bind parent .git directory
    char gitfile_path[4096];
    snprintf(gitfile_path, sizeof(gitfile_path), "%s/.git", pwd);
    struct stat st;
    if (stat(gitfile_path, &st) == 0 && S_ISREG(st.st_mode)) {
        FILE *f = fopen(gitfile_path, "r");
        if (f) {
            char line[4096];
            if (fgets(line, sizeof(line), f)) {
                char *prefix = "gitdir: ";
                size_t preflen = strlen(prefix);
                if (strncmp(line, prefix, preflen) == 0) {
                    char *gitdir = line + preflen;
                    size_t len = strlen(gitdir);
                    while (len > 0 && (gitdir[len-1] == '\n' || gitdir[len-1] == '\r')) {
                        gitdir[--len] = '\0';
                    }
                    char *last_slash = strrchr(gitdir, '/');
                    if (last_slash) {
                        char *prev_slash = NULL;
                        for (char *p = gitdir; p < last_slash; p++) {
                            if (*p == '/') prev_slash = p;
                        }
                        if (prev_slash) {
                            *prev_slash = '\0';
                            args[arg_idx++] = "--bind";
                            args[arg_idx++] = strdup(gitdir);
                            args[arg_idx++] = strdup(gitdir);
                        }
                    }
                }
            }
            fclose(f);
        }
    }

    // Continue with remaining args
    args[arg_idx++] = "--ro-bind-try";
    args[arg_idx++] = "/lib64";
    args[arg_idx++] = "/lib64";
    args[arg_idx++] = "--tmpfs";
    args[arg_idx++] = "/run";
    args[arg_idx++] = "--bind";
    args[arg_idx++] = (char *)pwd;
    args[arg_idx++] = (char *)pwd;
    args[arg_idx++] = "--bind";
    args[arg_idx++] = "/tmp";
    args[arg_idx++] = "/tmp";
    args[arg_idx++] = "--bind";
    args[arg_idx++] = "/dev/shm";
    args[arg_idx++] = "/dev/shm";
    args[arg_idx++] = "--proc";
    args[arg_idx++] = "/proc";
    args[arg_idx++] = "--cap-drop";
    args[arg_idx++] = "ALL";
    args[arg_idx++] = "--";
    args[arg_idx++] = "/bin/bash";
    
    // Append any arguments passed to restricted_bash (e.g., -c "command")
    for (int i = 1; i < argc; i++) {
        args[arg_idx++] = argv[i];
    }
    args[arg_idx++] = NULL;

    /* Execute bash in the restricted environment with no network access */
    execve("/usr/bin/bwrap", args, envp);
    perror("execve");
    return 1;
}
