#ifndef FACILITIES_H
#define FACILITIES_H

#include <iostream>
#include <string>
using namespace std;

class Facilities{
    private:
        string id;
        string name;
        string building;
        string category;

    public:
        Facilities(string facilityID, string facilityName, string facilityBuilding, string facilityCategory);

        string getID();
        string getName();
        string getBuilding();
        string getCategory();

        void display();
};






#endif