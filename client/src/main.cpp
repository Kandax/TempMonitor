#include <iostream>
#include <curl/curl.h>


int main()
{
    CURL* curl;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();


    if(curl)
    {
        curl_easy_setopt(
            curl,
            CURLOPT_URL,
            "http://localhost:3000/temperature"
        );


        curl_easy_setopt(
            curl,
            CURLOPT_POSTFIELDS,
            "{\"temperature\":25.5}"
        );


        CURLcode result = curl_easy_perform(curl);


        if(result != CURLE_OK)
        {
            std::cout 
                << "curl error: "
                << curl_easy_strerror(result)
                << "\n";
        }


        curl_easy_cleanup(curl);
    }


    curl_global_cleanup();
}