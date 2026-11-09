
#include <sstream>
#include <fstream>
#include "faclilitiesManager.h"

string FacilitiesManager::trim(string value){
    size_t start = value.find_first_not_of(" \t\r\n");

    if(start == string::npos){
        return "";
    }

    size_t end = value.find_last_not_of(" \t\r\n");

    return value.substr(start, end - start + 1);
}

string FacilitiesManager::normalize(string value){
    value = trim(value);

    for(char& c : value){
        c = toupper(static_cast<unsigned char>(c));
    }

    return value;
}

//facility IDs must have F followed by 3 digits
bool FacilitiesManager::validID(string id){
    id = normalize(id);

    if(id.length() != 4 || id[0] != 'F'){
        return false;
    }

    for(int i = 1; i < 4; i++){
        if(!isdigit(static_cast<unsigned char>(id[i]))){
            return false;
        }
    }

    return true;
}

//find accept building and return its official spelling
string FacilitiesManager::findBuilding(string building, vector<string> acceptedBuildings){
    string target = normalize(building);
    
    for(string accepted : acceptedBuildings){
        if(normalize(accepted) == target){
            return accepted;
        }
    }

    return "";
}

void FacilitiesManager::loadFacilities(string fileName, vector<string> acceptedBuildings){
    ifstream file(fileName);

    int accepted = 0;
    int rejected = 0;

    if(!file.is_open()){
        cout << "ERROR: Could not open file: " << fileName << endl;

        cout << "Load Summary: " << fileName << " | Accepted: 0 | Rejected: 0" << " | Capacity: " << hashTable.getCapacity() << " | Entries: " << hashTable.getEntries() << " | Collisions: " << hashTable.getCollisions() << endl;

        return;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)){
        lineNumber++;

        line = trim(line);

        //ignor blank lines and comments
        if(line.empty() || line[0] == '#'){
            continue;
        }

        vector<string> fields;
        string field;
        stringstream ss(line);

        while(getline(ss, field, '|')){
            fields.push_back(trim(field));
        }

        //Check for exactly four fields
        if(fields.size() != 4){
            cout << "WARNING: " << fileName << " line " << lineNumber << ": Incorrect number of fields" << endl;

            rejected++;
            continue;
        }

        string id = normalize(fields[0]);
        string name = fields[1];
        string building = fields[2];
        string category = fields[3];

        //validate ID format
        if(!validID(id)){
            cout << "WARNING: " << fileName << " line " << lineNumber << ": Invlaid facility ID" << endl;

            rejected++;
            continue;
        }

        //check for duplicate IDs
        if(hashTable.contains(id, facilities)){
            cout << "WARNING: " << fileName << " line " << lineNumber << ": Duplicate facility ID " << id << endl;

            rejected++;
            continue;
        }

        //validate building reference
        string acceptedBuilding = findBuilding(building, acceptedBuildings);

        if(acceptedBuilding.empty()){
            cout << "WARNING: " << fileName << " line " << lineNumber << ": Building does not exist" << endl;

            rejected++;
            continue;
        }

        //create facility
        Facilities newFacility(id, name, acceptedBuilding, category);

        //preserve accepted file order
        int facilityIndex = facilities.size();
        facilities.push_back(newFacility);

        //insert into has table
        hashTable.insert(id, facilityIndex, facilities);

        //record category spelling only for accepted records
        bool categoryExists = false;

        for(string existing : categories){
            if(normalize(existing) == normalize(category)){
                categoryExists = true;
                break;
            }
        }

        if(!categoryExists){
            categories.push_back(category);
        }

        accepted++;
    }

    file.close();

    //LOAD-4 and HASH-7 su,,ary
    cout << "Load Summary: " << fileName << " | Accepted: " << accepted << " | Rejected: " << rejected << " | Capacity: " << hashTable.getCapacity() << " | Entries: " << hashTable.getEntries() << " | Collisions: " << hashTable.getCollisions() << endl;
}

//display all accepted facilities
void FacilitiesManager::displayFacilities(){
    cout << endl << "========== FACILITIES DATABASE ==========" << endl;

    if(facilities.empty()){
        cout << "No facilities are currently loaded" << endl;
        return;
    }

    for(Facilities facility : facilities){
        facility.display();
    }

    cout << "Total Facilities: " << facilities.size() << endl;
}

//search by facility ID
void FacilitiesManager::searchByID(string id){
    id = normalize(id);

    cout << endl << "========== FACILITY SEARCH ==========" << endl;

    if(!validID(id)){
        cout << "Invalid facility ID format" << endl;
        cout << "Expected format: F followed by 3 digits" << endl;
        return;
    }

    int index = hashTable.search(id, facilities);

    if(index == -1){
        cout << "Facility " << id << " was not found" << endl;
        return;
    }

    cout << "Facility Found" << endl;

    facilities[index].display();
}

void FacilitiesManager::displayHashStatistics(){
    cout << endl << "========== HASH TABLE STATISTICS ==========" << endl;

    cout << "Capacity: " << hashTable.getCapacity() << endl;
    cout << "Entries: " << hashTable.getEntries() << endl;
    cout << "Collisions: " << hashTable.getCollisions() << endl;
}

int FacilitiesManager::getFacilityCount(){
    return facilities.size();
}

vector<Facilities> FacilitiesManager::getFacilities(){
    return facilities;
}