#include <iostream>
#include "order.h"
#include "Navigation.hpp"
#include "ItemManager.hpp"
#include "RobotAssignment.hpp"
using namespace std;

void itemManagementMenu(ItemManager& itemMgr);
void orderMenu(OrderManager& orderManager) {
    int choice;
    do {
        cout << "\n====================================\n";
        cout << " Order Management Module\n";
        cout << "====================================\n";
        cout << "1. Add new order\n";
        cout << "2. Process next order\n";
        cout << "3. Show current order\n";
        cout << "4. Show pending orders\n";
        cout << "5. Show completed orders\n";
        cout << "6. Show order summary\n";
        cout << "0. Back to main menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input. Please enter numbers only.\n";
            choice = -1;
        }

        switch (choice) {
            case 1: orderManager.addOrder();        break;
            case 2: orderManager.processOrder();    break;
            case 3: orderManager.showCurrentOrder(); break;
            case 4: orderManager.showPendingOrders(); break;
            case 5: orderManager.showCompletedOrders(); break;
            case 6: orderManager.showSummary();     break;
            case 0: cout << "\nReturning to main menu...\n"; break;
            default: cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 0);
}

void robotAssignmentMenu(RobotAssignment& assignment) {
    int choice;
    do {
        cout << "==========================" << endl;
        cout << " Robot Assignment Module" << endl;
        cout << "==========================" << endl;
        cout << "1. Add new robot" << endl;
        cout << "2. Assign next task" << endl;
        cout << "3. Display robot status" << endl;
        cout << "4. View robot task history" << endl;
        cout << "5. Mark task as completed" << endl;
        cout << "6. Set robot to maintenance" << endl;
        cout << "7. Set robot to available" << endl;
        cout << "0. Back to main menu" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            {
                Robot robot;
                cout << "Enter robot ID: ";
                cin >> robot.robotID;
                if (robot.robotID.empty() ||robot.robotID[0] != 'R') {
                    cout << "Invalid ID! Robot ID must start with 'R'." << endl;
                    break;
                }
                if (assignment.robotExists(robot.robotID)) {
                    cout << "Robot with ID " << robot.robotID << " already exists! Please use another robot ID!" << endl;
                    break;
                }
                robot.status = "Available";
                assignment.addRobot(robot);
            }
            break;
        case 2:
            {
                string task;
                cout << "Enter new task: ";
                cin.ignore();
                getline(cin, task);
                assignment.assignNext(task);
            }
            break;
        case 3: 
            assignment.displayStatus();
            break;
        case 4: 
            {
                string robotID;
                cout << "Enter robot ID to view history: ";
                cin >> robotID;
                assignment.viewHistory(robotID);
            }
            break;
        case 5:
            {
                string robotID;
                cout << "Enter robot ID to mark task as completed: ";
                cin >> robotID;
                assignment.completeTask(robotID);
            }
            break;
        case 6: 
            {
                string robotID;
                cout << "Enter robot ID to set to maintenance: ";
                cin >> robotID;
                assignment.setMaintenance(robotID);
            }
            break;
        case 7:
            {
                string robotID;
                cout << "Enter robot ID to set to available: ";
                cin >> robotID;
                assignment.setAvailable(robotID);
            }
            break;
        case 0:
            cout << "\nReturning to main menu..." << endl;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
            break;
        }
    } while (choice != 0);

}

