#include "WarehouseSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <ctime>

using namespace std;

WarehouseSystem::WarehouseSystem() : currentUser(nullptr) {
    productsFile = "data/products.csv";
    employeesFile = "data/employees.csv";
    ordersFile = "data/orders.csv";
    usersFile = "data/users.csv";
    loadData();
}

WarehouseSystem::~WarehouseSystem() {
    saveData();
    if (currentUser) delete currentUser;
}

bool WarehouseSystem::login() {
    cout << "\n=== Warehouse Inventory System Login ===\n";
    string username, password;

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    auto it = users.find(username);
    if (it != users.end() && it->second.authenticate(password)) {
        currentUser = new User(it->second);
        cout << "\nLogin successful! Welcome, " << username << " ("
            << currentUser->roleToString() << ")\n";
        return true;
    }

    cout << "\nInvalid username or password!\n";
    return false;
}

void WarehouseSystem::logout() {
    if (currentUser) {
        cout << "\nGoodbye, " << currentUser->getUsername() << "!\n";
        delete currentUser;
        currentUser = nullptr;
    }
}

UserRole WarehouseSystem::getCurrentUserRole() const {
    if (currentUser != nullptr) {
        return currentUser->getRole();
    }
    return UserRole::EMPLOYEE;
}

bool WarehouseSystem::isUserLoggedIn() const {
    return currentUser != nullptr;
}

void WarehouseSystem::loadData() {
    loadProducts();
    loadEmployees();
    loadOrders();
    loadUsers();
}

void WarehouseSystem::saveData() {
    saveProducts();
    saveEmployees();
    saveOrders();
    saveUsers();
}

void WarehouseSystem::loadProducts() {
    ifstream file(productsFile);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        Product product = Product::fromCSV(line);
        if (!product.getProductId().empty()) {
            products[product.getProductId()] = product;
        }
    }
    file.close();
}

void WarehouseSystem::loadEmployees() {
    ifstream file(employeesFile);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        Employee employee = Employee::fromCSV(line);
        if (!employee.getEmployeeId().empty()) {
            employees[employee.getEmployeeId()] = employee;
        }
    }
    file.close();
}

void WarehouseSystem::loadOrders() {
    ifstream file(ordersFile);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        Order order = Order::fromCSV(line);
        if (!order.getOrderId().empty()) {
            orders[order.getOrderId()] = order;
        }
    }
    file.close();
}

void WarehouseSystem::loadUsers() {
    ifstream file(usersFile);
    if (!file.is_open()) {
        // Create default users if file doesn't exist
        User manager("manager", "admin123", UserRole::MANAGER, "M001");
        User employee("employee", "emp123", UserRole::EMPLOYEE, "E001");
        users["manager"] = manager;
        users["employee"] = employee;
        saveUsers();
        return;
    }

    string line;
    while (getline(file, line)) {
        User user = User::fromCSV(line);
        if (!user.getUsername().empty()) {
            users[user.getUsername()] = user;
        }
    }
    file.close();
}

void WarehouseSystem::saveProducts() {
    ofstream file(productsFile);
    if (!file.is_open()) return;

    for (const auto& pair : products) {
        file << pair.second.toCSV() << endl;
    }
    file.close();
}

void WarehouseSystem::saveEmployees() {
    ofstream file(employeesFile);
    if (!file.is_open()) return;

    for (const auto& pair : employees) {
        file << pair.second.toCSV() << endl;
    }
    file.close();
}

void WarehouseSystem::saveOrders() {
    ofstream file(ordersFile);
    if (!file.is_open()) return;

    for (const auto& pair : orders) {
        file << pair.second.toCSV() << endl;
    }
    file.close();
}

void WarehouseSystem::saveUsers() {
    ofstream file(usersFile);
    if (!file.is_open()) return;

    for (const auto& pair : users) {
        file << pair.second.toCSV() << endl;
    }
    file.close();
}

