#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <iostream>
#include <string>
#include <fstream>

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
    std::string toString(); 
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
    void loadHistoryFromCSV(std::string filename);
    void saveHistoryToCSV(std::string filename);
    void showHistory();
private:
    std::string robotID;
    std::string log;
    bool taskCompleted;
    movementStack forwardPath;
    movementStack backwardPath;
    movementStack backwardPathCopy;

    struct HistoryEntry {
        std::string robotID;
        std::string forwardSteps;
        std::string returnSteps;
        std::string timestamp;
    };

    HistoryEntry history[100];
    int historyCount;

    std::string getCurrentTime();
};

#endif