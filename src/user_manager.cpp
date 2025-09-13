#include "user_manager.h"

namespace gearforge {

std::vector<std::string> User::to_csv_row() const {
    return {username, password_hash, role == UserRole::Admin ? "Admin" : "User"};
}

User User::from_csv_row(const std::vector<std::string>& row) {
    if (row.size() < 3) throw std::runtime_error("Invalid user CSV");
    User u;
    u.username = row[0];
    u.password_hash = row[1];
    u.role = row[2] == "Admin" ? UserRole::Admin : UserRole::User;
    return u;
}

UserManager::UserManager() {
    load_users();
}

void UserManager::load_users() {
    std::lock_guard<std::mutex> lock(mutex);
    auto csv = utils::read_csv(filename);
    users.clear();
    for (size_t i = 1; i < csv.size(); ++i) {
        users.push_back(User::from_csv_row(csv[i]));
    }
}

void UserManager::save_users() {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<std::vector<std::string>> data;
    data.push_back({"Username", "PasswordHash", "Role"});
    for (const auto& u : users) {
        data.push_back(u.to_csv_row());
    }
    utils::write_csv(filename, data);
}

bool UserManager::register_user(const std::string& username, const std::string& password, UserRole role) {
    auto hash = utils::sha256(password);
    // Convert hash to string (e.g., hex)
    std::stringstream ss;
    for (auto v : hash.state) ss << std::hex << std::setw(8) << std::setfill('0') << v;
    std::string hash_str = ss.str();

    for (const auto& u : users) {
        if (u.username == username) return false;  // Exists
    }
    users.push_back({username, hash_str, role});
    save_users();
    return true;
}

bool UserManager::login(const std::string& username, const std::string& password) {
    auto hash = utils::sha256(password);
    std::stringstream ss;
    for (auto v : hash.state) ss << std::hex << std::setw(8) << std::setfill('0') << v;
    std::string hash_str = ss.str();

    for (const auto& u : users) {
        if (u.username == username && u.password_hash == hash_str) {
            current_user = u;
            return true;
        }
    }
    return false;
}

const User& UserManager::get_current_user() const {
    return current_user;
}

bool UserManager::has_permission(UserRole required) const {
    return current_user.role >= required;
}

}  // namespace gearforge
