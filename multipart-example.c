#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define TOKEN     "YOUR API KEY"
#define URL       "https://hjonk.me/api/token/post/create/"
#define PLATFORM  "curl"
#define MAXLEN    500
int main(void) {
    char content[MAXLEN + 1];
    char imgpath[1024];

    printf("Content (%d char limit): ", MAXLEN);
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = '\0';

    if (strlen(content) > MAXLEN) {
        fprintf(stderr, "error: content exceeds %d characters\n", MAXLEN);
        return 1;
    }

    printf("Image path: ");
    fgets(imgpath, sizeof(imgpath), stdin);
    imgpath[strcspn(imgpath, "\n")] = '\0';

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "error: failed to init curl\n");
        return 1;
    }

    // Build multipart form
    curl_mime *form = curl_mime_init(curl);

    curl_mimepart *part;

    part = curl_mime_addpart(form);
    curl_mime_name(part, "token");
    curl_mime_data(part, TOKEN, CURL_ZERO_TERMINATED);

    part = curl_mime_addpart(form);
    curl_mime_name(part, "content");
    curl_mime_data(part, content, CURL_ZERO_TERMINATED);

    part = curl_mime_addpart(form);
    curl_mime_name(part, "platform");
    curl_mime_data(part, PLATFORM, CURL_ZERO_TERMINATED);

    part = curl_mime_addpart(form);
    curl_mime_name(part, "files[]");
    curl_mime_filedata(part, imgpath);

    curl_easy_setopt(curl, CURLOPT_URL, URL);
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        fprintf(stderr, "curl error: %s\n", curl_easy_strerror(res));

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    printf("\nHTTP %ld\n", http_code);

    curl_mime_free(form);
    curl_easy_cleanup(curl);
    return 0;
}
