//Piotr Stoklosa 314987

#include "traceroute.h"
#include "send.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "You need to pass one parameter (IP), example: ./traceroute 8.8.8.8\n");
        return EXIT_FAILURE;
    }
    char *address_ip = argv[1];
    traceroute(address_ip);
}

void traceroute(char *address_ip) {

    struct sockaddr_in recipient;
    bzero(&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;

    if (!inet_pton(AF_INET, address_ip, &recipient.sin_addr)) {
        fprintf(stderr, "Provide valid address IP!\n");
        exit(EXIT_FAILURE);
    }

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        fprintf(stderr, "The error occurred during creation of raw socket!\n");
        exit(EXIT_FAILURE);
    }
    send_and_receive_packets(sockfd, recipient);
}
