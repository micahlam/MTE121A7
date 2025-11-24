#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int MAXSPOTS = 50;
const int MAXCHANGES = 25;

// b)
void parkingCurrentData(ifstream &file,
    bool staffOrStudent[], string names[], int parkingSpot[], bool isFull[])
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
void parkingRemoveAddData(
    ifstream &fileR, ifstream &fileA,
    bool staffOrStudentGone[], string namesGone[],
    bool staffOrStudentAdd[], string namesAdd[])
{
    int status;
    string name;

    int i_remove = 0;
    while (fileR >> status >> name && i_remove < MAXCHANGES) {
        staffOrStudentGone[i_remove] = status;
        namesGone[i_remove] = name;
        i_remove++;
    }

    int i_add = 0;
    while (fileA >> status >> name && i_add < MAXCHANGES) {
        staffOrStudentAdd[i_add] = status;
        namesAdd[i_add] = name;
        i_add++;
    }
}

// d)
void RemovePeople(string nameOfPerson,
    bool staffOrStudent[], string names[], bool isFull[])
{
    for (int i = 0; i < MAXSPOTS; i++) {
        if (names[i] == nameOfPerson) {
            staffOrStudent[i] = false;
            names[i] = "";
            isFull[i] = false;
        }
    }
}

// e)
int EmptySpace(bool faculty, bool isFull[])
{
    int start = faculty ? 0 : 25;
    int end   = faculty ? 25 : 50;

    for (int i = start; i < end; i++) {
        if (!isFull[i])
            return i;
    }
    return -1;
}

// f)
bool PeopleToSpace(
    string nameOfPerson, bool faculty,
    bool staffOrStudent[], string names[], bool isFull[])
{
    int spot = EmptySpace(faculty, isFull);

    if (spot != -1) {
        staffOrStudent[spot] = faculty;
        names[spot] = nameOfPerson;
        isFull[spot] = true;
        return true;
    }
    return false;
}

// g)
void MoveStaff(bool staffOrStudent[], string names[], bool isFull[])
{
    for (int i = 25; i < 50; i++) {
        if (isFull[i] && staffOrStudent[i] == 1) {
            int newIndex = EmptySpace(true, isFull);
            if (newIndex != -1) {
                staffOrStudent[newIndex] = 1;
                names[newIndex] = names[i];
                isFull[newIndex] = true;

                staffOrStudent[i] = false;
                names[i] = "";
                isFull[i] = false;
            }
        }
    }
}

// h)
void OutputToFile(ofstream &outFile,
    string names[], bool staffOrStudent[])
{
    outFile << left << setw(8) << "Spot"
            << setw(12) << "Status"
            << setw(20) << "Name" << endl;
    outFile << string(40, '-') << endl;

    for (int i = 0; i < MAXSPOTS; i++) {
        outFile << left << setw(8) << (i + 1)
                << setw(12) << (staffOrStudent[i] ? "Staff" : "Student")
                << setw(20) << names[i] << endl;
    }
}

int main() {
    bool staffOrStudent[MAXSPOTS];
    string names[MAXSPOTS];
    int parkingSpot[MAXSPOTS];
    bool isFull[MAXSPOTS];

    bool staffOrStudentGone[MAXCHANGES];
    string namesGone[MAXCHANGES];

    bool staffOrStudentAdd[MAXCHANGES];
    string namesAdd[MAXCHANGES];

    for (int i = 0; i < MAXSPOTS; i++) {
        staffOrStudent[i] = false;
        names[i] = "";
        parkingSpot[i] = i + 1;
        isFull[i] = false;
    }

    for (int i = 0; i < MAXCHANGES; i++) {
        staffOrStudentGone[i] = false;
        namesGone[i] = "";
        staffOrStudentAdd[i] = false;
        namesAdd[i] = "";
    }

    ifstream currentFile("parking_current.txt");
    ifstream removeFile("parking_remove.txt");
    ifstream addFile("parking_add.txt");
    ofstream outFile("parking_updated.txt");

    if (!currentFile || !removeFile || !addFile || !outFile) {
        cerr << "Error: Could not open one or more files." << endl;
        return 1;
    }

    parkingCurrentData(currentFile,
        staffOrStudent, names, parkingSpot, isFull);

    parkingRemoveAddData(removeFile, addFile,
        staffOrStudentGone, namesGone,
        staffOrStudentAdd, namesAdd);

    // Output original lot
    outFile << "Original Lot:" << endl << endl;
    OutputToFile(outFile, names, staffOrStudent);
    outFile << endl;

    // Remove people
    for (int i = 0; i < MAXCHANGES; i++) {
        if (namesGone[i] != "")
            RemovePeople(namesGone[i], staffOrStudent, names, isFull);
    }

    outFile << "Lot with people removed:" << endl << endl;
    OutputToFile(outFile, names, staffOrStudent);
    outFile << endl;

    // Add people
    for (int i = 0; i < MAXCHANGES; i++) {
        if (namesAdd[i] != "") {
            bool ok = PeopleToSpace(namesAdd[i], staffOrStudentAdd[i],
                                    staffOrStudent, names, isFull);
            if (!ok) {
                outFile << "Unable to add: " << namesAdd[i]
                        << " — No space available." << endl;
            }
        }
    }

    // Move mis-parked staff
    MoveStaff(staffOrStudent, names, isFull);

    // Final output
    outFile << "Final updated lot:" << endl << endl;
    OutputToFile(outFile, names, staffOrStudent);

    cout << "Parking update complete. Results saved to parking_updated.txt" << endl;

    return 0;
}