#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PORT 8888

int main(int argc, char *argv[]) {
    int                serv_sock;
    char               message[BUFFER_SIZE];
    int                str_len;
    int                ret;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t          clnt_addr_size;

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(PORT);

    ret = bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    while (1) {
        clnt_addr_size   = sizeof(clnt_addr);
        str_len          = recvfrom(serv_sock, message, BUFFER_SIZE, 0, (struct sockaddr *) &clnt_addr, &clnt_addr_size);
        message[str_len] = '\0';
        printf("Received message: %s", message);
        sendto(serv_sock, message, str_len, 0, (struct sockaddr *) &clnt_addr, clnt_addr_size);
    }

    close(serv_sock);
    return 0;
}
