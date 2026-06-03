#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class WarehouseGraph; // Forward declaration

struct Item {
    std::string itemID;
    std::string itemName;
    std::string location;
};

struct ItemNode {
    Item data;
    ItemNode* left;
    ItemNode* right;

    ItemNode(Item newItem) {
        data = newItem;
        left = nullptr;
        right = nullptr;
    }
};

class ItemManager {
private:
    ItemNode* root;

    ItemNode* insertRecursive(ItemNode* node, Item newItem);
    ItemNode* searchRecursive(ItemNode* node, std::string id);
    void displayInOrder(ItemNode* node);
    void saveInOrder(ItemNode* node, std::ofstream& outFile);
    
    ItemNode* deleteRecursive(ItemNode* node, std::string id);
    ItemNode* findMin(ItemNode* node);
    void searchByNameRecursive(ItemNode* node, std::string name, bool& found);

public:
    ItemManager();
    ~ItemManager();

    void insertItem(std::string id, std::string name, std::string location);
    Item* searchItemByID(std::string id);
    void displaySortedItems();

    void searchItemByName(std::string name);
    bool updateItemLocation(std::string id, std::string newLocation);
    void deleteItem(std::string id);

    void loadItemsFromCSV(std::string filename);
    void saveItemsToCSV(std::string filename);
};

#endif
