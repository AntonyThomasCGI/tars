
#include <tars_intent.hpp>

#include <iostream>
#include <stdio.h>
#include <string>

#include <curl/curl.h>
#include <json/json.h>


static std::size_t WriteCallback(
            const char* in,
            std::size_t size,
            std::size_t num,
            std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
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
    std::unique_ptr<std::string> httpData(new std::string());

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
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform failed: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    // printf("response: %s\n", response.c_str());
    Json::Value jsonData;
    Json::Reader jsonReader;

    if (jsonReader.parse(*httpData, jsonData))
    {
        // std::cout << "Successfully parsed JSON data:" << std::endl;
        // std::cout << jsonData.toStyledString() << std::endl;

        intent.text = jsonData["text"].asString();
        intent.intent = jsonData["intents"][0]["name"].asString();
        intent.confidence = jsonData["intents"][0]["confidence"].asFloat();
    }
    

    return intent;
}
