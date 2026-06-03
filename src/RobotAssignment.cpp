#include "RobotAssignment.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

RobotAssignment::RobotAssignment() {
    rear = nullptr;
    size = 0;
}

void RobotAssignment::addRobot(Robot robot) {
    RobotNode* node = new RobotNode;
    node->data = robot;
    if (rear == nullptr) {
        rear = node;
        rear->next = rear;
    } else {
        node->next = rear->next;
        rear->next = node;
        rear = node;
    }
    size++;
    cout << "Robot " << robot.robotID << " added successfully." << endl;
}

void RobotAssignment::assignNext(string task) {
    if (rear == nullptr) {
        cout << "No robots available!" << endl;
        return;
    }
    RobotNode* current = rear->next;
    do {
        if (current->data.status == "Available") {
            current->data.currentTask = task;
            current->data.status = "Busy";
            current->data.history.push(task);
            rear = current;
            cout << "Assigned task '" << task << "' to robot " << current->data.robotID << endl;
            return;
        }
        current = current->next;
    } while (current != rear->next);
    cout << "No available robots to assign the task" << endl;
}

void RobotAssignment::displayStatus() {
    if (rear == nullptr) {
        cout << "No robots in the system!" << endl;
        return;
    }
    RobotNode* current = rear->next;
    cout << setfill('=') << setw(50) << "" << setfill(' ') << endl;
    cout << left << setw(15) << "Robot ID" << setw(20) << "Current Task" << setw(15) << "Status" << endl;
    cout << string(50, '=') << endl;
    do {
        cout << left << setw(15) << current->data.robotID 
             << setw(20) << (current->data.currentTask.empty() ? "None" : current->data.currentTask) 
             << setw(15) << current->data.status << endl;
        current = current->next;
    } while (current != rear->next);
}

void RobotAssignment::setMaintenance(string robotID) {
    if (rear == nullptr) {
        cout << "No robots in the system!" << endl;
        return;
    }
    RobotNode* current = rear->next;
    do {
        if (current->data.robotID == robotID) {
            if (current->data.status == "Busy") {
                cout << "Cannot set Robot " << robotID << " to Under Maintenance while it is Busy." << endl;
                return;
            }
            current->data.status = "Under Maintenance";
            cout << "Robot " << robotID << " set to Under Maintenance." << endl;
            return;
        }
        current = current->next;
    } while (current != rear->next);
    cout << "Robot with ID " << robotID << " not found!" << endl;
}

void RobotAssignment::setAvailable(string robotID) {
    if (rear == nullptr) {
        cout << "No robots in the system!" << endl;
        return;
    }
    RobotNode* current = rear->next;
    do {
        if (current->data.robotID == robotID) {
            current->data.status = "Available";
            current->data.currentTask = "";
            cout << "Robot " << robotID << " set to Available." << endl;
            return;
        }
        current = current->next;
    } while (current != rear->next);
    cout << "Robot with ID " << robotID << " not found!" << endl;
}

void RobotAssignment::viewHistory(string robotID) {
    if (rear == nullptr) {
        cout << "No robots in the system!" << endl;
        return;
    }
    RobotNode* current = rear->next;
    do {
        if (current->data.robotID == robotID) {
            cout << "Task history for Robot " << robotID << ":" << endl;
            current->data.history.display();
            return;
        }
        current = current->next;
    } while (current != rear->next);
    cout << "Robot with ID " << robotID << " not found!" << endl;
}

void RobotAssignment::completeTask(string robotID) {
    if (rear == nullptr) {
        cout << "No robots in the system!" << endl;
        return;
    }
    RobotNode* current = rear->next;
    do {
        if (current->data.robotID == robotID) {
            if (current->data.status == "Busy") {
                cout << "Task " << current->data.currentTask << " completed by Robot " << robotID << "." << endl;
                current->data.currentTask = "";
                current->data.status = "Available";
                cout << "Robot " << robotID << " has completed its task and is now Available." << endl;
            } 
            else if (current->data.status == "Under Maintenance") {
                cout << "Robot " << robotID << " is currently under maintenance and cannot complete tasks." << endl;
            }
            else {
                cout << "Robot " << robotID << " is not currently busy." << endl;
            }
            return;
        }
        current = current->next;
    } while (current != rear->next);
    cout << "Robot with ID " << robotID << " not found!" << endl;
}

void RobotAssignment::loadRobotsFromCSV(string filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cout << "Error opening file: " << filename << endl;
        return;
    }
    string line;
    getline(inFile, line);

    while (getline(inFile, line)) {
        stringstream ss(line);
        string robotID, currentTask, status;
        if (getline(ss, robotID, ',') && getline(ss, currentTask, ',') && getline(ss, status)) {
            Robot robot;
            robot.robotID = robotID;
            robot.currentTask = currentTask;
            robot.status = status;
            addRobot(robot);
        }
    }
    inFile.close();
}

void RobotAssignment::saveRobotsToCSV(string filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cout << "Error opening file: " << filename << endl;
        return;
    }
    outFile << "RobotID,CurrentTask,Status" << endl;
    if (rear == nullptr) {
        outFile << "No robots found in the system!" << endl;
        outFile.close();
        return;
    }
    RobotNode* current = rear->next;
    do {
        outFile << current->data.robotID << "," << current->data.currentTask << "," << current->data.status << endl;
        current = current->next;
    } while (current != rear->next);
    outFile.close();
    cout << "Robots are saved to " << filename << "." << endl;
}

bool RobotAssignment::isEmpty() {
    return rear == nullptr;
};

bool RobotAssignment::robotExists(string robotID) {
    if (rear == nullptr) {
        return false;
    }
    RobotNode* current = rear->next;
    do {
        if (current->data.robotID == robotID) {
            return true;
        }
        current = current->next;
    } while (current != rear->next);
    return false;
};

RobotAssignment::~RobotAssignment() {
    if (rear == nullptr) return;
    RobotNode* current = rear->next;
    rear->next = nullptr; 
    while (current != nullptr) {
        RobotNode* temp = current;
        current = current->next;
        delete temp;
    }
};