#include "Order.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>

Order::Order() : orderId(""), customerName(""), status(OrderStatus::PENDING),
assignedEmployeeId(""), dateCreated("") {
}

Order::Order(std::string id, std::string customer)
    : orderId(id), customerName(customer), status(OrderStatus::PENDING),
    assignedEmployeeId(""), dateCreated(getCurrentDate()) {
}

std::string Order::getOrderId() const { return orderId; }
std::string Order::getCustomerName() const { return customerName; }
std::map<std::string, int> Order::getItems() const { return items; }
OrderStatus Order::getStatus() const { return status; }
std::string Order::getAssignedEmployeeId() const { return assignedEmployeeId; }
std::string Order::getDateCreated() const { return dateCreated; }

void Order::setOrderId(const std::string& id) { orderId = id; }
void Order::setCustomerName(const std::string& customer) { customerName = customer; }
void Order::setStatus(OrderStatus newStatus) { status = newStatus; }
void Order::setAssignedEmployeeId(const std::string& empId) { assignedEmployeeId = empId; }
void Order::setDateCreated(const std::string& date) { dateCreated = date; }

void Order::addItem(const std::string& productId, int quantity) {
    items[productId] += quantity;
}

void Order::removeItem(const std::string& productId) {
    items.erase(productId);
}

void Order::updateItemQuantity(const std::string& productId, int quantity) {
    if (items.find(productId) != items.end()) {
        items[productId] = quantity;
    }
}

double Order::calculateTotal(const std::map<std::string, Product>& products) const {
    double total = 0.0;
    for (const auto& item : items) {
        auto it = products.find(item.first);
        if (it != products.end()) {
            total += it->second.getPrice() * item.second;
        }
    }
    return total;
}

void Order::display(const std::map<std::string, Product>& products) const {
    std::cout << "Order ID: " << orderId << "\n";
    std::cout << "Customer: " << customerName << "\n";
    std::cout << "Date Created: " << dateCreated << "\n";
    std::cout << "Status: " << statusToString() << "\n";

    if (!assignedEmployeeId.empty()) {
        std::cout << "Assigned to: " << assignedEmployeeId << "\n";
    }

    std::cout << "Items:\n";
    for (const auto& item : items) {
        auto it = products.find(item.first);
        if (it != products.end()) {
            std::cout << "  - " << it->second.getName() << " (" << item.first
                << "): " << item.second << " units\n";
        }
        else {
            std::cout << "  - Product ID: " << item.first << ": "
                << item.second << " units (Product not found)\n";
        }
    }

    std::cout << "Total: $" << std::fixed << std::setprecision(2)
        << calculateTotal(products) << "\n";
    std::cout << "-------------------------\n";
}

std::string Order::statusToString() const {
    switch (status) {
    case OrderStatus::PENDING: return "Pending";
    case OrderStatus::ASSIGNED: return "Assigned";
    case OrderStatus::COMPLETED: return "Completed";
    case OrderStatus::CANCELLED: return "Cancelled";
    default: return "Unknown";
    }
}

std::string Order::toCSV() const {
    std::string itemsStr;
    for (const auto& item : items) {
        itemsStr += item.first + ":" + std::to_string(item.second) + ";";
    }
    if (!itemsStr.empty()) itemsStr.pop_back();

    return orderId + "," + customerName + "," + itemsStr + "," +
        std::to_string(static_cast<int>(status)) + "," +
        assignedEmployeeId + "," + dateCreated;
}

Order Order::fromCSV(const std::string& csvLine) {
    std::stringstream ss(csvLine);
    std::string token;
    std::vector<std::string> tokens;

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() >= 6) {
        Order order(tokens[0], tokens[1]);

        if (!tokens[2].empty()) {
            std::stringstream itemsStream(tokens[2]);
            std::string itemToken;
            while (getline(itemsStream, itemToken, ';')) {
                size_t colonPos = itemToken.find(':');
                if (colonPos != std::string::npos) {
                    std::string productId = itemToken.substr(0, colonPos);
                    int quantity = std::stoi(itemToken.substr(colonPos + 1));
                    order.addItem(productId, quantity);
                }
            }
        }

        order.setStatus(static_cast<OrderStatus>(std::stoi(tokens[3])));
        order.setAssignedEmployeeId(tokens[4]);
        order.setDateCreated(tokens[5]);

        return order;
    }

    return Order();
}

std::string Order::getCurrentDate() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    std::stringstream ss;
    ss << 1900 + ltm.tm_year << "-"
        << 1 + ltm.tm_mon << "-"
        << ltm.tm_mday;
    return ss.str();
}