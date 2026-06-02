#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cctype>
#include "order.h"
#include "Navigation.hpp"
#include "ItemManager.hpp"
#include "RobotAssignment.hpp"
#include "WarehouseGraph.hpp"
using namespace std;

void clearScreen() {
    system("cls");
}

void itemManagementMenu(ItemManager& itemMgr);
void orderMenu(OrderManager& orderManager) {
    int choice;
    do {
        clearScreen();
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

        if (choice > 0) {
            clearScreen();
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

        if (choice > 0) {
            cout << "\nPress Enter to continue...";
            cin.ignore(1000, '\n');
            cin.get();
        }
    } while (choice != 0);
}

void robotAssignmentMenu(RobotAssignment& assignment) {
    int choice;
    do {
        clearScreen();
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

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input. Please enter numbers only.\n";
            choice = -1;
        }

        bool needIgnore = true;

        if (choice > 0) {
            clearScreen();
        }

        switch (choice)
        {
        case 1:
            {
                Robot robot;
                cout << "Enter robot ID: ";
                cin >> robot.robotID;
                if (robot.robotID.empty() || robot.robotID[0] != 'R') {
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
                needIgnore = false;
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
            if (choice != -1) cout << "Invalid choice. Try again." << endl;
            break;
        }

        if (choice > 0) {
            cout << "\nPress Enter to continue...";
            if (needIgnore) cin.ignore(1000, '\n');
            cin.get();
        }
    } while (choice != 0);
}

void navigationMenu(navigationSystem& navigation) {
    int choice;
    string input;

    do {
        clearScreen();
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

        if (choice > 0) {
            clearScreen();
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

        if (choice > 0) {
            cout << "\nPress Enter to continue...";
            cin.ignore(1000, '\n');
            cin.get();
        }
    } while (choice != 0);
}

void itemManagementMenu(ItemManager& itemMgr) {
    int choice = 0;
    string id, name, location;

    do {
        clearScreen();
        cout << "\n====================================\n";
        cout << "        Item Management Menu         \n";
        cout << "\n====================================\n";
        cout << "1. Display All Items (Sorted by ID)\n";
        cout << "2. Search Item by ID\n";
        cout << "3. Search Item by Name\n";
        cout << "4. Add New Item\n";
        cout << "5. Update Item Location\n";
        cout << "0. Back to Main Menu\n";
        cout << "====================================\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (cin.fail()) {
            if (cin.eof()) {
                break;
            }
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter numbers only.\n";
            choice = -1;
        }

        bool needIgnore = true;

        if (choice > 0) {
            clearScreen();
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
                cin >> ws;
                getline(cin, name);
                itemMgr.searchItemByName(name);
                needIgnore = false;
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
                if (choice != -1) {
                    cout << "Invalid choice.\n";
                }
        }

        if (choice > 0) {
            cout << "\nPress Enter to continue...";
            if (needIgnore) cin.ignore(1000, '\n');
            cin.get();
        }
    } while (choice != 0);
}

string readLine(const string& prompt) {
    cout << prompt;
    string line;
    getline(cin, line);
    return line;
}

string readWord(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) return "";
    stringstream ss(line);
    string token;
    ss >> token;
    return token;
}

int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) return 0;
        stringstream ss(line);
        int value;
        char leftover;
        if ((ss >> value) && !(ss >> leftover)) return value;
        cout << "  Please enter a valid whole number.\n";
    }
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    string dummy;
    getline(cin, dummy);
}

bool validLocationId(const string& id) {
    if (id.empty() || id.size() > 10) return false;
    if (!isalpha((unsigned char)id[0])) return false;
    for (char c : id) {
        if (!isalnum((unsigned char)c)) return false;
    }
    return true;
}

