#include "ItemManager.hpp"

using namespace std;

ItemManager::ItemManager() {
    root = nullptr;
}

ItemManager::~ItemManager() {
    while (root != nullptr) {
        root = deleteRecursive(root, root->data.itemID);
    }
}

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

ItemNode* ItemManager::searchRecursive(ItemNode* node, string id) {
    if (node == nullptr || node->data.itemID == id) {
        return node;
    }

    if (node->data.itemID < id) {
        return searchRecursive(node->right, id);
    }
    return searchRecursive(node->left, id);
}

void ItemManager::displayInOrder(ItemNode* node) {
    if (node != nullptr) {
        displayInOrder(node->left);
        cout << "ID: " << node->data.itemID 
             << " | Name: " << node->data.itemName 
             << " | Location: " << node->data.location << endl;
        displayInOrder(node->right);
    }
}

void ItemManager::saveInOrder(ItemNode* node, ofstream& outFile) {
    if (node != nullptr) {
        saveInOrder(node->left, outFile);
        outFile << node->data.itemID << "," 
                << node->data.itemName << "," 
                << node->data.location << "\n";
        saveInOrder(node->right, outFile);
    }
}

void ItemManager::insertItem(string id, string name, string location) {
    Item newItem;
    newItem.itemID = id;
    newItem.itemName = name;
    newItem.location = location;
    root = insertRecursive(root, newItem);
}

Item* ItemManager::searchItemByID(string id) {
    ItemNode* result = searchRecursive(root, id);
    if (result != nullptr) {
        return &(result->data); 
    }
    return nullptr;
}

void ItemManager::displaySortedItems() {
    if (root == nullptr) {
        cout << "The item database is currently empty." << endl;
        return;
    }
    cout << "=== Warehouse Item List (Sorted by ID) ===" << endl;
    displayInOrder(root);
    cout << "==========================================" << endl;
}

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

ItemNode* ItemManager::findMin(ItemNode* node) {
    while (node && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

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

void ItemManager::deleteItem(string id) {
    if (searchItemByID(id) == nullptr) {
        cout << "Cannot delete. Item ID " << id << " does not exist." << endl;
        return;
    }
    
    root = deleteRecursive(root, id);
    cout << "Item " << id << " successfully deleted." << endl;
}

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

void ItemManager::searchItemByName(string name) {
    cout << "=== Search Results for '" << name << "' ===" << endl;
    bool found = false;
    searchByNameRecursive(root, name, found);
    
    if (!found) {
        cout << "No items found matching that name." << endl;
    }
    cout << "=======================================" << endl;
}