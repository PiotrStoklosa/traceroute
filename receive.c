//Piotr Stoklosa 314987

#include <sys/time.h>
#include "receive.h"

int receive_packets(int sockfd, int ttl, char sender_ip_str[3][20], int *time, int *target) {

    int packets_received;
    uint16_t pid = getpid();
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    int start_time = 1000000;
    int sum_of_time = 0;
    for (packets_received = 0; packets_received < 3; packets_received++) {

        fd_set descriptors;
        FD_ZERO (&descriptors);
        FD_SET (sockfd, &descriptors);
        int ready = select(sockfd + 1, &descriptors, NULL, NULL, &tv);
        if (ready == -1) {
            fprintf(stderr, "select error");
            exit(EXIT_FAILURE);
        }
        if (ready == 1) {

            struct sockaddr_in sender;
            socklen_t sender_len = sizeof(sender);
            u_int8_t buffer[IP_MAXPACKET];

            ssize_t packet_len = recvfrom(sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr *) &sender, &sender_len);

            if (packet_len < 0) {
                fprintf(stderr, "recvfrom error");
                exit(EXIT_FAILURE);
            }

            struct ip *ip_header = (struct ip *) buffer;
            u_int8_t *icmp_packet = buffer + 4 * ip_header->ip_hl;
            struct icmp *icmp_header = (struct icmp *) icmp_packet;
            u_int8_t type = icmp_header->icmp_type;

            if (type == ICMP_TIME_EXCEEDED || type == ICMP_ECHOREPLY) {
                int packet_ttl;
                uint16_t packet_pid;
                if (type == ICMP_ECHOREPLY) {
                    *target = 1;
                    packet_ttl = icmp_header->icmp_hun.ih_idseq.icd_seq / 3;
                    packet_pid = icmp_header->icmp_hun.ih_idseq.icd_id;
                } else {
                    struct ip *ip_details = (struct ip *) ((uint8_t *) icmp_header + 8);
                    struct icmp *icmp_details = (struct icmp *) ((uint8_t *) ip_details + (*ip_details).ip_hl * 4);
                    packet_ttl = icmp_details->icmp_hun.ih_idseq.icd_seq / 3;
                    packet_pid = icmp_details->icmp_hun.ih_idseq.icd_id;
                }

                if (packet_ttl != ttl || packet_pid != pid) {
                    packets_received--;
                } else {
                    sum_of_time += start_time - tv.tv_usec;
                    inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str[packets_received],
                              sizeof(sender_ip_str[packets_received]));
                }

            }
        } else {
            break;
        }
    }
    if (packets_received) {
        *time = (sum_of_time / 1000) / packets_received;
    }

    return packets_received;

}

int receive(int sockfd, int ttl) {

    char sender_ip_str[3][20] = {0};
    int target = 0;
    int time = 0;

    int packets_received = receive_packets(sockfd, ttl, sender_ip_str, &time, &target);
    print_packets(packets_received, sender_ip_str, ttl, time);

    if (target == 1) {
        return 1;
    }
    return 0;
}

void print_packets(int packets_received, char sender_ip_str[3][20], int ttl, int time) {

    printf("%d. ", ttl);

    if (packets_received == 0) {
        printf("*\n");
    } else if (packets_received == 3) {
        printf("%s ", sender_ip_str[0]);
        if (strcmp(sender_ip_str[0], sender_ip_str[1]) != 0) {
            printf("%s ", sender_ip_str[1]);
        }
        if (strcmp(sender_ip_str[0], sender_ip_str[2]) != 0 && strcmp(sender_ip_str[1], sender_ip_str[2]) != 0) {
            printf("%s ", sender_ip_str[2]);
        }
        printf("%dms\n", time);
    } else {
        printf("%s ", sender_ip_str[0]);
        if (packets_received == 1) {
            printf("???\n");
        } else {
            if (strcmp(sender_ip_str[0], sender_ip_str[1]) != 0) {
                printf("%s ", sender_ip_str[1]);
            }
            printf("???\n");
        }
    }
}