#ifndef FACILITIESMANAGER_H
#define FACILITIESMANAGER_H

#include "facilitiesHashTable.h"

class FacilitiesManager{
    
    private:
        vector<Facilities> facilities;

        FacilitiesHashTable hashTable;

        vector<string> categories;

    public:
        string trim(string value);
        
        string normalize(string value);

        bool validID(string id);

        string findBuilding(string building, vector<string> acceptedBuildings);

        void loadFacilities(string fileName, vector<string> acceptedBuildings);

        void displayFacilities();

        void searchByID(string id);

        void displayHashStatistics();

        int getFacilityCount();

        vector<Facilities> getFacilities();
};







#endif