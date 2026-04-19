#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define TOKEN     "YOUR API KEY"
#define URL       "https://hjonk.me/api/token/post/create/"
#define PLATFORM  "curl"
#define MAXLEN    500

int main(void) {
    char content[MAXLEN + 1];
    char json[MAXLEN + 256];

    printf("Content (%d char limit): ", MAXLEN);
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = '\0';

    if (strlen(content) > MAXLEN) {
        fprintf(stderr, "error: content exceeds %d characters\n", MAXLEN);
        return 1;
    }

    snprintf(json, sizeof(json),
        "{\"token\":\"%s\", \"content\":\"%s\", \"platform\":\"%s\"}",
        TOKEN, content, PLATFORM);

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "error: failed to init curl\n");
        return 1;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, URL);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        fprintf(stderr, "curl error: %s\n", curl_easy_strerror(res));

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    printf("\nHTTP %ld\n", http_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return 0;
}