void navigationMenu(navigationSystem& navigation) {
    int choice;
    string input;

    do {
        cout << "\n====================================\n";
        cout << " Robot Navigation Module\n";
        cout << "====================================\n";
        cout << "1. Start robot\n";
        cout << "2. Move robot\n";
        cout << "3. Robot blocked\n";
        cout << "4. Robot arrived\n";
        cout << "5. Return to base\n";
        cout << "6. Show summary\n";
        cout << "0. Back to main menu\n";
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
                cout << "Enter robot ID: ";
                cin >> input;
                navigation.robotStart(input);
                break;


            case 2:
                if (navigation.getRobotID().empty()) {
                    cout << "  No robot started! Please start a robot first.\n";
                    break;
                }
                cout << "  Enter directions one by one. Type 'done' to stop.\n";
                while (true) {
                    cout << "  Direction (FORWARD/BACKWARD/LEFT/RIGHT or 'done'): ";
                    cin >> input;
                    for (char& c : input) c = toupper(c);
                    if (input == "DONE") break;
                    if (input != "FORWARD" && input != "BACKWARD" &&
                        input != "LEFT"    && input != "RIGHT") {
                        cout << "  Invalid! Use FORWARD/BACKWARD/LEFT/RIGHT.\n";
                        continue;
                    }
                    navigation.robotMove(input);
                }
                break;

            case 3:
                navigation.robotBlocked();
                break;

            case 4:
                navigation.robotArrived();
                break;

            case 5:
                navigation.robotEnd();
                cout << "\n  Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;

            case 6:
                navigation.showStepsSummary();
                break;

            case 0:
                cout << "\nReturning to main menu...\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 0);
}

int main() {
    OrderManager orderManager;
    navigationSystem navigation;
    ItemManager itemMgr; // Task 4 Initialization
    RobotAssignment assignment;
    int choice;

    // Load data when system starts
    cout << "Loading Item Database...\n";
    itemMgr.loadItemsFromCSV("../data/items.csv"); // Using the main CSV file

    do {
        cout << "\n====================================\n";
        cout << " Warehouse Robot System\n";
        cout << "====================================\n";
        cout << "1. Order Management (Task 1)\n";
        cout << "2. Robot Navigation (Task 3)\n";
        cout << "3. Robot Assignment (Task 2)\n";
        cout << "4. Item Management (Task 4)\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            if (cin.eof()) break; // Prevent EOF infinite loop
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input. Please enter numbers only.\n";
            choice = -1;
        }

        switch (choice) {
            case 1: orderMenu(orderManager); break;
            case 2: navigationMenu(navigation); break;
            case 3: robotAssignmentMenu(assignment); break;
            case 4: itemManagementMenu(itemMgr); break; // Calling Task 4 Menu
            case 0: 
                cout << "\nSaving data and exiting system...\n"; 
                itemMgr.saveItemsToCSV("../data/items.csv");
                break;
            default: cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}

// Sub-menu specifically for Task 4
void itemManagementMenu(ItemManager& itemMgr) {
    int choice = 0;
    string id, name, location;

    do {
        cout << "\n--- Item Management Menu ---\n";
        cout << "1. Display All Items (Sorted by ID)\n";
        cout << "2. Search Item by ID\n";
        cout << "3. Search Item by Name\n";
        cout << "4. Add New Item\n";
        cout << "5. Update Item Location\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter choice: ";
        
        cin >> choice;
        
        if (cin.fail()) {
            if (cin.eof()) break;
            cin.clear(); cin.ignore(10000, '\n'); choice = -1;
        }

        switch (choice) {
            case 1:
                itemMgr.displaySortedItems();
                break;
            case 2:
                cout << "Enter Item ID to search: ";
                cin >> id;
                {
                    Item* found = itemMgr.searchItemByID(id);
                    if (found) {
                        cout << "Found: " << found->itemName << " at " << found->location << endl;
                    } else {
                        cout << "Item not found." << endl;
                    }
                }
                break;
            case 3:
                cout << "Enter Item Name to search: ";
                cin >> ws; // clear input buffer
                getline(cin, name);
                itemMgr.searchItemByName(name);
                break;
            case 4:
                cout << "Enter New Item ID: "; cin >> id;
                cout << "Enter Name: "; cin >> ws; getline(cin, name);
                cout << "Enter Location: "; cin >> location;
                itemMgr.insertItem(id, name, location);
                cout << "Item added." << endl;
                break;
            case 5:
                cout << "Enter Item ID to update: "; cin >> id;
                cout << "Enter New Location: "; cin >> location;
                itemMgr.updateItemLocation(id, location);
                break;
            case 0:
                cout << "Returning to Main Menu...\n";
                break;
            default:
                if (choice != -1) cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}
