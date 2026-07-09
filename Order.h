#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <map>
#include "Product.h"

enum class OrderStatus {
    PENDING,
    ASSIGNED,
    COMPLETED,
    CANCELLED
};

class Order {
private:
    std::string orderId;
    std::string customerName;
    std::map<std::string, int> items;
    OrderStatus status;
    std::string assignedEmployeeId;
    std::string dateCreated;

    std::string getCurrentDate();

public:
    Order();
    Order(std::string id, std::string customer);

    std::string getOrderId() const;
    std::string getCustomerName() const;
    std::map<std::string, int> getItems() const;
    OrderStatus getStatus() const;
    std::string getAssignedEmployeeId() const;
    std::string getDateCreated() const;

    void setOrderId(const std::string& id);
    void setCustomerName(const std::string& customer);
    void setStatus(OrderStatus newStatus);
    void setAssignedEmployeeId(const std::string& empId);
    void setDateCreated(const std::string& date);

    void addItem(const std::string& productId, int quantity);
    void removeItem(const std::string& productId);
    void updateItemQuantity(const std::string& productId, int quantity);

    double calculateTotal(const std::map<std::string, Product>& products) const;

    void display(const std::map<std::string, Product>& products) const;
    std::string statusToString() const;

    std::string toCSV() const;
    static Order fromCSV(const std::string& csvLine);
};

#endif