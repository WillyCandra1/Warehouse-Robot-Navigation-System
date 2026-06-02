#ifndef WAREHOUSEGRAPH_HPP
#define WAREHOUSEGRAPH_HPP

#include <string>

class navigationSystem;

using namespace std;

struct EdgeNode {
    int target;
    int weight;
    EdgeNode* next;
};

struct Vertex {
    string id;
    string name;
    string type;
    int x;
    int y;
    EdgeNode* edges;
};

class WarehouseGraph {
private:
    Vertex* locations;
    int count;
    int capacity;

    int* lastPath;
    int lastPathLen;
    string* lastMoves;
    int lastMoveCount;
    int lastDistance;

    int findIndex(string id);
    void expand();
    void linkEdge(int from, int to, int weight);
    void buildMoves(int* path, int len);
    void clearRoute();

public:
    WarehouseGraph();
    ~WarehouseGraph();

    bool hasLocation(string id);
    bool addLocation(string id, string name, string type, int x, int y);
    bool connect(string fromId, string toId, int weight);

    void displayLayout();
    void traverseAll(string startId);
    void planRoute(string fromId, string toId);
    void dispatchToRobot(navigationSystem& nav, string robotID);

    void loadFromCSV(string filename);
    void saveToCSV(string filename);

    int size();
};

#endif
