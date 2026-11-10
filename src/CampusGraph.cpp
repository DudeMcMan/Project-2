#include "CampusGraph.h"

#include <fstream>
#include <sstream>
#include <queue>
#include <cctype>


string CampusGraph::trim(string value) {
    size_t start = value.find_first_not_of(" \t\r\n");

    if (start == string::npos) {
        return "";
    }

    size_t end = value.find_last_not_of(" \t\r\n");

    return value.substr(start, end - start + 1);
}


string CampusGraph::normalize(string value) {
    value = trim(value);

    for (char& c : value) {
        c = toupper(static_cast<unsigned char>(c));
    }

    return value;
}


int CampusGraph::findBuilding(string name) {
    string target = normalize(name);

    for (int i = 0; i < buildings.size(); i++) {
        if (normalize(buildings[i]) == target) {
            return i;
        }
    }

    return -1;
}


bool CampusGraph::hasConnection(int first, int second) {
    for (int neighbor : adjacencyList[first]) {
        if (neighbor == second) {
            return true;
        }
    }

    return false;
}


void CampusGraph::loadBuildings(string fileName) {
    ifstream file(fileName);

    int accepted = 0;
    int rejected = 0;

    if (!file.is_open()) {
        cout << "ERROR: Could not open file: " << fileName << endl;
        cout << "Load Summary: " << fileName
             << " | Accepted: 0 | Rejected: 0" << endl;
        return;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;

        line = trim(line);

        
        if (line.empty() || line[0] == '#') {
            continue;
        }

       
        if (findBuilding(line) != -1) {
            cout << "WARNING: " << fileName
                 << " line " << lineNumber
                 << ": Duplicate building name" << endl;
            rejected++;
            continue;
        }

        buildings.push_back(line);
        adjacencyList.push_back(vector<int>());

        accepted++;
    }

    file.close();

    cout << "Load Summary: " << fileName
         << " | Accepted: " << accepted
         << " | Rejected: " << rejected << endl;
}


void CampusGraph::loadConnections(string fileName) {
    ifstream file(fileName);

    int accepted = 0;
    int rejected = 0;

    if (!file.is_open()) {
        cout << "ERROR: Could not open file: " << fileName << endl;
        cout << "Load Summary: " << fileName
             << " | Accepted: 0 | Rejected: 0" << endl;
        return;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;

        line = trim(line);

        
        if (line.empty() || line[0] == '#') {
            continue;
        }

       
        size_t separator = line.find('|');

        if (separator == string::npos ||
            line.find('|', separator + 1) != string::npos) {

            cout << "WARNING: " << fileName
                 << " line " << lineNumber
                 << ": Incorrect number of fields" << endl;
            rejected++;
            continue;
        }

        string firstName = trim(line.substr(0, separator));
        string secondName = trim(line.substr(separator + 1));

        if (firstName.empty() || secondName.empty()) {
            cout << "WARNING: " << fileName
                 << " line " << lineNumber
                 << ": Empty building name" << endl;
            rejected++;
            continue;
        }

        int first = findBuilding(firstName);
        int second = findBuilding(secondName);

        
        if (first == -1 || second == -1) {
            cout << "WARNING: " << fileName
                 << " line " << lineNumber
                 << ": Building does not exist" << endl;
            rejected++;
            continue;
        }

       
        if (first == second) {
            cout << "WARNING: " << fileName
                 << " line " << lineNumber
                 << ": Self-connections are not allowed" << endl;
            rejected++;
            continue;
        }

       
        if (hasConnection(first, second)) {
            cout << "WARNING: " << fileName
                 << " line " << lineNumber
                 << ": Duplicate connection" << endl;
            rejected++;
            continue;
        }

       
        adjacencyList[first].push_back(second);
        adjacencyList[second].push_back(first);

        accepted++;
    }

    file.close();

    cout << "Load Summary: " << fileName
         << " | Accepted: " << accepted
         << " | Rejected: " << rejected << endl;
}


void CampusGraph::displayGraph() {
    cout << endl << "========== CAMPUS GRAPH ==========" << endl;

    if (buildings.empty()) {
        cout << "No buildings are currently loaded." << endl;
        return;
    }

    for (int i = 0; i < buildings.size(); i++) {
        cout << buildings[i] << ": ";

        if (adjacencyList[i].empty()) {
            cout << "(no connections)";
        }
        else {
            for (int j = 0; j < adjacencyList[i].size(); j++) {
                cout << buildings[adjacencyList[i][j]];

                if (j < adjacencyList[i].size() - 1) {
                    cout << ", ";
                }
            }
        }

        cout << endl;
    }
}


void CampusGraph::displayStatistics() {
    cout << endl << "========== GRAPH STATISTICS ==========" << endl;

    int connections = getConnectionCount();

    cout << "Total Buildings: " << buildings.size() << endl;
    cout << "Total Connections: " << connections << endl;

    cout << endl << "Building Degrees:" << endl;

    for (int i = 0; i < buildings.size(); i++) {
        cout << buildings[i] << ": "
             << adjacencyList[i].size() << endl;
    }
}


void CampusGraph::breadthFirstSearch(string startBuilding) {
    cout << endl << "========== BFS REACHABILITY ==========" << endl;

    int start = findBuilding(startBuilding);

    if (start == -1) {
        cout << "ERROR: Building not found: " << startBuilding << endl;
        return;
    }

    vector<bool> visited(buildings.size(), false);
    vector<int> distance(buildings.size(), -1);

    queue<int> searchQueue;

    
    visited[start] = true;
    distance[start] = 0;
    searchQueue.push(start);

    int reachable = 0;

    cout << "Starting Building: " << buildings[start] << endl;
    cout << "Reachable Buildings:" << endl;

    while (!searchQueue.empty()) {
        int current = searchQueue.front();
        searchQueue.pop();

        cout << buildings[current]
             << " (Distance: " << distance[current] << ")" << endl;

        reachable++;

        
        for (int neighbor : adjacencyList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;
                searchQueue.push(neighbor);
            }
        }
    }

    cout << "Reachable Count: " << reachable
         << " out of " << buildings.size() << endl;
}


vector<string> CampusGraph::getBuildings() {
    return buildings;
}


int CampusGraph::getBuildingCount() {
    return buildings.size();
}


int CampusGraph::getConnectionCount() {
    int total = 0;

    for (int i = 0; i < adjacencyList.size(); i++) {
        total += adjacencyList[i].size();
    }

    return total / 2;
}
