#include "facilities.h"

Facilities::Facilities(string facilityID, string facilityName, string facilityBuilding, string facilityCategory){
    id = facilityID;
    name = facilityName;
    building = facilityBuilding;
    category = facilityCategory;
}

string Facilities::getID(){
    return id;
}

string Facilities::getName(){
    return name;
}

string Facilities::getBuilding(){
    return building;
}

string Facilities::getCategory(){
    return category;
}

void Facilities::display(){
    cout << "Facility ID: " << id << endl;
    cout << "Facility Name: " << name << endl;
    cout << "Building: " << building << endl;
    cout << "Category: " << category << endl;
    cout << "-----------------------------" << endl;
}
