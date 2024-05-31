#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

#include <openssl/err.h>
#include <openssl/ssl.h>

int main(void) {
    int ret, socket_fd;

    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    SSL_CTX         *ssl_ctx = SSL_CTX_new(SSLv23_client_method());
    struct addrinfo  hints   = {.ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM};
    struct addrinfo *result  = NULL;
    ret                      = getaddrinfo("storage.googleapis.com", "443", &hints, &result);
    socket_fd                = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    ret                      = connect(socket_fd, result->ai_addr, result->ai_addrlen);
    SSL *ssl                 = SSL_new(ssl_ctx);
    SSL_set_fd(ssl, socket_fd);

    if (SSL_connect(ssl) != 1) {
        ERR_print_errors_fp(stderr);
    } else {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        SSL_write(ssl,
                  "GET /gtv-videos-bucket/sample/BigBuckBunny.mp4 HTTP/1.1\r\nHost: storage.googleapis.com\r\nConnection: close\r\n\r\n",
                  strlen("GET /gtv-videos-bucket/sample/BigBuckBunny.mp4 HTTP/1.1\r\nHost: storage.googleapis.com\r\nConnection: close\r\n\r\n"));

        char buf[1024 * 20];
        int  bytes;
        while ((bytes = SSL_read(ssl, buf, sizeof(buf) - 1)) > 0) {
            printf("Hello World %d\n", bytes);
            buf[bytes] = '\0';
        }
    }

    freeaddrinfo(result);
    SSL_free(ssl);
    close(socket_fd);
    SSL_CTX_free(ssl_ctx);
    EVP_cleanup();
}
