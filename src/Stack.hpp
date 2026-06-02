#pragma once
#include <string>
using namespace std;

struct StackNode {
    string task;
    StackNode* next;
};

class Stack {
private:
    StackNode* top;
public:
    Stack();
    void push(string task);
    string pop();
    string peek();
    bool isEmpty();
    void display();
    ~Stack();
};