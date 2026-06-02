#include "ItemManager.hpp"

using namespace std;

// Constructor initializes an empty tree
ItemManager::ItemManager() {
    root = nullptr;
}

// Destructor
ItemManager::~ItemManager() {
    // TODO: Add a recursive delete function to free memory when the program closes
}

// --- PRIVATE RECURSIVE HELPERS ---

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

// Helper to write the tree back to a CSV file in sorted order
void ItemManager::saveInOrder(ItemNode* node, ofstream& outFile) {
    if (node != nullptr) {
        saveInOrder(node->left, outFile);
        // Write as comma-separated values
        outFile << node->data.itemID << "," 
                << node->data.itemName << "," 
                << node->data.location << "\n";
        saveInOrder(node->right, outFile);
    }
}

// --- PUBLIC METHODS ---

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
    cout << "--- Warehouse Item List (Sorted by ID) ---" << endl;
    displayInOrder(root);
    cout << "------------------------------------------" << endl;
}

// --- FILE I/O METHODS ---

void ItemManager::loadItemsFromCSV(string filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Error: Could not open file " << filename << " for reading." << endl;
        return;
    }

    string line, id, name, location;
    // Read the file line by line
    while (getline(inFile, line)) {
        stringstream ss(line);
        // Split the line by commas
        if (getline(ss, id, ',') && getline(ss, name, ',') && getline(ss, location, ',')) {
            // Insert the item into our Binary Search Tree
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

    // Call the recursive save helper starting from the root
    saveInOrder(root, outFile);
    
    outFile.close();
    cout << "Successfully saved all items to " << filename << "." << endl;
}

// ==========================================
// --- PHASE 4: UPDATE, DELETE & SEARCH BY NAME ---
// ==========================================

bool ItemManager::updateItemLocation(string id, string newLocation) {
    // We can reuse our fast O(log n) search!
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

// Helper to find the node with the minimum value (leftmost node)
ItemNode* ItemManager::findMin(ItemNode* node) {
    while (node && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

ItemNode* ItemManager::deleteRecursive(ItemNode* node, string id) {
    // Base case: item not found
    if (node == nullptr) return node;

    // Traverse down the tree to find the node
    if (id < node->data.itemID) {
        node->left = deleteRecursive(node->left, id);
    } 
    else if (id > node->data.itemID) {
        node->right = deleteRecursive(node->right, id);
    } 
    else {
        // We found the node to delete!
        
        // Case 1: Node has NO children (Leaf node)
        // Case 2: Node has exactly ONE child
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

        // Case 3: Node has TWO children
        // Get the inorder successor (smallest node in the right subtree)
        ItemNode* temp = findMin(node->right);
        
        // Copy the inorder successor's data to this node
        node->data = temp->data;
        
        // Delete the original inorder successor
        node->right = deleteRecursive(node->right, temp->data.itemID);
    }
    return node;
}

void ItemManager::deleteItem(string id) {
    // To ensure we only print "deleted" if it actually existed, check first
    if (searchItemByID(id) == nullptr) {
        cout << "Cannot delete. Item ID " << id << " does not exist." << endl;
        return;
    }
    
    root = deleteRecursive(root, id);
    cout << "Item " << id << " successfully deleted." << endl;
}

// Since the tree is sorted by ID, searching by NAME requires checking every node (O(n))
void ItemManager::searchByNameRecursive(ItemNode* node, string name, bool& found) {
    if (node == nullptr) return;

    // In-order traversal to search
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
    cout << "--- Search Results for '" << name << "' ---" << endl;
    bool found = false;
    searchByNameRecursive(root, name, found);
    
    if (!found) {
        cout << "No items found matching that name." << endl;
    }
    cout << "---------------------------------------" << endl;
}

