#ifndef TOKEN_MANAGER_HPP
#define TOKEN_MANAGER_HPP

#include <string>

std::string get_token_from_keyring(const std::string& owner);
bool store_token_to_keyring(const std::string& owner, const std::string& token);

#endif

