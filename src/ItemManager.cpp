#include "ItemManager.hpp"
#include <iomanip>

using namespace std;

// constructor
ItemManager::ItemManager() {
    root = nullptr;
}

// Destructor
ItemManager::~ItemManager() {
    while (root != nullptr) {
        root = deleteRecursive(root, root->data.itemID);
    }
}

// insert a new item into the BST
ItemNode* ItemManager::insertRecursive(ItemNode* node, Item newItem) {
    if (node == nullptr) {
        return new ItemNode(newItem);
    }

    if (newItem.itemID < node->data.itemID) {
        node->left = insertRecursive(node->left, newItem);
    }

    else if (newItem.itemID > node->data.itemID) {
        node->right = insertRecursive(node->right, newItem);
    }

    return node;
}

// searching for an item by ID
ItemNode* ItemManager::searchRecursive(ItemNode* node, string id) {
    if (node == nullptr || node->data.itemID == id) {
        return node;
    }

    if (node->data.itemID < id) {
        return searchRecursive(node->right, id);
    }
    
    return searchRecursive(node->left, id);
}

// search for items by name (not ID)
void ItemManager::displayInOrder(ItemNode* node) {
    if (node != nullptr) {
        displayInOrder(node->left);
        cout << left << setw(10) << node->data.itemID 
             << " | " << setw(20) << node->data.itemName 
             << " | " << node->data.location << endl;
        displayInOrder(node->right);
    }
}

// save items to CSV in sorted order
void ItemManager::saveInOrder(ItemNode* node, ofstream& outFile) {
    if (node != nullptr) {
        saveInOrder(node->left, outFile);
        outFile << node->data.itemID << "," 
                << node->data.itemName << "," 
                << node->data.location << "\n";
        saveInOrder(node->right, outFile);
    }
}

// inserting a new item
void ItemManager::insertItem(string id, string name, string location) {
    Item newItem;
    newItem.itemID = id;
    newItem.itemName = name;
    newItem.location = location;
    root = insertRecursive(root, newItem);
}

// searching for an item by ID and returning a pointer to it
Item* ItemManager::searchItemByID(string id) {
    ItemNode* result = searchRecursive(root, id);
    if (result != nullptr) {
        return &(result->data); 
    }
    return nullptr;
}

// display all items in sorted order by ID
void ItemManager::displaySortedItems() {
    if (root == nullptr) {
        cout << "The item database is currently empty." << endl;
        return;
    }
    cout << "=======================================================" << endl;
    cout << "           Warehouse Item List (Sorted by ID)          " << endl;
    cout << "=======================================================" << endl;
    cout << left << setw(10) << "Item ID" 
         << " | " << setw(20) << "Item Name" 
         << " | " << "Location" << endl;
    cout << "-------------------------------------------------------" << endl;
    displayInOrder(root);
    cout << "=======================================================" << endl;
}

// load items from CSV file into the BST
void ItemManager::loadItemsFromCSV(string filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Error: Could not open file " << filename << " for reading." << endl;
        return;
    }

    string line, id, name, location;
    while (getline(inFile, line)) {
        stringstream ss(line);
        if (getline(ss, id, ',') && getline(ss, name, ',') && getline(ss, location, ',')) {
            insertItem(id, name, location);
        }
    }
    inFile.close();
    cout << "Successfully loaded items from " << filename << " into the tree." << endl;
}

// save items from the BST back to a CSV file
void ItemManager::saveItemsToCSV(string filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    saveInOrder(root, outFile);
    
    outFile.close();
    cout << "Successfully saved all items to " << filename << "." << endl;
}

// update the location of an existing item
bool ItemManager::updateItemLocation(string id, string newLocation) {
    Item* itemToUpdate = searchItemByID(id);
    
    if (itemToUpdate != nullptr) {
        itemToUpdate->location = newLocation;
        cout << "Item " << id << " location updated to " << newLocation << endl;
        return true;
    } else {
        cout << "Error: Cannot update. Item ID " << id << " not found." << endl;
        return false;
    }
}

// find the node with the minimum item ID in a subtree (used for deletion)
ItemNode* ItemManager::findMin(ItemNode* node) {
    while (node && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// delete an item in the tree
ItemNode* ItemManager::deleteRecursive(ItemNode* node, string id) {
    if (node == nullptr) return node;

    if (id < node->data.itemID) {
        node->left = deleteRecursive(node->left, id);
    } 
    else if (id > node->data.itemID) {
        node->right = deleteRecursive(node->right, id);
    } 
    else {
        if (node->left == nullptr) {
            ItemNode* temp = node->right;
            delete node;
            return temp;
        } 
        else if (node->right == nullptr) {
            ItemNode* temp = node->left;
            delete node;
            return temp;
        }

        ItemNode* temp = findMin(node->right);
        node->data = temp->data;
        node->right = deleteRecursive(node->right, temp->data.itemID);
    }
    return node;
}

// delete an item by ID
void ItemManager::deleteItem(string id) {
    if (searchItemByID(id) == nullptr) {
        cout << "Cannot delete. Item ID " << id << " does not exist." << endl;
        return;
    }
    
    root = deleteRecursive(root, id);
    cout << "Item " << id << " successfully deleted." << endl;
}

// search for items by name in tree
void ItemManager::searchByNameRecursive(ItemNode* node, string name, bool& found) {
    if (node == nullptr) return;

    searchByNameRecursive(node->left, name, found);
    
    if (node->data.itemName == name) {
        cout << "Found: ID: " << node->data.itemID 
             << " | Name: " << node->data.itemName 
             << " | Location: " << node->data.location << endl;
        found = true;
    }
    
    searchByNameRecursive(node->right, name, found);
}

// search for items by name and display results
void ItemManager::searchItemByName(string name) {
    cout << "Search Results for '" << name << "':" << endl;
    bool found = false;
    searchByNameRecursive(root, name, found);
    
    if (!found) {
        cout << "No items found matching that name." << endl;
    }
    cout << "=======================================" << endl;
}