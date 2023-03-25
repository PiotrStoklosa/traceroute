//Piotr Stoklosa 314987

#include "send.h"
#include "receive.h"

void send_and_receive_packets(int sockfd, struct sockaddr_in recipient) {

    for (int ttl = 1; ttl <= 30; ttl++) {
        for (int packet_number = 0; packet_number < 3; packet_number++) {

            int sq_number = ttl * 3 + packet_number;
            struct icmp header;

            create_packet_to_send(&header, sq_number);
            set_ttl(sockfd, ttl);

            if (sendto(sockfd, &header, sizeof(header), 0, (struct sockaddr *) &recipient, sizeof(recipient)) < 0) {
                fprintf(stderr, "Sending packet with ttl %d and sequence number %d failed!\n", ttl, sq_number);
                exit(EXIT_FAILURE);
            }

        }

        if (receive(sockfd, ttl) == 1) {
            return;
        }
    }
}

u_int16_t compute_icmp_checksum(const void *buff, int length) {
    u_int32_t sum;
    const u_int16_t *ptr = buff;
    assert (length % 2 == 0);
    for (sum = 0; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16) + (sum & 0xffff);
    return (u_int16_t) (~(sum + (sum >> 16)));
}


void set_ttl(int sockfd, int ttl) {
    setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
}

void create_packet_to_send(struct icmp *header, int sq_number) {
    header->icmp_type = ICMP_ECHO;
    header->icmp_code = 0;
    header->icmp_hun.ih_idseq.icd_id = (uint16_t) getpid();
    header->icmp_hun.ih_idseq.icd_seq = sq_number;
    header->icmp_cksum = 0;
    header->icmp_cksum = compute_icmp_checksum((u_int16_t *) header, sizeof(*header));
}

