#include <unistd.h>

// simpler dwm restart on NixOS
void self_restart(const Arg *arg) {
    char *const dwm_argv[] = { "/run/current-system/sw/bin/dwm", NULL };
    execv(dwm_argv[0], dwm_argv);
}
