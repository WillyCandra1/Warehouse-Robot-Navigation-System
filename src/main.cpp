#include <iostream>
#include "order.h"
using namespace std;

int main() {
    OrderManager orderManager;
    int choice;

    do {
        cout << "\n====================================\n";
        cout << " Order Management Module\n";
        cout << "====================================\n";
        cout << "1. Add new order\n";
        cout << "2. Process to the next order\n";
        cout << "3. Show current order\n";
        cout << "4. Show pending orders\n";
        cout << "5. Show completed orders\n";
        cout << "6. Show order summary\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input. Please enter numbers only.\n";
            choice = -1;
        }

        switch (choice) {
            case 1:
                orderManager.addOrder();
                break;

            case 2:
                orderManager.processOrder();
                break;

            case 3:
                orderManager.showCurrentOrder();
                break;

            case 4:
                orderManager.showPendingOrders();
                break;

            case 5:
                orderManager.showCompletedOrders();
                break;

            case 6:
                orderManager.showSummary();
                break;

            case 0:
                cout << "\nExiting program...\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}