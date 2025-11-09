#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
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
// c) 
void parkingRemoveAddData (ifstream &fileR, ifstream &fileA) 
//fileR is remove file, fileA is add file
{
	int status = 0;
	string name = "";
	
	int i_remove =0;
	
	while (fileR >> status >> name && i_remove <9) { 
		staffOrStudentGone [i_remove] = status; 
		namesGone[i_remove] = name;	
		i_remove++;
	}
	
	int i_add =0;
	
	while (fileA >> status >> name && i_add <20) {
		staffOrStudentAdd[i_add] = status;
		namesAdd [i_add] = name; 
		i_add++;
	}
}

// d)
void RemovePeople (string nameOfPerson) //send remove names in function in a loop
{
	for (int i=0; i < maxSpots; i++){
		
		if (nameOfPerson == names [i] ) {	
			staffOrStudent [i] = 0;
			names [i] = "";
			isFull [i] = false;
		} 
	}
}

// e)
int EmptySpace(bool faculty)
{
    int start = (faculty) ? 0 : 25;
    int end = (faculty) ? 25 : 50;

    for (int i = start; i < end; i++) {
        if (!isFull[i]) {
            return i;
        }
    }
    return -1;
}

// f) 
void PeopleToSpace (string nameOfPerson, bool faculty)
{
	int spot = EmptySpace(faculty); //returns space or -1 if none empty
	
	if (spot != -1) {
		staffOrStudent[spot] = faculty;
		names[spot] = nameOfPerson;
		isFull[spot] = true;
	}
	else
	{
		bool full =true;
	}
}


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
void OutputToFile (ofstream &outFile, string names[], bool staffOrStudent [], int maxSpots)
{
	outFile << setw(8) << "Spot " << setw(12) << "Who? " 
	<< setw(20) << "Name: " << endl;
	
	for (int i =0; i < maxSpots; i++){
		int spot = i+1;
		
		outFile << setw(8) << spot;
		outFile << setw(12) << staffOrStudent[i] << setw(20) << names[i]<< endl;
	}
}
