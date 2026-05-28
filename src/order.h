#ifndef ORDER_H
#define ORDER_H

#include <string>
using namespace std;

const int MAX_ORDERS = 10;

struct Order {
    int id;
    string customerName;
    string itemName;
    int quantity;
    string status;
};

class OrderManager {
private:
    Order pending[MAX_ORDERS];
    Order completed[MAX_ORDERS];

    int front;
    int rear;
    int pendingCount;
    int completedCount;
    int nextId;

public:
    OrderManager();

    bool isEmpty();
    bool isFull();

    void addOrder();
    void processOrder();
    void showCurrentOrder();
    void showPendingOrders();
    void showCompletedOrders();
    void showSummary();
};

#endif