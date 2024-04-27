#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl() {
    EVP_cleanup();
}

int create_socket(char* hostname, char* port) {
    struct addrinfo hints, *res, *result;
    int s;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, port, &hints, &result) != 0) {
        perror("getaddrinfo failed");
        exit(EXIT_FAILURE);
    }

    for (res = result; res != NULL; res = res->ai_next) {
        s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (s == -1) continue;

        if (connect(s, res->ai_addr, res->ai_addrlen) == 0) {
            break;  
        }

        close(s);
    }

    if (res == NULL) {  
        perror("Could not connect");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);  
    return s;
}

int main(void) {
    struct timeval start_time, end_time;
    double elapsed_time;

    init_openssl();

    gettimeofday(&start_time, NULL);  

    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    int sock = create_socket("storage.googleapis.com", "443");
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);

    if (SSL_connect(ssl) != 1) {
        ERR_print_errors_fp(stderr);
    } else {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        SSL_write(ssl, "GET /gtv-videos-bucket/sample/BigBuckBunny.mp4 HTTP/1.1\r\nHost: storage.googleapis.com\r\nConnection: close\r\n\r\n", strlen("GET /gtv-videos-bucket/sample/BigBuckBunny.mp4 HTTP/1.1\r\nHost: storage.googleapis.com\r\nConnection: close\r\n\r\n"));
        char buf[1024 * 20];
        int bytes;
        while ((bytes = SSL_read(ssl, buf, sizeof(buf) - 1)) > 0) {
            printf("Hello World %d\n", bytes);
            buf[bytes] = 0;
        }
    }

    gettimeofday(&end_time, NULL);  // End timing

    elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("Total time taken: %.6f seconds\n", elapsed_time);

    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}

