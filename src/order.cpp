#include <iostream>
#include <string>
#include "order.h"
using namespace std;

OrderManager::OrderManager() {
    front = 0;
    rear = -1;
    pendingCount = 0;
    completedCount = 0;
    nextId = 1;
}

bool OrderManager::isEmpty() {
    return pendingCount == 0;
}

bool OrderManager::isFull() {
    return pendingCount == MAX_ORDERS;
}

void OrderManager::addOrder() {
    if (isFull()) {
        cout << "\nOrder queue is full. System overload.\n";
        return;
    }

    Order newOrder;

    newOrder.id = nextId;
    nextId++;

    cin.ignore();

    cout << "\nEnter customer name: ";
    getline(cin, newOrder.customerName);

    cout << "Enter item name: ";
    getline(cin, newOrder.itemName);

    cout << "Enter quantity: ";
    cin >> newOrder.quantity;

    while (newOrder.quantity <= 0) {
        cout << "Invalid quantity. Enter again: ";
        cin >> newOrder.quantity;
    }

    newOrder.status = "Pending";

    rear = (rear + 1) % MAX_ORDERS;
    pending[rear] = newOrder;
    pendingCount++;

    cout << "\nOrder added successfully.\n";
    cout << "Order ID: " << newOrder.id << endl;
}

void OrderManager::processOrder() {
    if (isEmpty()) {
        cout << "\nNo pending orders to process.\n";
        return;
    }

    if (completedCount == MAX_ORDERS) {
        cout << "\nCompleted order history is full.\n";
        return;
    }

    Order current = pending[front];

    front = (front + 1) % MAX_ORDERS;
    pendingCount--;

    current.status = "Completed";
    completed[completedCount] = current;
    completedCount++;

    cout << "\nProcessing order...\n";
    cout << "Order ID: " << current.id << endl;
    cout << "Customer: " << current.customerName << endl;
    cout << "Item: " << current.itemName << endl;
    cout << "Quantity: " << current.quantity << endl;
    cout << "Status: " << current.status << endl;
}

void OrderManager::showCurrentOrder() {
    if (isEmpty()) {
        cout << "\nNo current order.\n";
        return;
    }

    cout << "\nCurrent order waiting to be processed\n";
    cout << "Order ID: " << pending[front].id << endl;
    cout << "Customer: " << pending[front].customerName << endl;
    cout << "Item: " << pending[front].itemName << endl;
    cout << "Quantity: " << pending[front].quantity << endl;
    cout << "Status: " << pending[front].status << endl;
}

void OrderManager::showPendingOrders() {
    if (isEmpty()) {
        cout << "\nNo pending orders.\n";
        return;
    }

    cout << "\nPending Orders\n";

    int index = front;

    for (int i = 0; i < pendingCount; i++) {
        cout << "\nOrder ID: " << pending[index].id << endl;
        cout << "Customer: " << pending[index].customerName << endl;
        cout << "Item: " << pending[index].itemName << endl;
        cout << "Quantity: " << pending[index].quantity << endl;
        cout << "Status: " << pending[index].status << endl;

        index = (index + 1) % MAX_ORDERS;
    }
}

void OrderManager::showCompletedOrders() {
    if (completedCount == 0) {
        cout << "\nNo completed orders yet.\n";
        return;
    }

    cout << "\nCompleted Orders\n";

    for (int i = 0; i < completedCount; i++) {
        cout << "\nOrder ID: " << completed[i].id << endl;
        cout << "Customer: " << completed[i].customerName << endl;
        cout << "Item: " << completed[i].itemName << endl;
        cout << "Quantity: " << completed[i].quantity << endl;
        cout << "Status: " << completed[i].status << endl;
    }
}

void OrderManager::showSummary() {
    cout << "\nOrder Summary\n";
    cout << "Pending orders: " << pendingCount << endl;
    cout << "Completed orders: " << completedCount << endl;
    cout << "Maximum pending orders: " << MAX_ORDERS << endl;
}