#ifndef USER_H
#define USER_H

#include <string>

enum class UserRole {
    MANAGER,
    EMPLOYEE
};

class User {
private:
    std::string username;
    std::string password;
    UserRole role;
    std::string employeeId; 

public:
    User();
    User(std::string uname, std::string pwd, UserRole role, std::string empId = "");

    std::string getUsername() const;
    std::string getPassword() const;
    UserRole getRole() const;
    std::string getEmployeeId() const;

    void setUsername(const std::string& uname);
    void setPassword(const std::string& pwd);
    void setRole(UserRole newRole);
    void setEmployeeId(const std::string& empId);

    bool authenticate(const std::string& inputPassword) const;
    std::string roleToString() const;

    std::string toCSV() const;
    static User fromCSV(const std::string& csvLine);
};

#endif