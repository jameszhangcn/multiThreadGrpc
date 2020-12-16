#include <cstring>

extern "C" {
#include "TrilliumLib.h"
}

int main()
{
    mav_init();

    mav_udp_header_t udp;
    memset(&udp, 0, sizeof(udp));
    mav_ip4_header_t ip4;
    memset(&ip4, 0, sizeof(ip4));
    mav_ip6_header_t ip6;
    memset(&ip6, 0, sizeof(ip6));

    mav_on_udp(&udp, &ip4);
    mav_on_udp6(&udp, &ip6);
}
