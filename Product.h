#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
private:
    std::string productId;
    std::string name;
    int quantity;
    std::string location;
    double price;

public:
    Product();
    Product(std::string id, std::string name, int qty, std::string loc, double price);

    std::string getProductId() const;
    std::string getName() const;
    int getQuantity() const;
    std::string getLocation() const;
    double getPrice() const;

    void setProductId(const std::string& id);
    void setName(const std::string& name);
    void setQuantity(int qty);
    void setLocation(const std::string& loc);
    void setPrice(double price);

    void display() const;

    std::string toCSV() const;
    static Product fromCSV(const std::string& csvLine);
};

#endif