void WarehouseSystem::managerMenu() {
    int choice;
    do {
        cout << "\n=== MANAGER MENU ===\n";
        cout << "1. Add Product\n";
        cout << "2. View Products\n";
        cout << "3. Edit Product\n";
        cout << "4. Add Employee\n";
        cout << "5. View Employees\n";
        cout << "6. Edit Employee\n";
        cout << "7. Create Order\n";
        cout << "8. View Orders\n";
        cout << "9. Edit Order\n";
        cout << "10. Assign Order\n";
        cout << "11. Generate Reports\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";

        choice = getValidatedInt("", 0, 11);

        switch (choice) {
        case 1: addProduct(); break;
        case 2: viewProducts(); break;
        case 3: editProduct(); break;
        case 4: addEmployee(); break;
        case 5: viewEmployees(); break;
        case 6: editEmployee(); break;
        case 7: createOrder(); break;
        case 8: viewOrders(); break;
        case 9: editOrder(); break;
        case 10: assignOrder(); break;
        case 11: generateReports(); break;
        case 0: logout(); break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 0 && currentUser != nullptr);
}

void WarehouseSystem::employeeMenu() {
    int choice;
    do {
        cout << "\n=== EMPLOYEE MENU ===\n";
        cout << "1. View Assigned Orders\n";
        cout << "2. Find Product\n";
        cout << "3. Complete Order\n";
        cout << "4. Generate Report\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";

        choice = getValidatedInt("", 0, 4);

        switch (choice) {
        case 1: viewAssignedOrders(); break;
        case 2: findProduct(); break;
        case 3: completeOrder(); break;
        case 4: generateEmployeeReport(); break;
        case 0: logout(); break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 0 && currentUser != nullptr);
}

// Manager Operations Implementation
void WarehouseSystem::addProduct() {
    cout << "\n=== Add New Product ===\n";

    string id = getValidatedString("Enter Product ID: ");
    if (products.find(id) != products.end()) {
        cout << "Product ID already exists!\n";
        return;
    }

    string name = getValidatedString("Enter Product Name: ");
    int quantity = getValidatedInt("Enter Quantity: ", 0);
    string location = getValidatedString("Enter Location (e.g., Aisle C, Shelf 4): ");
    double price = getValidatedDouble("Enter Price: $", 0.0);

    Product newProduct(id, name, quantity, location, price);
    products[id] = newProduct;

    cout << "\nProduct added successfully!\n";
    newProduct.display();
}

void WarehouseSystem::viewProducts() {
    cout << "\n=== Product List ===\n";
    if (products.empty()) {
        cout << "No products found.\n";
        return;
    }

    for (const auto& pair : products) {
        pair.second.display();
    }
}

void WarehouseSystem::editProduct() {
    cout << "\n=== Edit Product ===\n";
    string id = getValidatedString("Enter Product ID to edit: ");

    auto it = products.find(id);
    if (it == products.end()) {
        cout << "Product not found!\n";
        return;
    }

    Product& product = it->second;
    product.display();

    cout << "\nWhat would you like to edit?\n";
    cout << "1. Name\n";
    cout << "2. Quantity\n";
    cout << "3. Location\n";
    cout << "4. Price\n";
    cout << "5. All\n";
    cout << "0. Cancel\n";

    int choice = getValidatedInt("Enter choice: ", 0, 5);

    switch (choice) {
    case 1: {
        string name = getValidatedString("Enter new name: ");
        product.setName(name);
        break;
    }
    case 2: {
        int quantity = getValidatedInt("Enter new quantity: ", 0);
        product.setQuantity(quantity);
        break;
    }
    case 3: {
        string location = getValidatedString("Enter new location: ");
        product.setLocation(location);
        break;
    }
    case 4: {
        double price = getValidatedDouble("Enter new price: $", 0.0);
        product.setPrice(price);
        break;
    }
    case 5: {
        string name = getValidatedString("Enter new name: ");
        int quantity = getValidatedInt("Enter new quantity: ", 0);
        string location = getValidatedString("Enter new location: ");
        double price = getValidatedDouble("Enter new price: $", 0.0);

        product.setName(name);
        product.setQuantity(quantity);
        product.setLocation(location);
        product.setPrice(price);
        break;
    }
    case 0:
        cout << "Edit cancelled.\n";
        return;
    }

    cout << "\nProduct updated successfully!\n";
    product.display();
}

void WarehouseSystem::addEmployee() {
    cout << "\n=== Add New Employee ===\n";

    string id = getValidatedString("Enter Employee ID: ");
    if (employees.find(id) != employees.end()) {
        cout << "Employee ID already exists!\n";
        return;
    }

    string name = getValidatedString("Enter Employee Name: ");
    string position = getValidatedString("Enter Position: ");

    Employee newEmployee(id, name, position);
    employees[id] = newEmployee;

    // Create user account for employee
    string username = name.substr(0, 3) + id;
    transform(username.begin(), username.end(), username.begin(), ::tolower);

    User newUser(username, "password123", UserRole::EMPLOYEE, id);
    users[username] = newUser;

    cout << "\nEmployee added successfully!\n";
    cout << "Username: " << username << "\n";
    cout << "Default Password: password123\n";
    newEmployee.display();
}

void WarehouseSystem::viewEmployees() {
    cout << "\n=== Employee List ===\n";
    if (employees.empty()) {
        cout << "No employees found.\n";
        return;
    }

    for (const auto& pair : employees) {
        pair.second.display();
    }
}

void WarehouseSystem::editEmployee() {
    cout << "\n=== Edit Employee ===\n";
    string id = getValidatedString("Enter Employee ID to edit: ");

    auto it = employees.find(id);
    if (it == employees.end()) {
        cout << "Employee not found!\n";
        return;
    }

    Employee& employee = it->second;
    employee.display();

    cout << "\nWhat would you like to edit?\n";
    cout << "1. Name\n";
    cout << "2. Position\n";
    cout << "0. Cancel\n";

    int choice = getValidatedInt("Enter choice: ", 0, 2);

    switch (choice) {
    case 1: {
        string name = getValidatedString("Enter new name: ");
        employee.setName(name);
        break;
    }
    case 2: {
        string position = getValidatedString("Enter new position: ");
        employee.setPosition(position);
        break;
    }
    case 0:
        cout << "Edit cancelled.\n";
        return;
    }

    cout << "\nEmployee updated successfully!\n";
    employee.display();
}

void WarehouseSystem::createOrder() {
    cout << "\n=== Create New Order ===\n";

    string orderId = getValidatedString("Enter Order ID: ");
    if (orders.find(orderId) != orders.end()) {
        cout << "Order ID already exists!\n";
        return;
    }

    string customer = getValidatedString("Enter Customer Name: ");
    Order newOrder(orderId, customer);

    char addMore = 'n';
    do {
        cout << "\nAdd item to order:\n";
        string productId = getValidatedString("Enter Product ID: ");

        auto it = products.find(productId);
        if (it == products.end()) {
            cout << "Product not found!\n";
            cout << "Add another item? (y/n): ";
            cin >> addMore;
            cin.ignore();
            continue;
        }

        int maxQty = it->second.getQuantity();
        int quantity = getValidatedInt("Enter quantity (Available: " +
            to_string(maxQty) + "): ", 1, maxQty);

        newOrder.addItem(productId, quantity);

        cout << "Add another item? (y/n): ";
        cin >> addMore;
        cin.ignore();
    } while (addMore == 'y' || addMore == 'Y');

    orders[orderId] = newOrder;

    cout << "\nOrder created successfully!\n";
    newOrder.display(products);
}

void WarehouseSystem::viewOrders() {
    cout << "\n=== Order List ===\n";
    if (orders.empty()) {
        cout << "No orders found.\n";
        return;
    }

    for (const auto& pair : orders) {
        pair.second.display(products);
    }
}

void WarehouseSystem::editOrder() {
    cout << "\n=== Edit Order ===\n";
    string orderId = getValidatedString("Enter Order ID to edit: ");

    auto it = orders.find(orderId);
    if (it == orders.end()) {
        cout << "Order not found!\n";
        return;
    }

    Order& order = it->second;
    order.display(products);

    cout << "\nWhat would you like to edit?\n";
    cout << "1. Add Item\n";
    cout << "2. Remove Item\n";
    cout << "3. Update Item Quantity\n";
    cout << "4. Change Customer Name\n";
    cout << "0. Cancel\n";

    int choice = getValidatedInt("Enter choice: ", 0, 4);

    switch (choice) {
    case 1: {
        string productId = getValidatedString("Enter Product ID to add: ");
        auto prodIt = products.find(productId);
        if (prodIt == products.end()) {
            cout << "Product not found!\n";
            break;
        }

        int maxQty = prodIt->second.getQuantity();
        int quantity = getValidatedInt("Enter quantity (Available: " +
            to_string(maxQty) + "): ", 1, maxQty);
        order.addItem(productId, quantity);
        break;
    }
    case 2: {
        string productId = getValidatedString("Enter Product ID to remove: ");
        order.removeItem(productId);
        break;
    }
    case 3: {
        string productId = getValidatedString("Enter Product ID: ");
        auto prodIt = products.find(productId);
        if (prodIt == products.end()) {
            cout << "Product not found!\n";
            break;
        }

        int maxQty = prodIt->second.getQuantity();
        int quantity = getValidatedInt("Enter new quantity (Available: " +
            to_string(maxQty) + "): ", 0, maxQty);
        order.updateItemQuantity(productId, quantity);
        break;
    }
    case 4: {
        string customer = getValidatedString("Enter new customer name: ");
        order.setCustomerName(customer);
        break;
    }
    case 0:
        cout << "Edit cancelled.\n";
        return;
    }

    cout << "\nOrder updated successfully!\n";
    order.display(products);
}

void WarehouseSystem::assignOrder() {
    cout << "\n=== Assign Order ===\n";
    string orderId = getValidatedString("Enter Order ID: ");

    auto orderIt = orders.find(orderId);
    if (orderIt == orders.end()) {
        cout << "Order not found!\n";
        return;
    }

    if (orderIt->second.getStatus() == OrderStatus::COMPLETED) {
        cout << "Cannot assign completed order!\n";
        return;
    }

    string empId = getValidatedString("Enter Employee ID: ");
    auto empIt = employees.find(empId);
    if (empIt == employees.end()) {
        cout << "Employee not found!\n";
        return;
    }

    Order& order = orderIt->second;
    Employee& employee = empIt->second;

    // Remove from previous employee if assigned
    if (!order.getAssignedEmployeeId().empty()) {
        auto prevEmpIt = employees.find(order.getAssignedEmployeeId());
        if (prevEmpIt != employees.end()) {
            prevEmpIt->second.removeAssignedOrder(orderId);
        }
    }

    order.setStatus(OrderStatus::ASSIGNED);
    order.setAssignedEmployeeId(empId);
    employee.addAssignedOrder(orderId);

    cout << "\nOrder " << orderId << " assigned to " << employee.getName()
        << " (" << empId << ")\n";
}

void WarehouseSystem::generateReports() {
    cout << "\n=== Generate Reports ===\n";
    cout << "1. Inventory Report\n";
    cout << "2. All Orders Report\n";
    cout << "3. Pending Orders Report\n";
    cout << "4. Completed Orders Report\n";
    cout << "0. Back\n";

    int choice = getValidatedInt("Enter choice: ", 0, 4);

    string reportData;
    string filename;
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", &ltm);

    switch (choice) {
    case 1:
        reportData = generateInventoryReport();
        filename = "inventory_report_" + string(timestamp) + ".csv";
        break;
    case 2:
        reportData = generateOrdersReport();
        filename = "orders_report_" + string(timestamp) + ".csv";
        break;
    case 3:
        reportData = generatePendingOrdersReport();
        filename = "pending_orders_report_" + string(timestamp) + ".csv";
        break;
    case 4:
        reportData = generateCompletedOrdersReport();
        filename = "completed_orders_report_" + string(timestamp) + ".csv";
        break;
    case 0:
        return;
    }

    exportToCSV(filename, reportData);
    cout << "\nReport exported to " << filename << "\n";
}

// Employee Operations Implementation
void WarehouseSystem::viewAssignedOrders() {
    if (!currentUser) return;

    string empId = currentUser->getEmployeeId();
    cout << "\n=== Your Assigned Orders ===\n";

    bool hasOrders = false;
    for (const auto& pair : orders) {
        if (pair.second.getAssignedEmployeeId() == empId &&
            pair.second.getStatus() != OrderStatus::COMPLETED) {
            pair.second.display(products);
            hasOrders = true;
        }
    }

    if (!hasOrders) {
        cout << "No assigned orders.\n";
    }
}

void WarehouseSystem::findProduct() {
    cout << "\n=== Find Product ===\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Name\n";
    cout << "0. Back\n";

    int choice = getValidatedInt("Enter choice: ", 0, 2);

    switch (choice) {
    case 1: {
        string id = getValidatedString("Enter Product ID: ");
        Product* product = findProductById(id);
        if (product) {
            cout << "\nProduct found:\n";
            product->display();
        }
        else {
            cout << "Product not found!\n";
        }
        break;
    }
    case 2: {
        string name = getValidatedString("Enter Product Name: ");
        Product* product = findProductByName(name);
        if (product) {
            cout << "\nProduct found:\n";
            product->display();
        }
        else {
            cout << "Product not found!\n";
        }
        break;
    }
    case 0:
        return;
    }
}

void WarehouseSystem::completeOrder() {
    if (!currentUser) return;

    string empId = currentUser->getEmployeeId();
    cout << "\n=== Complete Order ===\n";

    // Show assigned orders
    vector<string> assignedOrderIds;
    for (const auto& pair : orders) {
        if (pair.second.getAssignedEmployeeId() == empId &&
            pair.second.getStatus() == OrderStatus::ASSIGNED) {
            cout << "Order ID: " << pair.first << " - Customer: "
                << pair.second.getCustomerName() << "\n";
            assignedOrderIds.push_back(pair.first);
        }
    }

    if (assignedOrderIds.empty()) {
        cout << "No assigned orders to complete.\n";
        return;
    }

    string orderId = getValidatedString("Enter Order ID to complete: ");
    auto it = orders.find(orderId);
    if (it == orders.end()) {
        cout << "Order not found!\n";
        return;
    }

    Order& order = it->second;
    if (order.getAssignedEmployeeId() != empId) {
        cout << "This order is not assigned to you!\n";
        return;
    }

    if (order.getStatus() == OrderStatus::COMPLETED) {
        cout << "Order already completed!\n";
        return;
    }

    // Update inventory
    bool canComplete = true;
    for (const auto& item : order.getItems()) {
        auto prodIt = products.find(item.first);
        if (prodIt == products.end()) {
            cout << "Product " << item.first << " not found in inventory!\n";
            canComplete = false;
            break;
        }

        if (prodIt->second.getQuantity() < item.second) {
            cout << "Insufficient stock for product " << prodIt->second.getName()
                << " (Available: " << prodIt->second.getQuantity()
                << ", Required: " << item.second << ")\n";
            canComplete = false;
            break;
        }
    }

    if (!canComplete) {
        cout << "Cannot complete order due to insufficient stock.\n";
        return;
    }

    // Deduct from inventory
    for (const auto& item : order.getItems()) {
        auto prodIt = products.find(item.first);
        prodIt->second.setQuantity(prodIt->second.getQuantity() - item.second);
    }

    // Update order status
    order.setStatus(OrderStatus::COMPLETED);

    // Remove from employee's assigned orders
    auto empIt = employees.find(empId);
    if (empIt != employees.end()) {
        empIt->second.removeAssignedOrder(orderId);
    }

    cout << "\nOrder " << orderId << " completed successfully!\n";
    cout << "Inventory updated.\n";
}

void WarehouseSystem::generateEmployeeReport() {
    if (!currentUser) return;

    string empId = currentUser->getEmployeeId();
    cout << "\n=== Generate Your Reports ===\n";
    cout << "1. My Completed Orders\n";
    cout << "2. My Pending Orders\n";
    cout << "0. Back\n";

    int choice = getValidatedInt("Enter choice: ", 0, 2);

    string reportData;
    string filename;
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", &ltm);

    switch (choice) {
    case 1: {
        reportData = "Order ID,Customer,Items,Total,Date,Status\n";
        double totalCompleted = 0.0;

        for (const auto& pair : orders) {
            if (pair.second.getAssignedEmployeeId() == empId &&
                pair.second.getStatus() == OrderStatus::COMPLETED) {
                double orderTotal = pair.second.calculateTotal(products);
                totalCompleted += orderTotal;

                stringstream ss;
                ss << pair.first << ","
                    << pair.second.getCustomerName() << ",";

                string itemsStr;
                for (const auto& item : pair.second.getItems()) {
                    auto prodIt = products.find(item.first);
                    if (prodIt != products.end()) {
                        itemsStr += prodIt->second.getName() + " (" +
                            to_string(item.second) + ");";
                    }
                }
                if (!itemsStr.empty()) itemsStr.pop_back();

                ss << "\"" << itemsStr << "\","
                    << fixed << setprecision(2) << orderTotal << ","
                    << pair.second.getDateCreated() << ","
                    << pair.second.statusToString() << "\n";

                reportData += ss.str();
            }
        }

        reportData += "\nTotal Completed Orders Value: $" +
            to_string(totalCompleted) + "\n";
        filename = "my_completed_orders_" + string(timestamp) + ".csv";
        break;
    }
    case 2: {
        reportData = "Order ID,Customer,Items,Total,Date,Status\n";
        double totalPending = 0.0;

        for (const auto& pair : orders) {
            if (pair.second.getAssignedEmployeeId() == empId &&
                pair.second.getStatus() != OrderStatus::COMPLETED) {
                double orderTotal = pair.second.calculateTotal(products);
                totalPending += orderTotal;

                stringstream ss;
                ss << pair.first << ","
                    << pair.second.getCustomerName() << ",";

                string itemsStr;
                for (const auto& item : pair.second.getItems()) {
                    auto prodIt = products.find(item.first);
                    if (prodIt != products.end()) {
                        itemsStr += prodIt->second.getName() + " (" +
                            to_string(item.second) + ");";
                    }
                }
                if (!itemsStr.empty()) itemsStr.pop_back();

                ss << "\"" << itemsStr << "\","
                    << fixed << setprecision(2) << orderTotal << ","
                    << pair.second.getDateCreated() << ","
                    << pair.second.statusToString() << "\n";

                reportData += ss.str();
            }
        }

        reportData += "\nTotal Pending Orders Value: $" +
            to_string(totalPending) + "\n";
        filename = "my_pending_orders_" + string(timestamp) + ".csv";
        break;
    }
    case 0:
        return;
    }

    exportToCSV(filename, reportData);
    cout << "\nReport exported to " << filename << "\n";
}

// Helper Functions
Product* WarehouseSystem::findProductById(const std::string& id) {
    auto it = products.find(id);
    if (it != products.end()) {
        return &it->second;
    }
    return nullptr;
}

Product* WarehouseSystem::findProductByName(const std::string& name) {
    for (auto& pair : products) {
        if (pair.second.getName().find(name) != std::string::npos) {
            return &pair.second;
        }
    }
    return nullptr;
}

Employee* WarehouseSystem::findEmployeeById(const std::string& id) {
    auto it = employees.find(id);
    if (it != employees.end()) {
        return &it->second;
    }
    return nullptr;
}

Order* WarehouseSystem::findOrderById(const std::string& id) {
    auto it = orders.find(id);
    if (it != orders.end()) {
        return &it->second;
    }
    return nullptr;
}

int WarehouseSystem::getValidatedInt(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        if (!prompt.empty()) {
            cout << prompt;
        }

        if (cin >> value) {
            if (value >= min && value <= max) {
                cin.ignore();
                return value;
            }
            cout << "Please enter a value between " << min << " and " << max << ": ";
        }
        else {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

double WarehouseSystem::getValidatedDouble(const std::string& prompt, double min, double max) {
    double value;
    while (true) {
        if (!prompt.empty()) {
            cout << prompt;
        }

        if (cin >> value) {
            if (value >= min && value <= max) {
                cin.ignore();
                return value;
            }
            cout << "Please enter a value between " << min << " and " << max << ": ";
        }
        else {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

std::string WarehouseSystem::getValidatedString(const std::string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);

    // Remove leading/trailing whitespace
    size_t start = input.find_first_not_of(" \t\n\r");
    size_t end = input.find_last_not_of(" \t\n\r");

    if (start == string::npos) {
        return "";
    }

    return input.substr(start, end - start + 1);
}

// Report Generation Functions
std::string WarehouseSystem::generateInventoryReport() {
    stringstream ss;
    ss << "Product ID,Name,Quantity,Location,Price,Value\n";

    double totalValue = 0.0;
    for (const auto& pair : products) {
        const Product& product = pair.second;
        double value = product.getQuantity() * product.getPrice();
        totalValue += value;

        ss << product.getProductId() << ","
            << product.getName() << ","
            << product.getQuantity() << ","
            << product.getLocation() << ","
            << fixed << setprecision(2) << product.getPrice() << ","
            << fixed << setprecision(2) << value << "\n";
    }

    ss << "\nTotal Inventory Value: $" << fixed << setprecision(2) << totalValue << "\n";
    ss << "Total Products: " << products.size() << "\n";

    return ss.str();
}

std::string WarehouseSystem::generateOrdersReport() {
    stringstream ss;
    ss << "Order ID,Customer,Items,Total,Date,Status,Assigned To\n";

    double totalValue = 0.0;
    int pendingCount = 0, completedCount = 0;

    for (const auto& pair : orders) {
        const Order& order = pair.second;
        double orderTotal = order.calculateTotal(products);
        totalValue += orderTotal;

        if (order.getStatus() == OrderStatus::PENDING ||
            order.getStatus() == OrderStatus::ASSIGNED) {
            pendingCount++;
        }
        else if (order.getStatus() == OrderStatus::COMPLETED) {
            completedCount++;
        }

        ss << order.getOrderId() << ","
            << order.getCustomerName() << ",";

        // Add items
        string itemsStr;
        for (const auto& item : order.getItems()) {
            auto prodIt = products.find(item.first);
            if (prodIt != products.end()) {
                itemsStr += prodIt->second.getName() + " (" +
                    to_string(item.second) + ");";
            }
        }
        if (!itemsStr.empty()) itemsStr.pop_back();

        ss << "\"" << itemsStr << "\","
            << fixed << setprecision(2) << orderTotal << ","
            << order.getDateCreated() << ","
            << order.statusToString() << ","
            << order.getAssignedEmployeeId() << "\n";
    }

    ss << "\nTotal Orders: " << orders.size() << "\n";
    ss << "Pending Orders: " << pendingCount << "\n";
    ss << "Completed Orders: " << completedCount << "\n";
    ss << "Total Orders Value: $" << fixed << setprecision(2) << totalValue << "\n";

    return ss.str();
}

std::string WarehouseSystem::generatePendingOrdersReport() {
    stringstream ss;
    ss << "Order ID,Customer,Items,Total,Date,Assigned To\n";

    double totalValue = 0.0;
    int count = 0;

    for (const auto& pair : orders) {
        const Order& order = pair.second;
        if (order.getStatus() == OrderStatus::PENDING ||
            order.getStatus() == OrderStatus::ASSIGNED) {

            double orderTotal = order.calculateTotal(products);
            totalValue += orderTotal;
            count++;

            ss << order.getOrderId() << ","
                << order.getCustomerName() << ",";

            // Add items
            string itemsStr;
            for (const auto& item : order.getItems()) {
                auto prodIt = products.find(item.first);
                if (prodIt != products.end()) {
                    itemsStr += prodIt->second.getName() + " (" +
                        to_string(item.second) + ");";
                }
            }
            if (!itemsStr.empty()) itemsStr.pop_back();

            ss << "\"" << itemsStr << "\","
                << fixed << setprecision(2) << orderTotal << ","
                << order.getDateCreated() << ","
                << order.getAssignedEmployeeId() << "\n";
        }
    }

    ss << "\nTotal Pending Orders: " << count << "\n";
    ss << "Total Pending Value: $" << fixed << setprecision(2) << totalValue << "\n";

    return ss.str();
}

std::string WarehouseSystem::generateCompletedOrdersReport() {
    stringstream ss;
    ss << "Order ID,Customer,Items,Total,Date,Completed By\n";

    double totalValue = 0.0;
    int count = 0;

    for (const auto& pair : orders) {
        const Order& order = pair.second;
        if (order.getStatus() == OrderStatus::COMPLETED) {

            double orderTotal = order.calculateTotal(products);
            totalValue += orderTotal;
            count++;

            ss << order.getOrderId() << ","
                << order.getCustomerName() << ",";

            // Add items
            string itemsStr;
            for (const auto& item : order.getItems()) {
                auto prodIt = products.find(item.first);
                if (prodIt != products.end()) {
                    itemsStr += prodIt->second.getName() + " (" +
                        to_string(item.second) + ");";
                }
            }
            if (!itemsStr.empty()) itemsStr.pop_back();

            ss << "\"" << itemsStr << "\","
                << fixed << setprecision(2) << orderTotal << ","
                << order.getDateCreated() << ","
                << order.getAssignedEmployeeId() << "\n";
        }
    }

    ss << "\nTotal Completed Orders: " << count << "\n";
    ss << "Total Completed Value: $" << fixed << setprecision(2) << totalValue << "\n";

    return ss.str();
}

void WarehouseSystem::exportToCSV(const std::string& filename, const std::string& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error creating file: " << filename << "\n";
        return;
    }

    file << data;
    file.close();
}