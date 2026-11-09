#ifndef FACILITIESHASHTABLE_H
#define FACILITIESHASHTABLE_H

#include "facilities.h"
#include <vector>
#include <cctype>

class FacilitiesHashTable{

    private:
        static const int INITIAL_CAPACITY = 11;

        vector<vector<int>> table;

        int entries;
        int collisions;

    public:
        FacilitiesHashTable();

        string normalize(string key);
        int hashFunction(string key);

        bool insert(string key, int facilityIndex, vector<Facilities> facilities);

        int search(string key, vector<Facilities> facilities);

        bool contains(string key, vector<Facilities> facilities);

        int getCapacity();
        int getEntries();
        int getCollisions();

};






#endif