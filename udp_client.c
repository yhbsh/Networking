#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int                sock;
    struct sockaddr_in server_addr = {.sin_family = AF_INET, .sin_addr.s_addr = inet_addr(SERVER_IP), .sin_port = htons(SERVER_PORT)};
    char               message[BUFFER_SIZE];
    int                str_len;
    socklen_t          serv_addr_size;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror(message);
        return 1;
    }

    while (1) {
        printf("Enter a message (q to quit): ");
        fgets(message, BUFFER_SIZE, stdin);
        if (strcmp(message, "q\n") == 0) break;
        sendto(sock, message, strlen(message), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
        serv_addr_size   = sizeof(server_addr);
        str_len          = recvfrom(sock, message, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, &serv_addr_size);
        message[str_len] = '\0';
        printf("Message from server: %s", message);
    }

    close(sock);
    return 0;
}
