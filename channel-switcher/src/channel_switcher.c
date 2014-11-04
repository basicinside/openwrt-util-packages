/* channel switcher by Robin Kuck <robin@basicinside.de> */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <net/if.h>

/* nl80211.h from the iw tool */
#include "nl80211.h"

#define LOG_FAIL(msg) fprintf(stderr, msg); exit(1)
#define LOG_DEBUG(msg) fprintf(stderr, msg); fflush(stdout)
#define CLEAR(s) memset(&(s), 0, sizeof(s))

static int channel_idx = 0;
static int driver_id;
static struct nl_sock* sock;
static enum nl80211_commands cmd;

static long interval_us = 999999;
const char * interface_name;
static int interface_idx = 0;
int channel_count = 0;
int channels[50] = {2412,2417,2422,2427,2432,2437,2442,2447,2452,2457,2462,2467,2472};

int load_channels(const char* path) {
    int i = 0;
    FILE *file = fopen(path, "r");
    char channel[10]; /* cause 10 is greater than 4 */
    CLEAR(channels);
    if (file == NULL) {
        LOG_FAIL("Could not read channels\n");
    } else {
        while(fgets(channel, sizeof(channel), file) != NULL) {
            channels[i++] = strtol(channel, NULL, 10);
        }
        channel_count = i;
        fclose(file);
    }
}

#ifdef DEBUG
static int nl_cb_iface(struct nl_msg* msg, void* args)
{
    struct timeval now;

    gettimeofday(&now, NULL);
    printf("Response took %ld us\n", now.tv_usec % interval_us);
}
#endif

void do_switch(int signal)
{
    struct nl_msg* msg = nlmsg_alloc();

    genlmsg_put(msg, 0, 0, driver_id, 0, 0, cmd, 0);

    NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, interface_idx);
    NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FREQ, channels[channel_idx]);

    nl_send_auto_complete(sock, msg);
#ifdef DEBUG
    struct timeval now;
    gettimeofday(&now, NULL);
    printf("[%i.%i]Set channel to %i on %s (%i)\n", now.tv_sec, now.tv_usec,
            channels[channel_idx],
            interface_name, interface_idx);
#endif

    nl_recvmsgs_default(sock);
    channel_idx = (channel_idx + 1) % channel_count;

nla_put_failure:
    nlmsg_free(msg);
}

int set_interface_idx(const char* name)
{
    interface_name = name;
    interface_idx = if_nametoindex(name);
    if (!interface_idx) {
        perror(name);
        exit(1);
    }
    return interface_idx;
}

int set_interval_us(const char* value)
{
    interval_us = strtol(value, NULL, 10);
    if (interval_us > 999999) {
        fprintf(stderr, "Interval %ldus > 999999us invalid\n");
    }
}

int main(int argc, char **argv)
{
    struct timeval now;
    int rc;

    if (argc < 3) {
        fprintf(stderr, "Usage: ./%s <interface> <channels> (<interval_us>)\n", argv[0]);
        exit(1);
    }
    set_interface_idx(argv[1]);
    load_channels(argv[2]);

    if (argc < 4) {
        interval_us = 999999;
    } else {
        set_interval_us(argv[3]);
    }

    sock = nl_socket_alloc();
    if (!sock) {
        LOG_FAIL("Netlink socket allocation failed\n");
    }
    rc = genl_connect(sock);
    if (rc < 0) {
        LOG_FAIL("Generic Netlink connection failure\n");
    }

    driver_id = genl_ctrl_resolve(sock, "nl80211");
#ifdef DEBUG
    nl_socket_modify_cb(sock, NL_CB_ACK, NL_CB_CUSTOM, nl_cb_iface, NULL);
#endif
    cmd = NL80211_CMD_SET_CHANNEL;

    gettimeofday(&now, NULL);
    signal(SIGALRM, do_switch);
    /* start at second 0,15,30,45 */
    sleep(15-(now.tv_sec % 15));
    gettimeofday(&now, NULL);
    /* start at microsecond 0 */
    ualarm(999999 - now.tv_usec, interval_us);

    while (1) {
        pause();
    }

    return 0;
}
