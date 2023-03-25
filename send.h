//Piotr Stoklosa 314987

#ifndef SEND_H
#define SEND_H

#include <netinet/ip_icmp.h>
#include <assert.h>

void create_packet_to_send(struct icmp *header, int sq_number);

void send_and_receive_packets(int sockfd, struct sockaddr_in recipient);

void set_ttl(int sockfd, int ttl);

#endif
