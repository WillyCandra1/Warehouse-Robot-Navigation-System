#include "WarehouseGraph.hpp"
#include "Navigation.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
using namespace std;

// checks a string is a whole number, prevent crashing
static bool isInteger(const string& s) {
    if (s.empty()) return false;
    int start = 0;
    if (s[0] == '-' || s[0] == '+') start = 1;
    if (start == (int)s.size()) return false;
    for (int i = start; i < (int)s.size(); i++) {
        if (!isdigit((unsigned char)s[i])) return false;
    }
    return true;
}

WarehouseGraph::WarehouseGraph() {
    capacity = 8;   // start with room for 8 locations and grow later if needed
    count = 0;
    locations = new Vertex[capacity];
    lastPath = nullptr;
    lastPathLen = 0;
    lastMoves = nullptr;
    lastMoveCount = 0;
    lastDistance = 0;
}

WarehouseGraph::~WarehouseGraph() {
    // free every edge in each location's list, then the locations array itself
    for (int i = 0; i < count; i++) {
        EdgeNode* e = locations[i].edges;
        while (e != nullptr) {
            EdgeNode* temp = e;
            e = e->next;
            delete temp;
        }
    }
    delete[] locations;
    clearRoute();
}

void WarehouseGraph::clearRoute() {
    if (lastPath != nullptr) {
        delete[] lastPath;
        lastPath = nullptr;
    }
    if (lastMoves != nullptr) {
        delete[] lastMoves;
        lastMoves = nullptr;
    }
    lastPathLen = 0;
    lastMoveCount = 0;
    lastDistance = 0;
}

int WarehouseGraph::findIndex(string id) {
    for (int i = 0; i < count; i++) {
        if (locations[i].id == id) return i;
    }
    return -1;
}

void WarehouseGraph::expand() { // double the capacity and copy everything across when the array is full
    int newCap = capacity * 2;
    Vertex* bigger = new Vertex[newCap];
    for (int i = 0; i < count; i++) {
        bigger[i] = locations[i];
    }
    delete[] locations;
    locations = bigger;
    capacity = newCap;
}

int WarehouseGraph::size() {
    return count;
}

bool WarehouseGraph::hasLocation(string id) {
    return findIndex(id) != -1;
}

bool WarehouseGraph::hasDock() {
    return findDock() != -1;
}

bool WarehouseGraph::addLocation(string id, string name, string type, int x, int y) {
    if (findIndex(id) != -1) return false;  // no duplicate IDs
    if (type == "DOCK" && findDock() != -1) return false; // only one dock allowed
    if (count == capacity) expand();
    locations[count].id = id;
    locations[count].name = name;
    locations[count].type = type;
    locations[count].x = x;
    locations[count].y = y;
    locations[count].edges = nullptr;
    count++;
    return true;
}

void WarehouseGraph::linkEdge(int from, int to, int weight) {
    // add a new edge node to the front of the list for this location
    EdgeNode* node = new EdgeNode;
    node->target = to;
    node->weight = weight;
    node->next = locations[from].edges;
    locations[from].edges = node;
}

bool WarehouseGraph::connect(string fromId, string toId, int weight) {
    // find the indices of the two locations and check they exist and are not already connected
    int a = findIndex(fromId);
    int b = findIndex(toId);
    if (a == -1 || b == -1) {
        cout << "Cannot connect. One of the locations does not exist." << endl;
        return false;
    }
    if (a == b) {
        cout << "A location cannot be connected to itself." << endl;
        return false;
    }
    for (EdgeNode* e = locations[a].edges; e != nullptr; e = e->next) {
        if (e->target == b) {
            cout << fromId << " and " << toId << " are already connected." << endl;
            return false;
        }
    }
    linkEdge(a, b, weight);
    linkEdge(b, a, weight); // store both directions so the path is two-way
    return true;
}

