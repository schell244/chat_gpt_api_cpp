#include <iostream>
#include <json/json.h>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main()
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl) {
        // Set the URL for the ChatGPT API
        // curl_easy_setopt(curl, CURLOPT_CAINFO, "path/to/cacert.pem");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/engine/davinci-codex/completions");

        // Set the HTTP request headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer YOUR_API_KEY");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the HTTP request data (JSON payload)
        Json::Value data;
        data["prompt"] = "Hallo, wie geht es dir?";
        std::string json_data = data.toStyledString();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

        // Set the callback for the HTTP response data
        std::string response_data;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // Perform the HTTP request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        // Parse the JSON response from the ChatGPT API
        Json::Value response_json;
        Json::Reader reader;
        reader.parse(response_data, response_json);

        // Print the response from the ChatGPT API
        std::cout << response_json["choices"][0]["text"].asString() << std::endl;
    }

    return 0;
}