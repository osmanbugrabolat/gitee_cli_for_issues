#include "issue_manager.hpp"
#include "token_manager.hpp"
#include "http_client.hpp"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int create_issue(const std::string& owner, const std::string& repo,
                 const std::string& title, const std::string& body,
                 const std::string& issue_type) {
    std::string token = get_token_from_keyring(owner);
    if (token.empty()) return 1;

    CURL* curl = curl_easy_init();
    if (!curl) return 1;

    std::string url = "https://gitee.com/api/v5/repos/" + owner + "/issues";
    char* title_e = curl_easy_escape(curl, title.c_str(), 0);
    char* body_e = curl_easy_escape(curl, body.c_str(), 0);
    char* token_e = curl_easy_escape(curl, token.c_str(), 0);
    char* repo_e = curl_easy_escape(curl, repo.c_str(), 0);
    char* type_e = curl_easy_escape(curl, issue_type.c_str(), 0);

    std::string post = "access_token=" + std::string(token_e) + "&repo=" + std::string(repo_e)
                     + "&title=" + std::string(title_e) + "&body=" + std::string(body_e)
                     + "&issue_type=" + std::string(type_e);

    std::string response;
    long http_code;
    bool ok = send_request(curl, url, "POST", post, response, http_code);

    std::cout << "HTTP code: " << http_code << std::endl;
    std::cout << "Response: " << response << std::endl;

    if (ok && http_code == 201)
        std::cout << "Issue created successfully." << std::endl;
    else
        std::cerr << "Failed to create issue." << std::endl;

    curl_free(title_e); curl_free(body_e); curl_free(token_e); curl_free(repo_e); curl_free(type_e);
    curl_easy_cleanup(curl);
    return ok ? 0 : 1;
}

int update_issue(const std::string& owner, const std::string& repo,
                 const std::string& number, const std::string& title,
                 const std::string& body, const std::string& state) {
    std::string token = get_token_from_keyring(owner);
    if (token.empty()) return 1;

    CURL* curl = curl_easy_init();
    if (!curl) return 1;

    std::string url = "https://gitee.com/api/v5/repos/" + owner + "/issues/" + number;
    char* token_e = curl_easy_escape(curl, token.c_str(), 0);
    char* repo_e = curl_easy_escape(curl, repo.c_str(), 0);

    std::string post = "access_token=" + std::string(token_e) + "&repo=" + std::string(repo_e);

    if (!title.empty()) {
        char* e = curl_easy_escape(curl, title.c_str(), 0);
        post += "&title=" + std::string(e);
        curl_free(e);
    }
    if (!body.empty()) {
        char* e = curl_easy_escape(curl, body.c_str(), 0);
        post += "&body=" + std::string(e);
        curl_free(e);
    }
    if (!state.empty()) {
        char* e = curl_easy_escape(curl, state.c_str(), 0);
        post += "&state=" + std::string(e);
        curl_free(e);
    }

    std::string response;
    long http_code;
    bool ok = send_request(curl, url, "PATCH", post, response, http_code);

    std::cout << "HTTP code: " << http_code << std::endl;
    std::cout << "Response: " << response << std::endl;

    if (ok && http_code == 200)
        std::cout << "Issue updated successfully." << std::endl;
    else
        std::cerr << "Failed to update issue." << std::endl;

    curl_free(token_e); curl_free(repo_e);
    curl_easy_cleanup(curl);
    return ok ? 0 : 1;
}

int list_issues(const std::string& owner, const std::string& repo) {
    std::string token = get_token_from_keyring(owner);
    if (token.empty()) return 1;

    CURL* curl = curl_easy_init();
    if (!curl) return 1;

    char* token_e = curl_easy_escape(curl, token.c_str(), 0);
    std::string url = "https://gitee.com/api/v5/repos/" + owner + "/" + repo + "/issues"
                    + "?access_token=" + token_e;

    std::string response;
    long http_code;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_cleanup(curl);
    curl_free(token_e);

    std::cout << "HTTP code: " << http_code << std::endl;
    if (http_code != 200) {
        std::cerr << "Failed to list issues. Response:\n" << response << std::endl;
        return 1;
    }

    try {
        json issues = json::parse(response);
        for (const auto& issue : issues) {
            std::string number = issue["number"].is_string()
                                ? issue["number"].get<std::string>()
                                : std::to_string(issue["number"].get<int>());
            std::string title = issue["title"];
            std::string state = issue["state"];
            std::string body = issue["body"];
            std::cout << "#" << number << " [" << state << "] " << title << "\n    " << body << "\n" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

