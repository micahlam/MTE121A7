#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int maxSpots = 50;
const int maxChanges = 25;

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
    bool staffOrStudent[], string names[], bool isFull[])
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

    for (int i = 0; i < maxSpots; i++) {
        outFile << left << setw(8) << (i + 1)
                << setw(12) << (staffOrStudent[i] ? "Staff" : "Student")
                << setw(20) << names[i] << endl;
    }
}

int main() {
    bool staffOrStudent[maxSpots];
    string names[maxSpots];
    int parkingSpot[maxSpots];
    bool isFull[maxSpots];

    bool staffOrStudentGone[maxChanges];
    string namesGone[maxChanges];

    bool staffOrStudentAdd[maxChanges];
    string namesAdd[maxChanges];

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
    for (int i = 0; i < maxChanges; i++) {
        if (namesGone[i] != "")
            RemovePeople(namesGone[i], staffOrStudent, names, isFull);
    }

    outFile << "Lot with people removed:" << endl << endl;
    OutputToFile(outFile, names, staffOrStudent);
    outFile << endl;

    // Add people
    for (int i = 0; i < maxChanges; i++) {
        if (namesAdd[i] != "") {
            bool ok = PeopleToSpace(namesAdd[i], staffOrStudentAdd[i],
                                    staffOrStudent, names, isFull);
            if (!ok) {
                outFile << "Unable to add: " << namesAdd[i]
                        << " — No space available." << endl;
            }
        }
    }

    // Move staff
    MoveStaff(staffOrStudent, names, isFull);

    // Final output
    outFile << endl << "Final updated lot:" << endl << endl;
    OutputToFile(outFile, names, staffOrStudent);

    cout << "Parking update complete. Results saved to parking_updated.txt" << endl;

    return 0;
}

/*
Original Lot: 
 
Spot    Status      Name                
----------------------------------------
1       Staff       Stubley_Gord        
2       Staff       Yiu_Fiona           
3       Staff       Pritzker_Mark       
4       Student                         
5       Student     Rochlitz_Alexandra  
6       Student                         
7       Student                         
8       Student                         
9       Staff       Consell_Ryan        
10      Student     Paik_Chad           
11      Staff       Davison_Dan         
12      Staff       Bedi_Sanjeev        
13      Student                         
14      Student                         
15      Student                         
16      Student                         
17      Staff       Stachowsky_Michael  
18      Staff       Lau_David           
19      Student     Malloch_Jeremy      
20      Student                         
21      Staff       McKillop_Bob        
22      Student                         
23      Student                         
24      Student                         
25      Staff       Davidson_George     
26      Student     Lung_Ian            
27      Student                         
28      Student     Huot_Isabella       
29      Student     Lau_Amanda          
30      Staff       Xie_Wei-Chau        
31      Student     Mills_Joel          
32      Student                         
33      Student                         
34      Student                         
35      Student     Jasmine_Princess    
36      Student                         
37      Student                         
38      Student     Barakat_Abdullah    
39      Student                         
40      Student     Hamza_Muhammad      
41      Student     Zheng_Tim           
42      Staff       Cronin_Duane        
43      Student                         
44      Student                         
45      Student     Chen_YZ             
46      Staff       Hulls_Carol         
47      Student     Yousufzay_Namoos    
48      Student                         
49      Student     Lau_Darren          
50      Staff       Fluid_Newtonian     

Lot with people removed: 

Spot    Status      Name                
----------------------------------------
1       Student                         
2       Student                         
3       Staff       Pritzker_Mark       
4       Student                         
5       Student     Rochlitz_Alexandra  
6       Student                         
7       Student                         
8       Student                         
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
27      Student                         
28      Student     Huot_Isabella       
29      Student     Lau_Amanda          
30      Staff       Xie_Wei-Chau        
31      Student     Mills_Joel          
32      Student                         
33      Student                         
34      Student                         
35      Student     Jasmine_Princess    
36      Student                         
37      Student                         
38      Student     Barakat_Abdullah    
39      Student                         
40      Student     Hamza_Muhammad      
41      Student     Zheng_Tim           
42      Student                         
43      Student                         
44      Student                         
45      Student                         
46      Staff       Hulls_Carol         
47      Student     Yousufzay_Namoos    
48      Student                         
49      Student     Lau_Darren          
50      Staff       Fluid_Newtonian     

Unable to add: Bhamare_Yash. No space.
Unable to add: Kandathil_Ashar. No space.
Unable to add: Keillor_Peter. No space.
Unable to add: Eltantawy_Mostafa. No space.
Unable to add: Elliott_Jennifer. No space.

Final updated lot: 

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