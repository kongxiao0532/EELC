/*************************************************************************
	> File Name: receive.c
	> Author: 
	> Mail: 
	> Created Time: Sat 28 Jul 2018 04:12:11 PM CST
 ************************************************************************/

#include "receive.h"

void packets_receive() {
    char err_buffer[PCAP_ERRBUF_SIZE];
    pcap_t* receive_nic = NULL;

    first_received = 0;

    // open NIC
    receive_nic = pcap_open_live(RECEIVE_NIC, 2048, 1, 100, err_buffer);

    // receive packets
    pcap_loop(receive_nic, PACKET_NUM, get_packet, NULL);

    printf("EELC-Receive: Ready to exit thread.\n");
    // close NIC
    pcap_close(receive_nic);

}

void get_packet(u_char* arg, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    // pcap_t* receive_nic = (pcap_t*)arg;
    struct ether_header* eth_header;
    u_char target_mac[6];
    int i;


    eth_header = (struct ether_header*)packet;
    sscanf(
        TARGET_MAC, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx",
        target_mac + 0, target_mac + 1, target_mac + 2,
        target_mac + 3, target_mac + 4, target_mac + 5
    );
    for (i = 0; i < 6; i++) {
        if (eth_header->ether_dhost[i] != target_mac[i]) {
            return;
        }
    }
    printf("received!\n");
    uint64_t latency;
    // timing!
    if(first_received){
        struct timespec current_time;
        clock_gettime(CLOCK_REALTIME, &current_time);
        latency =
        (current_time.tv_sec - start_time.tv_sec) * 1000000000
        + current_time.tv_nsec - start_time.tv_nsec;
    }else{
        clock_gettime(CLOCK_REALTIME, &start_time);
        printf("First packet received!\n");
        latency = 0;
        first_received = 1;
    }
    printf(
        "%fs, %luns\n", latency / 1000000000.0, latency
    );
    return;
}
