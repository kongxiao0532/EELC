/*************************************************************************
	> File Name: replay.h
	> Author: 
	> Mail: 
	> Created Time: Sat 28 Jul 2018 11:21:33 AM CST
 ************************************************************************/

#ifndef _REPLAY_H
#define _REPLAY_H

#include <stdio.h>
#include <unistd.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <pthread.h>
#include <sys/time.h>

#define REPLAY_NIC "enp4s0f0"
#define PCAP_FILE "400w.pcap"

#define ETHER_HEADER_LENGTH 14

#define PKT_MAX_SIZE 65535
#define REPLAY_PROMISC 1
#define TO_MS 1000

#define SEND_DELAY_US 1000

#define LOCAL_MAC "68:91:d0:61:12:3a"
#define TARGET_MAC "68:91:d0:61:b4:c4"

#define TIME_RECORD_SIZE 10000

// typedef struct replay_arg {
    // pthread_mutex_t* mutex;
    // struct timeval* start_time_record;
// } replay_arg;

void* pcap_replay(void* argv);

#endif
