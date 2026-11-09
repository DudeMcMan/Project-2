#include "faclilitiesManager.h"


int main(){

    FacilitiesManager manager;

    //replace this list with accepted buildings from Buildings Manager
    vector<string> acceptedBuildings = {"Library", "Student Union", "Science Building", "Engineering Building", "Business Building", "Gym"};

    //load facilities
    manager.loadFacilities("data/facilities.txt", acceptedBuildings);

    int choice;
    string searchID;

    do{
        cout << endl << "========== CAMPUS FACILITY SYSTEM ==========" << endl;
        cout << "1. Display All Facilities" << endl;
        cout << "2. Search Facility by ID" << endl;
        cout << "3. Display Hash Table Statistics" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";

        cin >> choice;

        switch (choice) {

            case 1:
                manager.displayFacilities();
                break;

            case 2:
                cout << "Enter Facility ID: ";
                cin >> searchID;

                manager.searchByID(searchID);
                break;

            case 3:
                manager.displayHashStatistics();
                break;

            case 4:
                cout << "Exiting program.\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    }while(choice != 4);

    return 0;
}