//Piotr Stoklosa 314987

#ifndef RECIEVE_H
#define RECIEVE_H

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>

int receive(int sockfd, int ttl);

int receive_packets(int sockfd, int ttl, char sender_ip_str[3][20], int *time, int *target);

void print_packets(int packets_received, char sender_ip_str[3][20], int ttl, int time);


#endif
