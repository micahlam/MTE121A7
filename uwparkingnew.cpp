#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int maxSpots = 50;
const int maxChanges = 25;


// b)
void parkingCurrentData(ifstream &file,
                        bool staffOrStudent[],
                        string names[],
                        int parkingSpot[],
                        bool isFull[])
{
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
void parkingRemoveAddData(ifstream &fileR,
                          ifstream &fileA,
                          bool staffOrStudentGone[],
                          string namesGone[],
                          bool staffOrStudentAdd[],
                          string namesAdd[])
{
    int status;
    string name;

    int i_remove = 0;
    while (fileR >> status >> name && i_remove < maxChanges) {
        staffOrStudentGone[i_remove] = status;
        namesGone[i_remove] = name;
        i_remove++;
    }

    int i_add = 0;
    while (fileA >> status >> name && i_add < maxChanges) {
        staffOrStudentAdd[i_add] = status;
        namesAdd[i_add] = name;
        i_add++;
    }
}


// d)
void RemovePeople(string nameOfPerson,
                  bool staffOrStudent[],
                  string names[],
                  bool isFull[])
{
    for (int i = 0; i < maxSpots; i++) {
        if (names[i] == nameOfPerson) {
            staffOrStudent[i] = false;
            names[i] = "";
            isFull[i] = false;
        }
    }
}


// e)
int EmptySpace(bool faculty,
               bool isFull[])
{
    int start = (faculty) ? 0 : 25;
    int end   = (faculty) ? 25 : 50;

    for (int i = start; i < end; i++) {
        if (!isFull[i])
            return i;
    }
    return -1;
}


// f)
void PeopleToSpace(string nameOfPerson,
                   bool faculty,
                   bool staffOrStudent[],
                   string names[],
                   bool isFull[])
{
    int spot = EmptySpace(faculty, isFull);

    if (spot != -1) {
        staffOrStudent[spot] = faculty;
        names[spot] = nameOfPerson;
        isFull[spot] = true;
    }
}


// g)
void MoveStaff(bool staffOrStudent[],
               string names[],
               bool isFull[])
{
    for (int i = 25; i < 50; i++) {
        if (isFull[i] && staffOrStudent[i] == 1) { // staff in student zone

            int newIndex = EmptySpace(true, isFull);
            if (newIndex != -1) {

                // move them
                staffOrStudent[newIndex] = staffOrStudent[i];
                names[newIndex] = names[i];
                isFull[newIndex] = true;

                // empty old space
                staffOrStudent[i] = 0;
                names[i] = "";
                isFull[i] = false;
            }
        }
    }
}

// h)
void OutputToFile(ofstream &outFile,
                  string names[],
                  bool staffOrStudent[],
                  int maxSpots)
{
    outFile << left << setw(8) << "Spot"
            << setw(12) << "Status"
            << setw(20) << "Name" << endl;

    outFile << string(40, '-') << endl;

    for (int i = 0; i < maxSpots; i++) {
        outFile << left
                << setw(8)  << (i + 1)
                << setw(12) << (staffOrStudent[i] ? "Staff" : "Student")
                << setw(20) << names[i]
                << endl;
    }
}



int main() {

    // arrays for parking
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

    // initialize everything
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

    // files
    ifstream currentFile("parking_current.txt");
    ifstream removeFile("parking_remove.txt");
    ifstream addFile("parking_add.txt");
    ofstream outFile("parking_updated.txt");

    if (!currentFile || !removeFile || !addFile || !outFile) {
        cerr << "Error: Could not open one or more files." << endl;
        return 1;
    }

    // read data
    parkingCurrentData(currentFile,
                       staffOrStudent, names, parkingSpot, isFull);

    parkingRemoveAddData(removeFile, addFile,
                         staffOrStudentGone, namesGone,
                         staffOrStudentAdd, namesAdd);

    // remove
    for (int i = 0; i < maxChanges; i++) {
        if (namesGone[i] != "") {
            RemovePeople(namesGone[i],
                         staffOrStudent, names, isFull);
        }
    }

    // add
    for (int i = 0; i < maxChanges; i++) {
        if (namesAdd[i] != "") {
            PeopleToSpace(namesAdd[i], staffOrStudentAdd[i],
                          staffOrStudent, names, isFull);
        }
    }

    // move staff out of student area
    MoveStaff(staffOrStudent, names, isFull);

    // output result
    OutputToFile(outFile, names, staffOrStudent, maxSpots);

    cout << "Parking update complete. Results saved to parking_updated.txt\n";

    return 0;
}