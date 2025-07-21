#ifndef KEYRING_MANAGER_HPP
#define KEYRING_MANAGER_HPP

#include <string>

class KeyringManager {
public:
    static std::string getToken(const std::string& owner);
    static bool storeToken(const std::string& owner, const std::string& token);
};

#endif // KEYRING_MANAGER_HPP
