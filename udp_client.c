#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

void error_handling(char *message) {
    perror(message);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE];
    int str_len;
    socklen_t serv_addr_size;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons(SERVER_PORT);

    while (1) {
        printf("Enter a message (q to quit): ");
        fgets(message, BUFFER_SIZE, stdin);
        if (strcmp(message, "q\n") == 0)
            break;

        sendto(sock, message, strlen(message), 0,
               (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        serv_addr_size = sizeof(serv_addr);
        str_len = recvfrom(sock, message, BUFFER_SIZE, 0,
                           (struct sockaddr*)&serv_addr, &serv_addr_size);
        message[str_len] = '\0';
        printf("Message from server: %s", message);
    }

    close(sock);
    return 0;
}

