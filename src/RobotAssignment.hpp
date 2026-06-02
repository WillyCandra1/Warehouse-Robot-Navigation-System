#pragma once
#include "Robot.hpp"
using namespace std;

struct RobotNode {
    Robot data;
    RobotNode* next;
};

class RobotAssignment {
private:
    RobotNode* rear;
    int size;
public:
    RobotAssignment();
    void addRobot(Robot robot);
    void assignNext(string task);
    void displayStatus();
    void viewHistory(string robotID);
    void completeTask(string robotID);
    void setMaintenance(string robotID);
    void setAvailable(string robotID);
    bool isEmpty();
    bool robotExists(string robotID);
    ~RobotAssignment();
};    