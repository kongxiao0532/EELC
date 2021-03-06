/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Sat 28 Jul 2018 11:19:43 AM CST
 ************************************************************************/

#include "main.h"

int main(void) {
    int ret;
    void* status;
    pthread_t replay_thread;
    pthread_t receive_thread;
    cpu_set_t set;
    // struct timeval start_time;
    // struct timeval end_time;
    struct timespec start_time;
    struct timespec end_time;
    FILE* fp;

    ret = pthread_create(
        &replay_thread, NULL, &pcap_replay, (void*)start_time_record
    );
    if (ret != 0) {
        fprintf(stderr, "Error: EELC-Main: Can't create Replay thread!");
        exit(1);
    }

    ret = pthread_create(
        &receive_thread, NULL, &packets_receive, (void*)end_time_record
    );
    if (ret != 0) {
        fprintf(stderr, "Error: EELC-Main: Can't create Reiceive thread!");
        exit(1);
    }

    CPU_ZERO(&set);
    CPU_SET(4, &set);
    if(pthread_setaffinity_np(pthread_self(), sizeof(set), &set) != 0) {
        fprintf(stderr, "Error: EELC-Main: Can't set Main's cpu-affinity\n");
        exit(1);
    }
    CPU_ZERO(&set);
    CPU_SET(6, &set);
    if(pthread_setaffinity_np(replay_thread, sizeof(set), &set) != 0) {
        fprintf(stderr, "Error: EELC-Main: Can't set Replay's cpu-affinity\n");
        exit(1);
    }
    CPU_ZERO(&set);
    CPU_SET(8, &set);
    if(pthread_setaffinity_np(receive_thread, sizeof(set), &set) != 0) {
        fprintf(stderr, "Error: EELC-Main: Can't set Receive's cpu-affinity\n");
        exit(1);
    }


    if (pthread_join(replay_thread, &status) != 0) {
        fprintf(stderr, "Error: EELC-Main: Can't end Replay thread!");
        exit(2);
    }
    if (pthread_join(receive_thread, &status) != 0) {
        fprintf(stderr, "Error: EELC-Main: Can't end Receive thread!");
        exit(2);
    }

    fprintf(stdout, "EELC-Main: Replay and Receive threads run over\n");
    fprintf(stdout, "EELC-Main: Ready to compute latency and write to file\n");

    if ((fp = fopen(LATENCY_FILE, "w")) == NULL) {
        fprintf(stderr, "Erro: EELC-Main: Can't open file %s\n", LATENCY_FILE);
        exit(3);
    }

    for (int i = 0; i < TIME_RECORD_SIZE; i++) {
        start_time = start_time_record[i];
        end_time = end_time_record[i];
        // fprintf(stdout, "EELC-Main: %d %lus\n", i, end_time.tv_sec - start_time.tv_sec);
        // fprintf(stdout, "EELC-Main: %d %luns\n", i, end_time.tv_nsec - start_time.tv_nsec);
        // latency_record[i] = (end_time.tv_sec - start_time.tv_sec) * 1000000;
        // latency_record[i] += end_time.tv_usec - start_time.tv_usec;
        latency_record[i] = (end_time.tv_sec - start_time.tv_sec) * 1000000000;
        latency_record[i] += end_time.tv_nsec - start_time.tv_nsec;
        fprintf(
            fp, "%d %luus %luns\n", i, latency_record[i]/1000, latency_record[i]
        );
    }

    fclose(fp);

    return 0;
}
