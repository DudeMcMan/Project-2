#include "facilitiesHashTable.h"

FacilitiesHashTable::FacilitiesHashTable(){
    table.resize(INITIAL_CAPACITY);

    entries = 0;
    collisions = 0;
}

//convert to uppercase
string FacilitiesHashTable::normalize(string key){
    for(char& c : key){
        c = toupper(static_cast<unsigned char>(c));
    }

    return key;
}

//adds ASCII values of each character

int FacilitiesHashTable::hashFunction(string key){
    key = normalize(key);

    int hash = 0;

    for(char c : key){
        hash += static_cast<unsigned char>(c);
    }

    return hash % table.size();
}

//insert a facility ID into the hash table
bool FacilitiesHashTable::insert(string key, int facilityIndex, vector<Facilities> facility){
    key = normalize(key);

    int index = hashFunction(key);

    //check for duplicate IDs
    for(int existingIndex : table[index]){
        if(facility[existingIndex].getID() == key){
            return false;
        }
    }

    //count collision only for accepted insertions
    if(!table[index].empty()){
        collisions++;
    }

    table[index].push_back(facilityIndex);

    entries++;

    return true;
}

//search for an ID using the hash table
int FacilitiesHashTable::search(string key, vector<Facilities> facilities){

    key = normalize(key);

    int index = hashFunction(key);

    for(int facilityIndex : table[index]){
        if(facilities[facilityIndex].getID() == key){
            return facilityIndex;
        }
    }

    return -1;
}

bool FacilitiesHashTable::contains(string key, vector<Facilities> facilities){
    return search(key, facilities) != -1;
}

int FacilitiesHashTable::getCapacity(){
    return table.size();
}

int FacilitiesHashTable::getEntries(){
    return entries;
}

int FacilitiesHashTable::getCollisions(){
    return collisions;
}