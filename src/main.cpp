#include <iostream>
#include "order.h"
#include "Navigation.hpp"
#include "RobotAssignment.hpp"
using namespace std;

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
                if (robot.robotID[0] != 'R' || robot.robotID.empty()) {
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
    RobotAssignment assignment;
    int choice;

    do {
        cout << "\n====================================\n";
        cout << " Warehouse Robot System\n";
        cout << "====================================\n";
        cout << "1. Order Management\n";
        cout << "2. Robot Assignment\n";
        cout << "3. Robot Navigation\n";
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
            case 1: orderMenu(orderManager); break;
            case 2: robotAssignmentMenu(assignment); break;
            case 3: navigationMenu(navigation); break;
            case 0: cout << "\nExiting system...\n"; break;
            default: cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
