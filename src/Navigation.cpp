#include "Navigation.hpp"
#include <iostream>
using namespace std;

movementStack::movementStack() {
    top = nullptr;
}

movementStack::~movementStack() {
    clear();
}

void movementStack::push(std::string direction) {
    Node* newNode = new Node;
    newNode->direction = direction;
    newNode->next = top;
    top = newNode;
}

std::string movementStack::pop() {
    if (isEmpty()) return "";
    Node* temp = top;
    top = top->next;
    std::string direction = temp->direction;
    delete temp;
    return direction;
}

std::string movementStack::peek() {
    if (isEmpty()) return "";
    return top->direction;
}

bool movementStack::isEmpty() {
    return top == nullptr;
}

int movementStack::size() {
    int count = 0;
    Node* current = top;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void movementStack::display() {
    if (isEmpty()) {
        cout << "  [No steps recorded]\n";
        return;
    }
    Node* nodes[500];
    int count = 0;
    Node* current = top;
    while (current != nullptr) {
        nodes[count++] = current;
        current = current->next;
    }
    for (int i = count - 1; i >= 0; i--) {
        cout << "  Step " << (count - i) << ": " << nodes[i]->direction << "\n";
    }
}

void movementStack::clear() {
    while (!isEmpty()) pop();
}

navigationSystem::navigationSystem() {
    robotID = "";
    log = "";
    taskCompleted = false;
}

navigationSystem::~navigationSystem() {
    forwardPath.clear();
    backwardPath.clear();
    backwardPathCopy.clear();
}

std::string navigationSystem::getRobotID() {
    return robotID;
}

void navigationSystem::robotStart(std::string id) {
    robotID = id;
    taskCompleted = false;
    forwardPath.clear();
    backwardPath.clear();
    backwardPathCopy.clear();
    log = "";
    cout << "\nRobot " << robotID << " started navigation.\n";
}

void navigationSystem::robotMove(std::string direction) {
    forwardPath.push(direction);
    cout << "  [" << robotID << "] Moved: " << direction
         << " (total steps: " << forwardPath.size() << ")\n";
}

void navigationSystem::robotBlocked() {
    if (forwardPath.isEmpty()) {
        cout << "  No steps to undo.\n";
        return;
    }
    std::string undone = forwardPath.pop();
    log += "Obstacle at step " + to_string(forwardPath.size() + 1) + ": backtracked " + undone + "\n";
    cout << "  Obstacle detected! Backtracked: " << undone << "\n";
    cout << "  Enter new direction (FORWARD/BACKWARD/LEFT/RIGHT): ";
    std::string newDir;
    cin >> newDir;
    for (char& c : newDir) c = toupper(c);
    robotMove(newDir);
}

void navigationSystem::robotArrived() {
    taskCompleted = true;
    cout << "\n  Robot " << robotID << " arrived at destination!\n";

    // build return path here so its ready for display and execution
    int count = forwardPath.size();
    std::string* steps = new std::string[count];
    for (int i = 0; i < count; i++) {
        steps[i] = forwardPath.pop();
    }
    for (int i = count - 1; i >= 0; i--) {
        forwardPath.push(steps[i]);
    }

    backwardPath.clear();
    backwardPathCopy.clear();
    for (int i = count - 1; i >= 0; i--) {
        std::string rev;
        if      (steps[i] == "FORWARD")  rev = "BACKWARD";
        else if (steps[i] == "BACKWARD") rev = "FORWARD";
        else if (steps[i] == "LEFT")     rev = "RIGHT";
        else if (steps[i] == "RIGHT")    rev = "LEFT";
        else                             rev = steps[i];
        backwardPath.push(rev);
        backwardPathCopy.push(rev);
    }
    delete[] steps;
    cout << "  Return path ready!\n";
}

void navigationSystem::robotEnd() {
    if (backwardPath.isEmpty()) {
        cout << "  No return path! Make sure robot has arrived first.\n";
        return;
    }
    cout << "\n  --- " << robotID << " returning to base ---\n";
    int step = 1;
    while (!backwardPath.isEmpty()) {
        cout << "  Return step " << step++ << ": " << backwardPath.pop() << "\n";
    }
    cout << "  " << robotID << " safely returned to base!\n";
}

void navigationSystem::showStepsSummary() {
    cout << "\n  Robot ID     : " << robotID << "\n";
    cout << "  Task complete: " << (taskCompleted ? "YES" : "NO") << "\n";
    cout << "  Forward steps: " << forwardPath.size() << "\n";
    cout << "\n  Forward Path\n";
    forwardPath.display();
    cout << "\n  Return Path\n";
    backwardPathCopy.display();
    if (!log.empty()) {
        cout << "\n  Navigation Log\n" << log;
    }
    cout << "  ===============\n";
}