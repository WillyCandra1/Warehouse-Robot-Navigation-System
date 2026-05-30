#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <iostream>
#include <string>

struct Node {
    std::string direction;
    Node* next;
};

class movementStack {
public:
    movementStack();
    ~movementStack();
    void push(std::string direction);
    std::string pop();
    std::string peek();
    bool isEmpty();
    int size();
    void display();
    void clear();
private:
    Node* top;
};

class navigationSystem {
public:
    navigationSystem();
    ~navigationSystem();
    void robotStart(std::string robotID);
    void robotMove(std::string direction);
    void robotBlocked();
    void robotArrived();
    void robotEnd();
    void showStepsSummary();
    std::string getRobotID();
private:
    std::string robotID;
    std::string log;
    bool taskCompleted;
    movementStack forwardPath;
    movementStack backwardPath;      // used for executing return
    movementStack backwardPathCopy;  // kept for display in summary
};

#endif