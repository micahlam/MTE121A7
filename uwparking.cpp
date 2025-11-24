#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int MAXSPOTS = 50;
const int MAXCHANGES = 25;

// for first bool: 1 = staff, 0 = student
// for second: 1 = full, 0 = empty
// arrays for current parking
bool staffOrStudent[MAXSPOTS];
string names[MAXSPOTS];
int parkingSpot[MAXSPOTS];
bool isFull[MAXSPOTS];

// arrays for remove
bool staffOrStudentGone[MAXCHANGES];
string namesGone[MAXCHANGES];

// arrays for add
bool staffOrStudentAdd[MAXCHANGES];
string namesAdd[MAXCHANGES];

// a)
void initializeArrays() {
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
void RemovePeople (string nameOfPerson) 
{
	for (int i=0; i < MAXSPOTS; i++){
		
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
    return -1; // no empty space
}


// f) 
bool PeopleToSpace(string nameOfPerson, bool faculty)
{
    int spot = EmptySpace(faculty); // returns space or -1 if none empty
    
    if (spot != -1) {
        staffOrStudent[spot] = faculty;
        names[spot] = nameOfPerson;
        isFull[spot] = true;
        return true; //person added successfully
    }
    return false; //not able to add person
}


// g)
void MoveStaff() {
    for (int i = 25; i < 50; i++) {
        if (isFull[i] && staffOrStudent[i] == 1) { // staff parked in student zone
            int newIndex = EmptySpace(true);
            if (newIndex != -1) {
                // move them to new staff spot
                names[newIndex] = names[i];
                staffOrStudent[newIndex] = staffOrStudent[i];
                isFull[newIndex] = true;

                // free old space
                names[i] = "";
                staffOrStudent[i] = 0;
                isFull[i] = false;
            }
        }
    }
}


// h)
void OutputToFile(ofstream &outFile, string names[], bool staffOrStudent[], int MAXSPOTS)
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
    // a) Initialize arrays
    initializeArrays();

    // b + c) Open all input/output files
    ifstream currentFile("parking_current.txt");
    ifstream removeFile("parking_remove.txt");
    ifstream addFile("parking_add.txt");
    ofstream outFile("parking_updated.txt");

    if (!currentFile || !removeFile || !addFile || !outFile) {
        cerr << "Error: Could not open one or more files." << endl;
        return 1;
    }

    // Read in data from files
    parkingCurrentData(currentFile);
    parkingRemoveAddData(removeFile, addFile);
	
	
	outFile << "Original Lot: " << endl <<" " << endl;
 	OutputToFile(outFile, names, staffOrStudent, MAXSPOTS); 
 	outFile << "" << endl;
	//Outputs original parking lot before anyone has been removed
	
    // d) Remove people listed in remove file
    for (int i = 0; i < MAXCHANGES; i++) {
        if (namesGone[i] != "") {
            RemovePeople(namesGone[i]);
        }
    }
	
	outFile << "Lot with people removed: " << endl << "" << endl;
	OutputToFile(outFile, names, staffOrStudent, MAXSPOTS); 
	outFile << "" << endl;
	//Outputs parking lot after people have been removed
	
    // f) Add new people listed in add file
    for (int i = 0; i < MAXCHANGES; i++) {
        if (namesAdd[i] != "") {
            bool addSuccess = PeopleToSpace(namesAdd[i], staffOrStudentAdd[i]);
	        
	        if (!addSuccess) {
	        	outFile << "Unable to add: " << namesAdd[i] << ". No space." << endl;	
			}
		}
    }

    // g) Move staff who are in the wrong area
    MoveStaff();

    // h) Output final updated parking data to file
    outFile << "" << endl << "Final updated lot: " << endl << "" << endl;
    OutputToFile(outFile, names, staffOrStudent, MAXSPOTS);

    // Close files
    currentFile.close();
    removeFile.close();
    addFile.close();
    outFile.close();

    cout << "Parking update complete. Results saved to parking_updated.txt" << endl;

    return 0;
}

/*
Spot    Status      Name                
----------------------------------------
1       Staff       Manezes_Alfred      
2       Staff       Tuncel_Levent       
3       Staff       Pritzker_Mark       
4       Staff       McKinnon_David      
5       Student     Rochlitz_Alexandra  
6       Staff       Xie_Wei-Chau        
7       Staff       Hulls_Carol         
8       Staff       Fluid_Newtonian     
9       Student                         
10      Student                         
11      Staff       Davison_Dan         
12      Staff       Bedi_Sanjeev        
13      Student                         
14      Student                         
15      Student                         
16      Student                         
17      Student                         
18      Student                         
19      Student     Malloch_Jeremy      
20      Student                         
21      Staff       McKillop_Bob        
22      Student                         
23      Student                         
24      Student                         
25      Staff       Davidson_George     
26      Student     Lung_Ian            
27      Student     Hmeidan_Amer        
28      Student     Huot_Isabella       
29      Student     Lau_Amanda          
30      Student                         
31      Student     Mills_Joel          
32      Student     Tan_Mark            
33      Student     Roller_Anika        
34      Student     Starratt_Kathryn    
35      Student     Jasmine_Princess    
36      Student     Belisle_Matt        
37      Student     Wang_Willian        
38      Student     Barakat_Abdullah    
39      Student     Samlalsingh_Ryan    
40      Student     Hamza_Muhammad      
41      Student     Zheng_Tim           
42      Student     Yuan_Jacky          
43      Student     Ratiu_Timea         
44      Student     Sheng_Stephen       
45      Student     Kim_Leo             
46      Student                         
47      Student     Yousufzay_Namoos    
48      Student     Clifford_Zachary    
49      Student     Lau_Darren          
50      Student                         
*/