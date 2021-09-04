
#include <tars_intent.hpp>

#include <stdio.h>
#include <curl/curl.h>


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


TARS_Intent TARS_getIntent(std::string raw_audio)
{
    TARS_Intent intent;
    CURL *curl;
    CURLcode res;
    std::string response;
    std::string url = "https://api.wit.ai/speech?v=20210904";
    // std::string data = "curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());";
    curl_global_init(CURL_GLOBAL_ALL);


    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl initialization failure!\n");
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: audio/raw;encoding=unsigned-integer;bits=16;rate=44100;endian=little");
    headers = curl_slist_append(headers, "Authorization: Bearer KPKYYRQERAK2TDM6XJT4EFT4OSFFNIR4");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, raw_audio.c_str());
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform failed: %s\n", curl_easy_strerror(res));
    }
    printf("response: %s\n", response.c_str());
    
    curl_easy_cleanup(curl);

    return intent;
}
