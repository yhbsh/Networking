#include <curl/curl.h>
#include <stdio.h>

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    (void) stream;
    (void) ptr;
    printf("Hello World %zu\n", nmemb);
    return size * nmemb;
}

int main(void) {
    CURL    *curl;
    CURLcode ret;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "https://storage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    ret = curl_easy_perform(curl);

    if (ret != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
        return 1;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
