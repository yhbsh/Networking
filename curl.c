#include <stdio.h>
#include <curl/curl.h>
#include <sys/time.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    (void)stream;
    (void)ptr;
    printf("Hello World %zu\n", nmemb);
    return size * nmemb;
}

int main(void) {
    CURL *curl;
    CURLcode res;
    struct timeval start_time, end_time;
    double elapsed_time;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "[ERROR]: curl_easy_init() failed\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://storage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    gettimeofday(&start_time, NULL);  // Start timing
    res = curl_easy_perform(curl);
    gettimeofday(&end_time, NULL);    // End timing

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        // Calculate the elapsed time in seconds as a double with microseconds precision
        elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
        printf("Download completed in %.6f seconds.\n", elapsed_time);
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}