void WarehouseGraph::displayLayout() {
    if (count == 0) {
        cout << "The warehouse layout is empty. Add some locations first." << endl;
        return;
    }

    cout << "\n=================== Warehouse Layout Map ===================";

    string types[4] = {"DOCK", "ZONE", "AISLE", "SHELF"};
    string titles[4] = {"Packing Dock", "Zones", "Aisles", "Shelves"};

    for (int g = 0; g < 4; g++) {
        bool shown = false;
        for (int i = 0; i < count; i++) {
            if (locations[i].type != types[g]) continue;
            if (!shown) {
                cout << "\n" << titles[g] << ":\n";
                shown = true;
            }

            int degree = 0;
            for (EdgeNode* e = locations[i].edges; e != nullptr; e = e->next) degree++;

            string pos = "(" + to_string(locations[i].x) + "," + to_string(locations[i].y) + ")";

            cout << "  " << left << setw(6) << locations[i].id
                 << setw(16) << locations[i].name
                 << setw(8) << pos;

            if (degree == 0) {
                cout << "no paths yet\n";
                continue;
            }

            int* neighbours = new int[degree];
            int k = 0;
            for (EdgeNode* e = locations[i].edges; e != nullptr; e = e->next) {
                neighbours[k++] = e->target;
            }
            for (int a = 0; a < degree - 1; a++) { //// sort neighbours by ID so the list reads tidily instead of in insertion order
                int pick = a;
                for (int b = a + 1; b < degree; b++) {
                    if (locations[neighbours[b]].id < locations[neighbours[pick]].id) pick = b;
                }
                int tmp = neighbours[a];
                neighbours[a] = neighbours[pick];
                neighbours[pick] = tmp;
            }

            cout << "paths: ";
            for (int a = 0; a < degree; a++) {
                int w = 0;
                for (EdgeNode* e = locations[i].edges; e != nullptr; e = e->next) {
                    if (e->target == neighbours[a]) {
                        w = e->weight;
                        break;
                    }
                }
                cout << locations[neighbours[a]].id << "(" << w << ")";
                if (a < degree - 1) cout << ", ";
            }
            cout << "\n";
            delete[] neighbours;
        }
    }

    cout << "\nTotal locations: " << count << "\n";
    cout << "===========================================================\n";
}

void WarehouseGraph::traverseAll(string startId) { // breadth-first search: visit the start, then everything one step away, then two, and so on
    int start = findIndex(startId);
    if (start == -1) {
        cout << "Location " << startId << " not found." << endl;
        return;
    }

    bool* seen = new bool[count];
    int* queue = new int[count];
    for (int i = 0; i < count; i++) seen[i] = false;

    int front = 0;
    int back = 0;
    queue[back++] = start;
    seen[start] = true;

    cout << "\nVisiting every section reachable from " << locations[start].name << ":\n";
    int step = 1;
    while (front < back) {
        int u = queue[front++];
        cout << "  " << step++ << ". " << left << setw(6) << locations[u].id
             << setw(16) << locations[u].name << "(" << locations[u].type << ")\n";
        for (EdgeNode* e = locations[u].edges; e != nullptr; e = e->next) {
            if (!seen[e->target]) {
                seen[e->target] = true;
                queue[back++] = e->target;
            }
        }
    }

    int left = 0;
    for (int i = 0; i < count; i++) {
        if (!seen[i]) left++;
    }
    if (left > 0) {
        cout << "  Note: " << left << " section(s) could not be reached from here.\n";
    }

    delete[] seen;
    delete[] queue;
}

void WarehouseGraph::buildMoves(int* path, int len) {   // turn the route into robot directions by comparing each step's coordinates
    if (lastMoves != nullptr) {
        delete[] lastMoves;
        lastMoves = nullptr;
    }
    lastMoveCount = 0;
    if (len < 2) return;

    string* steps = new string[(len - 1) * 2];
    int m = 0;
    for (int i = 0; i < len - 1; i++) {
        int a = path[i];
        int b = path[i + 1];
        int dx = locations[b].x - locations[a].x;
        int dy = locations[b].y - locations[a].y;
        if (dy < 0) steps[m++] = "FORWARD";       
        else if (dy > 0) steps[m++] = "BACKWARD";
        if (dx > 0) steps[m++] = "RIGHT";
        else if (dx < 0) steps[m++] = "LEFT";
    }
    lastMoves = steps;
    lastMoveCount = m;
}

int WarehouseGraph::findDock() {
    for (int i = 0; i < count; i++) {
        if (locations[i].type == "DOCK") return i;
    }
    return -1;
}

