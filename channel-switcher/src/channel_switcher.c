/* channel switcher by Robin Kuck <robin@basicinside.de> */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

static int i = 0;
static long interval_ms;

void do_switch(int signal)
{
    static struct timeval now;
    gettimeofday(&now, NULL);
    printf("%ld ", now.tv_usec - (i * interval_ms));
    i++;
}

int main(int argc, char **argv)
{
    struct timeval now;
    if (argc < 2) {
        interval_ms = 100000;
    } else {
        interval_ms = strtol(argv[1], NULL, 10);
    }
    gettimeofday(&now, NULL);
    signal(SIGALRM, do_switch);
    /* start at microsecond 0 */
    ualarm(1000000 - now.tv_usec, interval_ms);
    while (i < 10) {
        sleep(10);
    }
    return 0;
}
