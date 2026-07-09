#include "Employee.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Employee::Employee() : employeeId(""), name(""), position("Employee") {}

Employee::Employee(std::string id, std::string name, std::string position)
    : employeeId(id), name(name), position(position) {
}

std::string Employee::getEmployeeId() const { return employeeId; }
std::string Employee::getName() const { return name; }
std::string Employee::getPosition() const { return position; }
std::vector<std::string> Employee::getAssignedOrders() const { return assignedOrders; }

void Employee::setEmployeeId(const std::string& id) { employeeId = id; }
void Employee::setName(const std::string& name) { this->name = name; }
void Employee::setPosition(const std::string& position) { this->position = position; }

void Employee::addAssignedOrder(const std::string& orderId) {
    assignedOrders.push_back(orderId);
}

void Employee::removeAssignedOrder(const std::string& orderId) {
    assignedOrders.erase(
        std::remove(assignedOrders.begin(), assignedOrders.end(), orderId),
        assignedOrders.end()
    );
}

void Employee::clearAssignedOrders() {
    assignedOrders.clear();
}

void Employee::display() const {
    std::cout << "Employee ID: " << employeeId << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Position: " << position << "\n";
    std::cout << "Assigned Orders: ";
    for (const auto& order : assignedOrders) {
        std::cout << order << " ";
    }
    std::cout << "\n-------------------------\n";
}

std::string Employee::toCSV() const {
    std::string ordersStr;
    for (size_t i = 0; i < assignedOrders.size(); i++) {
        ordersStr += assignedOrders[i];
        if (i < assignedOrders.size() - 1) ordersStr += ";";
    }
    return employeeId + "," + name + "," + position + "," + ordersStr;
}

Employee Employee::fromCSV(const std::string& csvLine) {
    std::stringstream ss(csvLine);
    std::string token;
    std::vector<std::string> tokens;

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() >= 3) {
        Employee emp(tokens[0], tokens[1], tokens[2]);

        if (tokens.size() > 3 && !tokens[3].empty()) {
            std::stringstream ordersStream(tokens[3]);
            std::string orderId;
            while (getline(ordersStream, orderId, ';')) {
                emp.addAssignedOrder(orderId);
            }
        }

        return emp;
    }

    return Employee();
}