bool WarehouseGraph::computePath(int src, int dest) {   // Dijkstra's shortest path
    const int INF = 1000000;    
    int* dist = new int[count];         // best known distance from src to each location
    int* prev = new int[count];         //which location we came from
    bool* done = new bool[count];       // whether a location's distance is finalised
    for (int i = 0; i < count; i++) {
        dist[i] = INF;
        prev[i] = -1;
        done[i] = false;
    }
    dist[src] = 0;

    for (int s = 0; s < count; s++) {   // pick the nearest location we haven't finalised yet
        int u = -1;
        int smallest = INF;
        for (int i = 0; i < count; i++) { 
            if (!done[i] && dist[i] < smallest) {
                smallest = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        done[u] = true;
        for (EdgeNode* e = locations[u].edges; e != nullptr; e = e->next) { // update distances to each neighbour of that location
            int v = e->target;
            if (!done[v] && dist[u] + e->weight < dist[v]) {
                dist[v] = dist[u] + e->weight;
                prev[v] = u;
            }
        }
    }

    if (dist[dest] == INF) { // destination was never reached
        delete[] dist;
        delete[] prev;
        delete[] done;
        return false;
    }
    // walk prev[] backwards from dest to rebuild the route in forward order
    int len = 0;
    for (int at = dest; at != -1; at = prev[at]) len++;

    clearRoute();
    lastPath = new int[len];
    lastPathLen = len;
    int k = len - 1;
    for (int at = dest; at != -1; at = prev[at]) {
        lastPath[k--] = at;
    }
    lastDistance = dist[dest];
    buildMoves(lastPath, lastPathLen);

    delete[] dist;
    delete[] prev;
    delete[] done;
    return true;
}

void WarehouseGraph::planRoute(string fromId, string toId) {
    int src = findIndex(fromId);
    int dest = findIndex(toId);
    if (src == -1 || dest == -1) {
        cout << "Route failed. Make sure both locations exist." << endl;
        return;
    }
    if (src == dest) {
        cout << "Start and destination are the same place." << endl;
        return;
    }

    if (!computePath(src, dest)) {
        cout << "There is no route between " << locations[src].name
             << " and " << locations[dest].name << "." << endl;
        return;
    }

    cout << "\nShortest route from " << locations[src].name << " to "
         << locations[dest].name << ":\n  ";
    for (int i = 0; i < lastPathLen; i++) {
        cout << locations[lastPath[i]].id;
        if (i < lastPathLen - 1) cout << " -> ";
    }
    cout << "\n  Total distance: " << lastDistance << "\n";

    cout << "  Robot steps: ";
    if (lastMoveCount == 0) {
        cout << "none";
    } else {
        for (int i = 0; i < lastMoveCount; i++) {
            cout << lastMoves[i];
            if (i < lastMoveCount - 1) cout << ", ";
        }
    }
    cout << "\n";
}
// a robot always leaves from the dock (its base), so the return trip lands back there
void WarehouseGraph::dispatchToRobot(navigationSystem& nav, string robotID, string destId) {
    int base = findDock();
    if (base == -1) {
        cout << "No base is set. Add a location of type Dock to act as the robot base." << endl;
        return;
    }
    int dest = findIndex(destId);
    if (dest == -1) {
        cout << "Destination " << destId << " does not exist." << endl;
        return;
    }
    if (dest == base) {
        cout << "The destination is the base itself, so there is nothing to travel." << endl;
        return;
    }

    if (!computePath(base, dest)) {
        cout << "There is no route from " << locations[base].name
             << " to " << locations[dest].name << "." << endl;
        return;
    }

    cout << "\nDispatching " << robotID << " from " << locations[base].name
         << " to " << locations[dest].name << " (distance " << lastDistance << ").\n";
// hand the route to the Task 3 navigation module to execute step by step
    nav.robotStart(robotID);
    for (int i = 0; i < lastMoveCount; i++) {
        nav.robotMove(lastMoves[i]);
    }
    nav.robotArrived();

    cout << "\n" << robotID << " is now at " << locations[dest].name
         << ". Open the Robot Navigation menu and choose 'Return to base' to send it back to "
         << locations[base].name << ".\n";
}

void WarehouseGraph::loadFromCSV(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open " << filename << ". Starting with an empty layout." << endl;
        return;
    }

    string line;
    int mode = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') {
            if (line.find("LOCATION") != string::npos) mode = 1;
            else if (line.find("CONNECTION") != string::npos) mode = 2;
            continue;
        }

        stringstream ss(line);
        if (mode == 1) {
            string id, name, type, xs, ys;
            if (getline(ss, id, ',') && getline(ss, name, ',') &&
                getline(ss, type, ',') && getline(ss, xs, ',') && getline(ss, ys, ',')) {
                if (isInteger(xs) && isInteger(ys)) {
                    addLocation(id, name, type, stoi(xs), stoi(ys));
                }
            }
        } else if (mode == 2) {
            string a, b, w;
            if (getline(ss, a, ',') && getline(ss, b, ',') && getline(ss, w, ',')) {
                if (isInteger(w) && stoi(w) > 0) {
                    connect(a, b, stoi(w));
                }
            }
        }
    }
    file.close();
    cout << "Loaded warehouse layout from " << filename << "." << endl;
}

void WarehouseGraph::saveToCSV(string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Could not save the layout to " << filename << "." << endl;
        return;
    }

    file << "#LOCATIONS\n";
    for (int i = 0; i < count; i++) {
        file << locations[i].id << "," << locations[i].name << ","
             << locations[i].type << "," << locations[i].x << "," << locations[i].y << "\n";
    }

    file << "#CONNECTIONS\n";
    for (int i = 0; i < count; i++) {
        for (EdgeNode* e = locations[i].edges; e != nullptr; e = e->next) {
            if (i < e->target) {
                file << locations[i].id << "," << locations[e->target].id << ","
                     << e->weight << "\n";
            }
        }
    }
    file.close();
    cout << "Warehouse layout saved to " << filename << "." << endl;
}