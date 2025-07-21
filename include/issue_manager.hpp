#ifndef ISSUE_MANAGER_HPP
#define ISSUE_MANAGER_HPP

#include <string>

int create_issue(const std::string& owner, const std::string& repo,
                 const std::string& title, const std::string& body,
                 const std::string& issue_type);

int update_issue(const std::string& owner, const std::string& repo,
                 const std::string& number, const std::string& title,
                 const std::string& body, const std::string& state);

int list_issues(const std::string& owner, const std::string& repo);

#endif

