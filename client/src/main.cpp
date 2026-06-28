#include <iostream>
#include <curl/curl.h>

size_t WriteCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp
)
{
    size_t totalSize = size * nmemb;

    std::string* response = static_cast<std::string*>(userp);
    response->append(
        static_cast<char*>(contents),
        totalSize
    );

    return totalSize;
}

int main()
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        std::cerr << "Nie udalo sie zainicjalizowac CURL\n";
        return 1;
    }

    std::string response;

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        "https://httpbin.org/get"
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        WriteCallback
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response
    );

    CURLcode result = curl_easy_perform(curl);

    if (result != CURLE_OK)
    {
        std::cerr
            << "Blad CURL: "
            << curl_easy_strerror(result)
            << "\n";

        curl_easy_cleanup(curl);
        return 1;
    }

    std::cout << "Odpowiedz serwera:\n";
    std::cout << response << "\n";

    curl_easy_cleanup(curl);

    return 0;
}