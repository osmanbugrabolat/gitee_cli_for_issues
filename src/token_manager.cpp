#include "token_manager.hpp"
#include <iostream>
#include <libsecret/secret.h>

std::string get_token_from_keyring(const std::string& owner) {
    GError* error = nullptr;
    const SecretSchema schema = {
        "org.gitee.cli.Token", SECRET_SCHEMA_NONE,
        {
            { "owner", SECRET_SCHEMA_ATTRIBUTE_STRING },
            { nullptr, SECRET_SCHEMA_ATTRIBUTE_STRING }
        }
    };
    gchar* token = secret_password_lookup_sync(&schema, nullptr, &error, "owner", owner.c_str(), nullptr);
    if (error != nullptr) {
        std::cerr << "Keyring error: " << error->message << std::endl;
        g_error_free(error);
        return "";
    }
    if (!token) {
        std::cerr << "Token not found." << std::endl;
        return "";
    }
    std::string token_str(token);
    secret_password_free(token);
    return token_str;
}

bool store_token_to_keyring(const std::string& owner, const std::string& token) {
    GError* error = nullptr;
    const SecretSchema schema = {
        "org.gitee.cli.Token", SECRET_SCHEMA_NONE,
        {
            { "owner", SECRET_SCHEMA_ATTRIBUTE_STRING },
            { nullptr, SECRET_SCHEMA_ATTRIBUTE_STRING }
        }
    };
    secret_password_store_sync(&schema, nullptr, "Gitee CLI Token", token.c_str(), nullptr, &error,
                               "owner", owner.c_str(), nullptr);
    if (error != nullptr) {
        std::cerr << "Keyring error: " << error->message << std::endl;
        g_error_free(error);
        return false;
    }
    return true;
}

