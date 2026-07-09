#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <vector>

class Employee {
private:
    std::string employeeId;
    std::string name;
    std::string position;
    std::vector<std::string> assignedOrders;

public:
    Employee();
    Employee(std::string id, std::string name, std::string position);

    std::string getEmployeeId() const;
    std::string getName() const;
    std::string getPosition() const;
    std::vector<std::string> getAssignedOrders() const;

    void setEmployeeId(const std::string& id);
    void setName(const std::string& name);
    void setPosition(const std::string& position);

    void addAssignedOrder(const std::string& orderId);
    void removeAssignedOrder(const std::string& orderId);
    void clearAssignedOrders();

    void display() const;
    std::string toCSV() const;
    static Employee fromCSV(const std::string& csvLine);
};

#endif