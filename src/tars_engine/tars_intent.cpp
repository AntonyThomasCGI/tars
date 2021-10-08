
#include <tars_intent.hpp>

#include <iostream>
#include <stdio.h>
#include <string>
#include <curl/curl.h>

// #include "rapidjson/document.h"
// #include "rapidjson/writer.h"
// #include "rapidjson/stringbuffer.h"


static std::size_t WriteCallback(void *contents, std::size_t size, std::size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


// size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
// {
//   FILE *readhere = (FILE *)userdata;
//   curl_off_t nread;

//   /* copy as much data as possible into the 'ptr' buffer, but no more than
//      'size' * 'nmemb' bytes! */
//   size_t retcode = fread(ptr, size, nmemb, readhere);

//   nread = (curl_off_t)retcode;

//   fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
//           " bytes from file\n", nread);
//   return retcode;
// }


TARS_Intent TARS_getIntent(const char *audioBinary, std::size_t binaryLen)
{
    TARS_Intent intent;
    CURL *curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_ALL);


    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl initialization failure!\n");
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.wit.ai/speech?v=20210904");
    // curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: audio/raw;encoding=signed-integer;bits=16;rate=16k;endian=little");
    headers = curl_slist_append(headers, "Authorization: Bearer KPKYYRQERAK2TDM6XJT4EFT4OSFFNIR4");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, audioBinary);
    // curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    // curl_easy_setopt(curl, CURLOPT_READDATA, (void *)audio_file);
    // long defaultLen = -1;
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, binaryLen); // -1 makes curl to strlen (default anyway)
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform failed: %s\n", curl_easy_strerror(res));
    }
    // printf("response: %s\n", response.c_str());
    std::cout << response << std::endl;
    
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return intent;
}
