#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

static volatile int keep_running = 1;

static void handle_sigint(int sig) {
    (void)sig;
    keep_running = 0;
}

// PUBLIC_INTERFACE
int main(int argc, char *argv[]) {
    /**
     * This is a simple sample application that prints a heartbeat every 10 seconds.
     * It is packaged as a systemd service via the Yocto recipe for demonstration.
     *
     * Arguments:
     *  --version : prints version and exits.
     *  --oneshot : runs once and exits.
     */
    if (argc > 1) {
        if (strcmp(argv[1], "--version") == 0) {
            printf("sampleapp version 1.0\n");
            return 0;
        } else if (strcmp(argv[1], "--oneshot") == 0) {
            printf("sampleapp oneshot run at %ld\n", (long)time(NULL));
            return 0;
        }
    }

    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);

    printf("sampleapp starting up...\n");
    while (keep_running) {
        printf("sampleapp heartbeat at %ld\n", (long)time(NULL));
        fflush(stdout);
        sleep(10);
    }
    printf("sampleapp shutting down.\n");
    return 0;
}
