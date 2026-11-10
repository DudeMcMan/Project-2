// main.cpp
// Starts the program: loads the four data files in order, prints the load
// summaries, then runs the menu. The real work happens in the module classes.
// Author: Ryker
#include <iostream>
#include <string>

#include "../include/CampusGraph.h"
#include "../include/DepartmentTree.h"
#include "../include/faclilitiesManager.h"
#include "../include/Utils.h"

using namespace std;

// ---------- input helpers (everything is read with getline) ----------

// Gets set if input runs out, so the menu loop can stop instead of spinning.
static bool inputClosed = false;

// Asks a question and returns the trimmed answer. Spaces in names are fine.
static string promptLine(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        inputClosed = true;
        return "";
    }
    return Utils::trim(line);
}

// Reads the menu choice. Prints an error and returns false if it's blank,
// not a number, or way too long.
static bool readChoice(int& choice) {
    string text = promptLine("Enter choice: ");
    if (inputClosed) return false;
    if (text.empty()) {
        cout << "Error: please enter a number from the menu." << endl;
        return false;
    }
    if (text.size() > 6) {
        cout << "Error: '" << text << "' is not a valid menu choice." << endl;
        return false;
    }
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] < '0' || text[i] > '9') {
            cout << "Error: '" << text << "' is not a valid number. Enter a number from the menu." << endl;
            return false;
        }
    }
    choice = stoi(text);
    return true;
}

// ---------- menu and loading ----------

static void printMenu() {
    cout << endl;
    cout << "===== Smart Campus Navigation and Facility Analytics (Milestone 1) =====" << endl;
    cout << "1. View all facilities" << endl;
    cout << "2. Search facility by ID" << endl;
    cout << "3. View campus graph" << endl;
    cout << "4. Display graph counts and degrees" << endl;
    cout << "5. Find reachable buildings (BFS)" << endl;
    cout << "6. Add department" << endl;
    cout << "7. Search department" << endl;
    cout << "8. Display department hierarchy" << endl;
    cout << "9. Run preorder traversal" << endl;
    cout << "0. Exit" << endl;
}

// Puts the folder and file name together without doubling the slash.
static string joinPath(const string& dir, const string& file) {
    if (dir.empty()) return file;
    char last = dir[dir.size() - 1];
    return (last == '/' || last == '\\') ? dir + file : dir + "/" + file;
}

// Summary line for departments.txt, worded like Mason's and Owen's.
static void printSummary(const string& file, int accepted, int rejected) {
    cout << "Load Summary: " << file << " | Accepted: " << accepted
         << " | Rejected: " << rejected << endl;
}

// Option 6. If the tree is empty the first department is the root, so no parent prompt.
static void addDepartmentFromMenu(DepartmentTree& tree) {
    string name = promptLine("Enter new department name: ");
    if (name.empty()) {
        if (!inputClosed) cout << "Error: department name cannot be blank." << endl;
        return;
    }
    string parent;
    if (tree.isEmpty()) {
        cout << "The tree is empty, so '" << name << "' will become the root." << endl;
    } else {
        parent = promptLine("Enter parent department name: ");
        if (parent.empty()) {
            if (!inputClosed) cout << "Error: parent name cannot be blank." << endl;
            return;
        }
    }
    string reason;
    if (tree.addDepartment(name, parent, reason)) {
        cout << "Added department: " << name << endl;
    } else {
        cout << "Error: could not add department: " << reason << endl;
    }
}

int main(int argc, char* argv[]) {
    // Optional folder argument, otherwise use data/.
    string dataDir = (argc > 1) ? argv[1] : "data";

    CampusGraph graph;
    FacilitiesManager facilities;
    DepartmentTree departments;
    int accepted = 0, rejected = 0;

    cout << "Loading data from '" << dataDir << "'..." << endl;

    // Load order matters: buildings, connections, facilities, departments.
    // Mason's and Owen's loaders print their own warnings and summaries.
    graph.loadBuildings(joinPath(dataDir, "buildings.txt"));
    graph.loadConnections(joinPath(dataDir, "connections.txt"));
    facilities.loadFacilities(joinPath(dataDir, "facilities.txt"), graph.getBuildings());

    departments.loadFromFile(joinPath(dataDir, "departments.txt"), accepted, rejected);
    printSummary(joinPath(dataDir, "departments.txt"), accepted, rejected);

    // Keep showing the menu until the user exits.
    int choice = -1;
    while (!inputClosed) {
        printMenu();
        if (!readChoice(choice)) continue;

        switch (choice) {
        case 1:
            facilities.displayFacilities();
            break;
        case 2: {
            string id = promptLine("Enter facility ID: ");
            if (!inputClosed) facilities.searchByID(id);
            break;
        }
        case 3:
            graph.displayGraph();
            break;
        case 4:
            graph.displayStatistics();
            break;
        case 5: {
            string start = promptLine("Enter starting building: ");
            if (inputClosed) break;
            if (start.empty()) cout << "Error: building name cannot be blank." << endl;
            else graph.breadthFirstSearch(start);
            break;
        }
        case 6:
            addDepartmentFromMenu(departments);
            break;
        case 7: {
            string name = promptLine("Enter department name to search: ");
            if (inputClosed) break;
            if (name.empty()) cout << "Error: department name cannot be blank." << endl;
            else departments.displaySearchResult(name);
            break;
        }
        case 8:
            departments.displayHierarchy();
            break;
        case 9:
            departments.displayPreorder();
            break;
        case 0:
            cout << "Goodbye." << endl;
            return 0;
        default:
            cout << "Error: " << choice << " is not on the menu. Enter a number from 0 to 9." << endl;
        }
    }
    return 0;
}
