#pragma once

#include "utils.h"

namespace gearforge {

enum class UserRole { User, Admin };

struct User {
    std::string username;
    std::string password_hash;  // SHA256
    UserRole role;

    std::vector<std::string> to_csv_row() const;
    static User from_csv_row(const std::vector<std::string>& row);
};

class UserManager {
private:
    std::vector<User> users;
    std::mutex mutex;
    std::string filename = "data/users.csv";
    User current_user;

    void load_users();
    void save_users();

public:
    UserManager();
    bool register_user(const std::string& username, const std::string& password, UserRole role = UserRole::User);
    bool login(const std::string& username, const std::string& password);
    const User& get_current_user() const;
    bool has_permission(UserRole required) const;  // Simple check
};

}  // namespace gearforge
