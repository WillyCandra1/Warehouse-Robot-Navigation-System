#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cctype>
#include <iomanip>
#include "order.h"
#include "Navigation.hpp"
#include "ItemManager.hpp"
#include "RobotAssignment.hpp"
#include "WarehouseGraph.hpp"
using namespace std;

void clearScreen() {
    system("cls");
}

// Prototypes for helper functions
string readLine(const string& prompt);
string readWord(const string& prompt);
int readInt(const string& prompt);
void waitForEnter();
bool validItemId(const string& id);
bool validItemLocation(const string& loc, WarehouseGraph& layout);
void itemManagementMenu(ItemManager& itemMgr, WarehouseGraph& layout);

void orderMenu(OrderManager& orderManager) {
    int choice;
    do {
        clearScreen();
        cout << "\n====================================\n";
        cout << "       Order Management Module\n";
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
        cout << "===========================" << endl;
        cout << "  Robot Assignment Module" << endl;
        cout << "===========================" << endl;
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

void navigationMenu(navigationSystem& navigation, RobotAssignment& assignment) {
    int choice;
    string input;

    do {
        clearScreen();
        cout << "\n====================================\n";
        cout << "       Robot Navigation Module\n";
        cout << "====================================\n";
        cout << "1. Start robot\n";
        cout << "2. Move robot\n";
        cout << "3. Robot blocked\n";
        cout << "4. Robot arrived\n";
        cout << "5. Return to base\n";
        cout << "6. Show summary\n";
        cout << "7. Show history\n";
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
            case 1: {
                string availableRobots[50];
                int availCount = 0;
                assignment.getAvailableRobots(availableRobots, availCount, 50);
                if (availCount == 0) {
                    cout << "  No available robots! Add robots in Task 2 first.\n";
                    break;
                }
                cout << "  Available Robots:\n";
                for (int i = 0; i < availCount; i++) {
                    cout << "  " << (i + 1) << ". " << availableRobots[i] << "\n";
                }
                int robotChoice;
                cout << "  Select robot (1-" << availCount << "): ";
                cin >> robotChoice;
                if (robotChoice < 1 || robotChoice > availCount) {
                    cout << "  Invalid selection.\n";
                    break;
                }
                input = availableRobots[robotChoice - 1];
                navigation.robotStart(input);
                break;
            }

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

            case 7:
                navigation.showHistory();
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

void itemManagementMenu(ItemManager& itemMgr, WarehouseGraph& layout) {
    string discard;
    getline(cin, discard);

    int choice = 0;
    string id, name, location;

    do {
        clearScreen();
        cout << "\n====================================\n";
        cout << "        Item Management Menu         \n";
        cout << "====================================\n";
        cout << "1. Display All Items (Sorted by ID)\n";
        cout << "2. Search Item by ID\n";
        cout << "3. Search Item by Name\n";
        cout << "4. Add New Item\n";
        cout << "5. Update Item Location\n";
        cout << "0. Back to Main Menu\n";
        cout << "====================================\n";
        choice = readInt("Enter choice: ");

        if (choice > 0) {
            clearScreen();
        }

        switch (choice) {
            case 1:
                itemMgr.displaySortedItems();
                break;
            case 2: {
                id = readWord("Enter Item ID to search: ");
                Item* found = itemMgr.searchItemByID(id);
                cout << "=======================================================" << endl;
                cout << "             Search Results for '" << id << "'             " << endl;
                cout << "=======================================================" << endl;
                cout << left << setw(10) << "Item ID" 
                     << " | " << setw(20) << "Item Name" 
                     << " | " << "Location" << endl;
                cout << "=======================================================" << endl;
                if (found) {
                    cout << left << setw(10) << found->itemID 
                         << " | " << setw(20) << found->itemName 
                         << " | " << found->location << endl;
                } 
                
                else {
                    cout << "            Item not found in database.                " << endl;
                }
                cout << "=======================================================" << endl;
                break;
            }
            case 3: {
                name = readLine("Enter Item Name to search: ");
                itemMgr.searchItemByName(name);
                break;
            }
            case 4: {
                id = readWord("Enter New Item ID (e.g., ITM016): ");
                if (!validItemId(id)) {
                    cout << "Error: Invalid ID format. Must be 'ITM' followed by 3 digits." << endl;
                    break;
                }
                if (itemMgr.searchItemByID(id) != nullptr) {
                    cout << "Error: Item ID " << id << " already exists in the database." << endl;
                    break;
                }

                name = readLine("Enter Name: ");
                location = readWord("Enter Location (e.g., ZA, A1, SA1): ");
                for (char& c : location) c = toupper(c);

                if (!validItemLocation(location, layout)) {
                    cout << "Error: Location " << location << " was out of the layout!" << endl;
                    break;
                }

                itemMgr.insertItem(id, name, location);
                itemMgr.saveItemsToCSV("../data/item.csv");
                cout << "Item added successfully." << endl;
                break;
            }
            case 5: {
                id = readWord("Enter Item ID to update: ");
                if (itemMgr.searchItemByID(id) == nullptr) {
                    cout << "Error: Item ID " << id << " not found." << endl;
                    break;
                }
                location = readWord("Enter New Location (e.g., ZA, A1, SA1): ");
                for (char& c : location) c = toupper(c);

                if (!validItemLocation(location, layout)) {
                    cout << "Error: Location " << location << " was out of the layout!" << endl;
                    break;
                }

                itemMgr.updateItemLocation(id, location);
                itemMgr.saveItemsToCSV("../data/item.csv");
                break;
            }
            case 0:
                cout << "Returning to Main Menu...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }

        if (choice > 0) {
            waitForEnter();
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
    if (!getline(cin, line)) {
        return "";
    }
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
    if (id.empty() || id.size() > 10) {
        return false;
    }
    if (!isalpha((unsigned char)id[0])) {
        return false;
    }
    for (char c : id) {
        if (!isalnum((unsigned char)c)) {
            return false;
        }
    }
    return true;
}

bool validItemId(const string& id) {
    if (id.length() != 6) {
        return false;
    }
    if (id.substr(0, 3) != "ITM") {
        return false;
    }
    for (int i = 3; i < 6; i++) {
        if (!isdigit(id[i])) {
            return false;
        }
    }
    return true;
}

bool validItemLocation(const string& loc, WarehouseGraph& layout) {
    if (loc.empty()) {
        return false;
    }

    stringstream ss(loc);
    string segment;
    string parts[3];
    int count = 0;
    
    while (getline(ss, segment, '-')) {
        if (count < 3) {
            parts[count] = segment;
        }
        count++;
    }
    
    if (count != 3) {
        cout << "Error: Format must be Zone-Aisle-Shelf (e.g., ZB-B1-SB1)!" << endl;
        return false;
    }

    for (int i = 0; i < 3; i++) {
        if (parts[i].empty() || !layout.hasLocation(parts[i])) {
            cout << "Error: Part '" << parts[i] << "' is not in the layout!" << endl;
            return false;
        }
    }
    
    if (parts[2][0] != 'S') {
        cout << "Error: The destination '" << parts[2] << "' must be a Shelf (starting with 'S')!" << endl;
        return false;
    }

    return true;
}

void warehouseLayoutMenu(WarehouseGraph& layout, navigationSystem& navigation, RobotAssignment& assignment) {
    string discard;
    getline(cin, discard);

    int choice;
    do {
        clearScreen();
        cout << "\n=====================================\n";
        cout << "       Warehouse Layout Module\n";
        cout << "=====================================\n";
        cout << "1. Display warehouse layout\n";
        cout << "2. Add location\n";
        cout << "3. Connect two locations\n";
        cout << "4. Find route between two locations\n";
        cout << "5. Traverse all sections\n";
        cout << "6. Dispatch a robot to a location\n";
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
                string id = readWord("Enter location ID (e.g. A1, SB1, DOCK1, ZA): ");
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
                if (name.find(',') != string::npos) {
                    cout << "The name cannot contain a comma.\n";
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

                if (type == "DOCK" && layout.hasDock()) {
                    cout << "A base dock already exists. Only one dock (the robot base) is allowed.\n";
                    break;
                }

                cout << "\nGrid position is where the location sits on the warehouse floor map.\n";
                cout << "  X = column, Y = row\n";
                int x = readInt("Enter grid X: ");
                int y = readInt("Enter grid Y: ");
                if (x < 0 || y < 0) {
                    cout << "Coordinates cannot be negative. Location not added.\n";
                    break;
                }

                if (layout.addLocation(id, name, type, x, y)) {
                    cout << "\nLocation " << id << " (" << name << ") added at (" << x << "," << y << ").\n";
                } else {
                    cout << "Could not add the location.\n";
                }
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
                if (robotID.empty()) {
                    cout << "Robot ID cannot be empty.\n";
                    break;
                }
                if (!assignment.robotExists(robotID)) {
                    cout << "Robot " << robotID << " does not exist. Add it first in the "
                         << "Robot Assignment module (Task 2).\n";
                    break;
                }
                string destID = readWord("Enter the destination location ID: ");
                for (char& c : destID) c = toupper(c);
                layout.dispatchToRobot(navigation, robotID, destID);
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
    cout << "Loading Databases...\n";
    layout.loadFromCSV("../data/layout.csv");
    itemMgr.loadItemsFromCSV("../data/item.csv");
    assignment.loadRobotsFromCSV("../data/robots.csv");
    orderManager.loadFromCSV("../data/orders.csv");
    navigation.loadHistoryFromCSV("../data/navigation_history.csv");

    do {
        clearScreen();
        cout << "\n====================================\n";
        cout << "       Warehouse Robot System\n";
        cout << "====================================\n";
        cout << "1. Order Management (Task 1)\n";
        cout << "2. Robot Assignment (Task 2)\n";
        cout << "3. Robot Navigation (Task 3)\n";
        cout << "4. Item Management (Task 4)\n";
        cout << "5. Warehouse Layout (Task 5)\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            if (cin.eof()) {
                break;
            }
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input. Please enter numbers only.\n";
            choice = -1;
        }

        switch (choice) {
            case 1: 
                orderMenu(orderManager); break;
            case 2: 
                robotAssignmentMenu(assignment); break;
            case 3: 
                navigationMenu(navigation, assignment); break;
            case 4: 
                itemManagementMenu(itemMgr, layout); break; 
            case 5:
                warehouseLayoutMenu(layout, navigation, assignment); break;
            case 0: 
                cout << "\nSaving data and exiting system...\n"; 
                itemMgr.saveItemsToCSV("../data/item.csv");
                assignment.saveRobotsToCSV("../data/robots.csv");
                layout.saveToCSV("../data/layout.csv");
                orderManager.saveToCSV("../data/orders.csv");
                navigation.saveHistoryToCSV("../data/navigation_history.csv");
                break;
            default: cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}