void warehouseLayoutMenu(WarehouseGraph& layout, navigationSystem& navigation) {
    string discard;
    getline(cin, discard);

    int choice;
    do {
        clearScreen();
        cout << "\n====================================\n";
        cout << " Warehouse Layout Module\n";
        cout << "====================================\n";
        cout << "1. Display warehouse layout\n";
        cout << "2. Add location\n";
        cout << "3. Connect two locations\n";
        cout << "4. Find route between two locations\n";
        cout << "5. Traverse all sections\n";
        cout << "6. Send route to robot\n";
        cout << "0. Back to main menu\n";
        choice = readInt("Enter choice: ");

        if (choice > 0) {
            clearScreen();
        }

        switch (choice) {
            case 1:
                layout.displayLayout();
                break;

            case 2: {
                string id = readWord("Enter location ID (e.g. A1 or SB2): ");
                for (char& c : id) c = toupper(c);
                if (!validLocationId(id)) {
                    cout << "Invalid ID. Use letters and digits only, starting with a letter (e.g. A1).\n";
                    break;
                }
                if (layout.hasLocation(id)) {
                    cout << "A location with ID " << id << " already exists.\n";
                    break;
                }

                string name = readLine("Enter a name for it (e.g. Shelf A-3): ");
                if (name.empty()) {
                    cout << "The name cannot be empty.\n";
                    break;
                }

                int typeChoice;
                do {
                    cout << "Select the type:\n";
                    cout << "  1. Zone\n";
                    cout << "  2. Aisle\n";
                    cout << "  3. Shelf\n";
                    cout << "  4. Dock\n";
                    typeChoice = readInt("Enter 1-4: ");
                    if (typeChoice < 1 || typeChoice > 4) {
                        cout << "Please choose a number from 1 to 4.\n";
                    }
                } while (typeChoice < 1 || typeChoice > 4);

                string type;
                if (typeChoice == 1) type = "ZONE";
                else if (typeChoice == 2) type = "AISLE";
                else if (typeChoice == 3) type = "SHELF";
                else type = "DOCK";

                cout << "\nGrid position is where the location sits on the warehouse floor map.\n";
                cout << "  X = column (left to right), Y = row (0 is the back wall,\n";
                cout << "  a larger Y is closer to the dock). Example: the dock is at X=0, Y=4.\n";
                int x = readInt("Enter grid X: ");
                int y = readInt("Enter grid Y: ");
                if (x < 0 || y < 0) {
                    cout << "Coordinates cannot be negative. Location not added.\n";
                    break;
                }

                layout.addLocation(id, name, type, x, y);
                cout << "\nLocation " << id << " (" << name << ") added at (" << x << "," << y << ").\n";
                break;
            }

            case 3: {
                string first = readWord("Enter first location ID: ");
                string second = readWord("Enter second location ID: ");
                for (char& c : first) c = toupper(c);
                for (char& c : second) c = toupper(c);
                int w = readInt("Enter the distance between them in steps (e.g. 2): ");
                if (w <= 0) {
                    cout << "Distance must be a positive number.\n";
                    break;
                }
                if (layout.connect(first, second, w)) {
                    cout << "Connected " << first << " and " << second << " (distance " << w << ").\n";
                }
                break;
            }

            case 4: {
                string from = readWord("Enter start location ID: ");
                string to = readWord("Enter destination location ID: ");
                for (char& c : from) c = toupper(c);
                for (char& c : to) c = toupper(c);
                layout.planRoute(from, to);
                break;
            }

            case 5: {
                string from = readWord("Enter location ID to start from: ");
                for (char& c : from) c = toupper(c);
                layout.traverseAll(from);
                break;
            }

            case 6: {
                string robotID = readWord("Enter robot ID to carry out the route: ");
                layout.dispatchToRobot(navigation, robotID);
                break;
            }

            case 0:
                cout << "\nReturning to main menu...\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

        if (choice > 0) {
            waitForEnter();
        }
    } while (choice != 0);
}

int main() {
    OrderManager orderManager;
    navigationSystem navigation;
    ItemManager itemMgr;
    RobotAssignment assignment;
    WarehouseGraph layout;
    int choice;

    clearScreen();
    cout << "Loading Item Database...\n";
    itemMgr.loadItemsFromCSV("../data/items.csv");
    layout.loadFromCSV("../data/layout.csv");

    do {
        clearScreen();
        cout << "\n====================================\n";
        cout << " Warehouse Robot System\n";
        cout << "====================================\n";
        cout << "1. Order Management (Task 1)\n";
        cout << "2. Robot Navigation (Task 3)\n";
        cout << "3. Robot Assignment (Task 2)\n";
        cout << "4. Item Management (Task 4)\n";
        cout << "5. Warehouse Layout (Task 5)\n";
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
            case 1: 
                orderMenu(orderManager); break;
            case 2: 
                navigationMenu(navigation); break;
            case 3: 
                robotAssignmentMenu(assignment); break;
            case 4: 
                itemManagementMenu(itemMgr); break; 
            case 5:
                warehouseLayoutMenu(layout, navigation); break;
            case 0: 
                cout << "\nSaving data and exiting system...\n"; 
                itemMgr.saveItemsToCSV("../data/items.csv");
                layout.saveToCSV("../data/layout.csv");
                break;
            default: cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}