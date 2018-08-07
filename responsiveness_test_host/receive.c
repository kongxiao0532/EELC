/*************************************************************************
	> File Name: receive.c
	> Author: 
	> Mail: 
	> Created Time: Sat 28 Jul 2018 04:12:11 PM CST
 ************************************************************************/

#include "receive.h"

void packets_receive(void* argv) {
    char err_buffer[PCAP_ERRBUF_SIZE];
    pcap_t* receive_nic;

    first_received = 0;

    // open NIC
    receive_nic = pcap_open(RECEIVE_NIC, 2048, PCAP_OPENFLAG_PROMISCUOUS, 100, NULL, err_buffer);
    // receive packets
    pcap_loop(receive_nic, PACKET_NUM, get_packet, (u_char*)receive_nic);

    printf(stdout, "EELC-Receive: Ready to exit thread.\n");
    // close NIC
    pcap_close(receive_nic);

    return NULL;
}

void get_packet(u_char* arg, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    pcap_t* receive_nic = (pcap_t*)arg;
    struct ether_header* eth_header;
    u_char target_mac[6];
    int i;

    eth_header = (struct ether_header*)packet;
    sscanf(
        TARGET_MAC, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx",
        target_mac + 5, target_mac + 4, target_mac + 3,
        target_mac + 2, target_mac + 1, target_mac + 0
    );
    for (i = 0; i < 6; i++) {
        if (eth_header->ether_dhost[i] != target_mac[i]) {
            return;
        }
    }
    // timing!
    if(first_received){
        struct timespec current_time;
        clock_gettime(CLOCK_REALTIME, &current_time);
        printf(
            "%luns\n", current_time.tv_nsec - start_time.tv_nsec
        );
    }else{
        clock_gettime(CLOCK_REALTIME, &start_time);
        printf("First packet received!\n");
        first_received = 1;
    }
    return;
}
