#include "WarehouseSystem.h"
#include <iostream>

using namespace std;

int main() {
    WarehouseSystem system;

    cout << "=======================================\n";
    cout << " WAREHOUSE INVENTORY MANAGEMENT SYSTEM\n";
    cout << "=======================================\n";

    while (true) {
        if (!system.login()) {
            cout << "\n1. Try again\n";
            cout << "2. Exit\n";
            cout << "Enter choice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 2) {
                cout << "\nExiting system...\n";
                break;
            }
            continue;
        }

        if (system.getCurrentUserRole() == UserRole::MANAGER) {
            system.managerMenu();
        }
        else {
            system.employeeMenu();
        }

        if (!system.isUserLoggedIn()) {
            cout << "\nReturning to login screen...\n";
        }
    }

    return 0;
}