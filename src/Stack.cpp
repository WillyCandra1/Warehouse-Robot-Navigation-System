#include "Stack.hpp"
#include <iostream>
using namespace std;

Stack::Stack() {
    top = nullptr;
};

void Stack::push(string task) {
    StackNode* node = new StackNode;
    node->task = task;
    node->next = top;
    top = node;
}

string Stack::pop() {
    if (isEmpty()) {
        cout << "Stack is empty" << endl;
        return "";
    }
    StackNode* temp = top;
    string task = temp->task;
    top = top->next;
    delete temp;
    return task;
}

string Stack::peek() {
    if (isEmpty()) {
        cout << "Stack is empty" << endl;
        return "";
    }
    return top->task;
}

bool Stack::isEmpty() {
    return top == nullptr;
};

void Stack::display() {
    int num = 1;
    StackNode* current = top;
    while (current != nullptr) {
        cout << num << ". " << current->task << endl;
        current = current->next;
        num++;
    }
}

Stack::~Stack() {
    StackNode* temp = top;
    while (temp != nullptr) {
        StackNode* node = temp->next;
        delete temp;
        temp = node;
    }
}