#include <unistd.h>

// simpler dwm restart on NixOS
void self_restart(const Arg *arg) {
    char *const dwm_argv[] = { "/run/current-system/sw/bin/dwm", NULL };
    execv(dwm_argv[0], dwm_argv);
}

void self_restart_test(const Arg *arg) {
    const char *home = getenv("HOME");
    if (!home) return;

    char path[512];
    snprintf(path, sizeof(path), "%s/HOME/infra/dwm/dwm", home);

    char *const dwm_argv[] = { path, NULL };
    execv(path, dwm_argv);
}
