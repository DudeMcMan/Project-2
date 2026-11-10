#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CampusGraph {
private:
    vector<string> buildings;
    vector<vector<int>> adjacencyList;

    string trim(string value);
    string normalize(string value);
    int findBuilding(string name);
    bool hasConnection(int first, int second);

public:
    void loadBuildings(string fileName);
    void loadConnections(string fileName);

    void displayGraph();
    void displayStatistics();
    void breadthFirstSearch(string startBuilding);

    vector<string> getBuildings();
    int getBuildingCount();
    int getConnectionCount();
};

#endif
