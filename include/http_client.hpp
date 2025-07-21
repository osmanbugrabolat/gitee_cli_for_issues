#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <string>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
bool send_request(CURL* curl, const std::string& url, const std::string& method,
                  const std::string& post_fields, std::string& response, long& http_code);

#endif

