#include "User.h"
#include <sstream>
#include <vector>
#include <string>

User::User() : username(""), password(""), role(UserRole::EMPLOYEE), employeeId("") {}

User::User(std::string uname, std::string pwd, UserRole role, std::string empId)
    : username(uname), password(pwd), role(role), employeeId(empId) {
}

std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
UserRole User::getRole() const { return role; }
std::string User::getEmployeeId() const { return employeeId; }

void User::setUsername(const std::string& uname) { username = uname; }
void User::setPassword(const std::string& pwd) { password = pwd; }
void User::setRole(UserRole newRole) { role = newRole; }
void User::setEmployeeId(const std::string& empId) { employeeId = empId; }

bool User::authenticate(const std::string& inputPassword) const {
    return password == inputPassword;
}

std::string User::roleToString() const {
    switch (role) {
    case UserRole::MANAGER: return "Manager";
    case UserRole::EMPLOYEE: return "Employee";
    default: return "Unknown";
    }
}

std::string User::toCSV() const {
    return username + "," + password + "," +
        std::to_string(static_cast<int>(role)) + "," + employeeId;
}

User User::fromCSV(const std::string& csvLine) {
    std::stringstream ss(csvLine);
    std::string token;
    std::vector<std::string> tokens;

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() == 4) {
        return User(tokens[0], tokens[1],
            static_cast<UserRole>(std::stoi(tokens[2])),
            tokens[3]);
    }

    return User();
}