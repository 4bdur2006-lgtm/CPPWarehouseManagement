#ifndef WAREHOUSESYSTEM_H
#define WAREHOUSESYSTEM_H

#include <map>
#include <vector>
#include <string>
#include <climits>
#include "Product.h"
#include "Employee.h"
#include "Order.h"
#include "User.h"

class WarehouseSystem {
private:
    std::map<std::string, Product> products;
    std::map<std::string, Employee> employees;
    std::map<std::string, Order> orders;
    std::map<std::string, User> users;
    User* currentUser;

    // File paths
    std::string productsFile;
    std::string employeesFile;
    std::string ordersFile;
    std::string usersFile;

public:
    WarehouseSystem();
    ~WarehouseSystem();

    // Authentication
    bool login();
    void logout();

    // Current user access
    UserRole getCurrentUserRole() const;
    bool isUserLoggedIn() const;

    // File operations
    void loadData();
    void saveData();
    void exportToCSV(const std::string& filename, const std::string& data);

    // Manager operations
    void managerMenu();
    void addProduct();
    void viewProducts();
    void editProduct();
    void addEmployee();
    void viewEmployees();
    void editEmployee();
    void createOrder();
    void viewOrders();
    void editOrder();
    void assignOrder();
    void generateReports();

    // Employee operations
    void employeeMenu();
    void viewAssignedOrders();
    void findProduct();
    void completeOrder();
    void generateEmployeeReport();

    // Helper functions
    Product* findProductById(const std::string& id);
    Product* findProductByName(const std::string& name);
    Employee* findEmployeeById(const std::string& id);
    Order* findOrderById(const std::string& id);

    // Input validation
    int getValidatedInt(const std::string& prompt, int min = INT_MIN, int max = INT_MAX);
    double getValidatedDouble(const std::string& prompt, double min = -1e9, double max = 1e9);
    std::string getValidatedString(const std::string& prompt);

private:
    void loadProducts();
    void loadEmployees();
    void loadOrders();
    void loadUsers();
    void saveProducts();
    void saveEmployees();
    void saveOrders();
    void saveUsers();

    // Report generation
    std::string generateInventoryReport();
    std::string generateOrdersReport();
    std::string generatePendingOrdersReport();
    std::string generateCompletedOrdersReport();
};

#endif