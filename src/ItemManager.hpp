#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// Structure to hold the details of a single item in the warehouse
struct Item {
    std::string itemID;
    std::string itemName;
    std::string location; // Format example: "ZoneA-Aisle3-Shelf2"
};

// Node structure for our Binary Search Tree
struct ItemNode {
    Item data;
    ItemNode* left;
    ItemNode* right;

    // Constructor to easily create a new node
    ItemNode(Item newItem) {
        data = newItem;
        left = nullptr;
        right = nullptr;
    }
};

class ItemManager {
private:
    ItemNode* root;

    // Private helper functions for recursion
    ItemNode* insertRecursive(ItemNode* node, Item newItem);
    ItemNode* searchRecursive(ItemNode* node, std::string id);
    void displayInOrder(ItemNode* node);
    void saveInOrder(ItemNode* node, std::ofstream& outFile); // Helper for saving to CSV
    
    // Phase 4 additions:
    ItemNode* deleteRecursive(ItemNode* node, std::string id);
    ItemNode* findMin(ItemNode* node); // Helper to find the smallest node in a right subtree
    void searchByNameRecursive(ItemNode* node, std::string name, bool& found);

public:
    // Constructor and Destructor
    ItemManager();
    ~ItemManager();

    // Core functionality
    void insertItem(std::string id, std::string name, std::string location);
    Item* searchItemByID(std::string id);
    void displaySortedItems();

    // Phase 4 additions:
    void searchItemByName(std::string name);
    bool updateItemLocation(std::string id, std::string newLocation);
    void deleteItem(std::string id);

    // --- File I/O Integration ---
    void loadItemsFromCSV(std::string filename);
    void saveItemsToCSV(std::string filename);
};

#endif
