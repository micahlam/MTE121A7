#include <iostream>
#include <fstream>
#include <string>
using namespace std;


// NOTE: i only did the functions for now, ill write main once u finish ur functions
// add me on insta micahlam0 to tell me when ur done

const int maxSpots = 50;
const int maxChanges = 25;

// for first bool: 1 = staff, 0 = student
// for second: 1 = full, 0 = empty
// arrays for current parking
bool staffOrStudent[maxSpots];
string names[maxSpots];
int parkingSpot[maxSpots];
bool isFull[maxSpots];

// arrays for remove
bool staffOrStudentGone[maxChanges];
string namesGone[maxChanges];

// arrays for add
bool staffOrStudentAdd[maxChanges];
string namesAdd[maxChanges];

// a)
void initializeArrays() {
    for (int i = 0; i < maxSpots; i++) {
        staffOrStudent[i] = false;
        names[i] = "";
        parkingSpot[i] = i + 1;
        isFull[i] = false;
    }

    for (int i = 0; i < maxChanges; i++) {
        staffOrStudentGone[i] = false;
        namesGone[i] = "";
        staffOrStudentAdd[i] = false;
        namesAdd[i] = "";
    }
}

// b)
void parkingCurrentData(ifstream &file) {
    int status;
    string person;
    int spot;

    while (file >> status >> person >> spot) {
        int index = spot - 1;
        staffOrStudent[index] = status;
        names[index] = person;
        parkingSpot[index] = spot;
        isFull[index] = true;
    }
}
// c) do stuff here




// d) do stuff here




// e)
int EmptySpace(bool faculty) {
    int start = (faculty) ? 0 : 25;
    int end = (faculty) ? 25 : 50;

    for (int i = start; i < end; i++) {
        if (!isFull[i]) {
            return i;
        }
    }
    return -1;
}

// f) do stuff here





// g)
void MoveStaff() {
    for (int i = 25; i < 50; i++) {
        if (isFull[i] && staffOrStudent[i] == 1) {
            int newIndex = EmptySpace(true);
            if (newIndex != -1) {
                // Move to new location
                names[newIndex] = names[i];
                staffOrStudent[newIndex] = staffOrStudent[i];
                isFull[newIndex] = true;

                // Free old space
                names[i] = "";
                staffOrStudent[i] = 0;
                isFull[i] = false;
            }
        }
    }
}

// h)






