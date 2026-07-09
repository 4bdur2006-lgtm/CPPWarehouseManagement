#include "Product.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

Product::Product() : productId(""), name(""), quantity(0), location(""), price(0.0) {}

Product::Product(std::string id, std::string name, int qty, std::string loc, double price)
    : productId(id), name(name), quantity(qty), location(loc), price(price) {
}

std::string Product::getProductId() const { return productId; }
std::string Product::getName() const { return name; }
int Product::getQuantity() const { return quantity; }
std::string Product::getLocation() const { return location; }
double Product::getPrice() const { return price; }

void Product::setProductId(const std::string& id) { productId = id; }
void Product::setName(const std::string& name) { this->name = name; }
void Product::setQuantity(int qty) { quantity = qty; }
void Product::setLocation(const std::string& loc) { location = loc; }
void Product::setPrice(double price) { this->price = price; }

void Product::display() const {
    std::cout << "ID: " << productId << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Quantity: " << quantity << "\n";
    std::cout << "Location: " << location << "\n";
    std::cout << "Price: $" << price << "\n";
    std::cout << "-------------------------\n";
}

std::string Product::toCSV() const {
    return productId + "," + name + "," + std::to_string(quantity) + "," +
        location + "," + std::to_string(price);
}

Product Product::fromCSV(const std::string& csvLine) {
    std::stringstream ss(csvLine);
    std::string token;
    std::vector<std::string> tokens;

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() == 5) {
        return Product(tokens[0], tokens[1], std::stoi(tokens[2]),
            tokens[3], std::stod(tokens[4]));
    }

    return Product();
}