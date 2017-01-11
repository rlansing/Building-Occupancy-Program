///***************************************************************************
//  FILENAME:	 LansingAssn4.CPP
//  DESCRIPTION: This program is used to keep track of occupied or unoccupied
//               offices in a building. Starting from basement (floor 0).
//               Program reads from and writes to a binary file.
//  DATE:        November 20, 2014
//  DESIGNER:	 Robert Lansing
//  FUNCTIONS:	 readFile - Reads in data file into 2D array. 
//                         File is in binary format, and only contains data
//                         about offices that are occupied.
//                         Data is stored in the order 2D array row, column,
//                         value. 
//              displayOfficeTotals - displays the totals for occupied offices
//              menuPrompt - displays menu, prompts and stores selection
//              displayOfficeStatus - displays unoccupied or occupied offices
//              officeLetter - converts office number to letter for display
//              modifyOffices - changes the values in the 2D array
//              errorCheckInput - verifies user input for office location
//              changeOccupantType - changes type of occupant stored in array
//              writeFile - overites data stored in file used for input
//                          with data stored in array
//***************************************************************************

#include <iostream> //for I/O
#include <iomanip>  //for output manipulators
#include <fstream>  //for stream variable
#include <cctype>   //for character checking

using namespace std;

//global enumerated type
enum occupantType {EMPTY, LAWYER, PARALEGAL, ASSISTANT};


//global constants
const int FLOORS = 5,
          FLOOR_MIN = 0,
          OFFICES = 8;

//typedef aliases
typedef int office[FLOORS] [OFFICES];

//prototypes
void readFile(office officeArray);
void displayOfficeTotals(office officeArray, int& emptyOffices);
void menuPrompt(office officeArray, int& emptyOffices);
bool displayOfficeStatus(bool occupied, office officeArray, int emptyOffices);
char officeLetter(int officeCol);
void modifyOffices(bool occupied, office officeArray, int& emptyOffices, 
                   char choice);
void errorCheckInput(int& floor, int& officeNum);
void changeOccupantType(office officeArray, int floor, int officeNum);
void writeFile(office officeArray);
//*********************************************************************
// FUNCTION: Main
// DESCRIPTION: Declares variables for use in program. Calls other functions.
//             
// OUTPUT: Displays program description.
//
// CALLS TO: openFile, readData, lowestArrayAmt, highestArrayAmt, averageArray,
//           printResults.
//**********************************************************************

int main()
{
   //initalize all values in array to 0
   office officeArray = {0};
   
   //assume that all offices are empty until filled
   int emptyOffices = 40; 
   
   readFile(officeArray);
   
   displayOfficeTotals(officeArray, emptyOffices);
   
   menuPrompt(officeArray, emptyOffices);
   
   displayOfficeTotals(officeArray, emptyOffices);
   
   //pause after displaying totals
   cin.ignore();
   cin.get();
   
   writeFile(officeArray);
   
   cout << "Press [enter] to exit" << endl;
   cin.ignore();
   cin.get();
   return 0;
}
//*********************************************************************
// FUNCTION: readFile
// DESCRIPTION: Opens binary file for reading.
// INPUT: Parameters: office officeArray
//        file: reads from a binary file that contains only data about 
//              occupied offices
// OUTPUT: Stores data in officeArray, passed by reference
//**********************************************************************
void readFile(office officeArray)
{
   ifstream binFile;
   
   int fileSize = 0,
       floor,
       office,
       numOfRecords;
   
   
   binFile.open("OFFICE.BIN", ios::binary | ios::in);
   
   if (!binFile)
      cout << endl << endl
           << "No office file found. Unable to use existing information."
           << endl << endl;
   else
   {
      binFile.seekg(0, ios::end);   //move to end of file
   
      fileSize = binFile.tellg();  //determine # of bytes
      
      binFile.seekg(0, ios::beg);   //move to top of file
      
      numOfRecords = fileSize / (sizeof(int) * 3);
      
      for (int i = 0; i < numOfRecords; i++)
      {
         //read in value for floor number
         binFile.read(reinterpret_cast<char*>(&floor), sizeof(int));
         
         //read in value for office number
         binFile.read(reinterpret_cast<char*>(&office), sizeof(int));
         
         //store value for given floor and office number into array
         binFile.read(reinterpret_cast<char*>(&officeArray[floor] [office]), 
                      sizeof(int));
      }
      
      binFile.close();
   }
      
}
//*********************************************************************
// FUNCTION: displayOfficeTotals
// DESCRIPTION: displays total occupied offices
// INPUT:
//       Parameters: officeArray, emptyOffices - number of empty offices
// OUTPUT:
//       emptyOffices - total of empty offices
//**********************************************************************
void displayOfficeTotals(office officeArray, int& emptyOffices)
{
   int status,
       lawyer = 0,
       paralegal = 0,
       assistant = 0;
   
   for (int r = 0; r < FLOORS; r++)
       for (int c = 0; c < OFFICES; c++)
       {
          status = officeArray[r] [c];
          
          switch (status)
          {
             case EMPTY:
                break;
             case LAWYER:
                emptyOffices--;
                lawyer++;
                break;
             case PARALEGAL:
                emptyOffices--;
                paralegal++;
                break;
             case ASSISTANT:
                emptyOffices--;
                assistant++;
                break;
          } 
       }
   
   
   cout << endl << endl
        << "Current Office Status"
        << endl
        << "*********************"
        << endl
        << "       " << lawyer << " lawyers"
        << endl
        << "       " << paralegal << " paralegals"
        << endl
        << "       " << assistant << " assistants"
        << endl
        << "     " << "---" 
        << endl
        << "       " << lawyer + paralegal + assistant << " occupied offices"
        << endl << endl;
   

}
//*********************************************************************
// FUNCTION: menuPrompt
// DESCRIPTION: Displays menu to user and calls other functions.
// INPUT:
//       Parameters: officeArray, emptyOffices
// OUTPUT:
//       Parameters: emptyOffices
//**********************************************************************
void menuPrompt(office officeArray, int& emptyOffices)
{
    bool exit = false; //Used as text to exit loop
    
    char choice;
    
    do
    {
        cout << endl << endl
             << "                           MENU"
             << endl
             << "**************************************************************"
             << endl
             << "E - Change from occupied to empty"
             << endl
             << "M - Modify occupant type"
             << endl
             << "O - Change from empty to occupied"
             << endl
             << "D - Done"
             << endl
             << "**************************************************************"
             << endl
             << "Please enter a choice from above: ";
        cin >> choice;
            
        choice = toupper(choice);
        
        switch (choice)
        {
            case 'E':
                modifyOffices(true, officeArray, emptyOffices, choice);
                break;
            case 'M':
                 modifyOffices(true, officeArray, emptyOffices, choice);
                break;
            case 'O':
                 modifyOffices(false, officeArray, emptyOffices, choice);
                break;
            case 'D':
                exit = true;
                break;
            default:
                cout << endl
                     << "Invalid selection - Valid selections are E, M, O, D"
                     << endl << endl;
        }
        
    }
    while (!exit);
    
}
//*********************************************************************
// FUNCTION: displayOfficeStatus
// DESCRIPTION: Displays occupied or unoccupied offices
// INPUT:
//       Parameters: occupied - decides whether or not to display occupied
//                              or unoccupied.
//                   officeArray, emptyOffices
// OUTPUT:
//       Returns a bool value indicating if there are occupied or unoccupied
//       offices available
//**********************************************************************
bool displayOfficeStatus(bool occupied, office officeArray, int emptyOffices)
{  
   
   if ((occupied) and (emptyOffices == 40))
   {   
      cout << endl << endl
           << "There are no occupied offices!"
           << endl << endl;
           
      return true;
   }
   else if ((!occupied) and (emptyOffices == 0))
   {
      cout << endl << endl
           << "There are no unoccupied offices!"
           << endl << endl;
      
      return true;
   }       
   else
   {
      if (occupied)
         cout << endl << endl
              << "Occupied offices"
              << endl;
      else
         cout << endl << endl
              << "Unoccupied offices"
              << endl;
              
      
      cout << left;
   
      for (int r = 4; r >= 0; r--)
      {
         if (r < 4)
            cout << endl;
   
         for (int c = 0; c < OFFICES; c++)
         {
            if ((officeArray[r] [c]) and (occupied))
               cout << r << setw(5) << officeLetter(c);
             
            else if ((!officeArray[r] [c]) and (!occupied))
               cout << r << setw(5) << officeLetter(c);
          
            else
               cout << setw(6) << " ";
         }
      }
   
      cout << right;
      
      return false;
   }
   
}  
//*********************************************************************
// FUNCTION: officeLetter
// DESCRIPTION: returns a char value for displaying office letter
// INPUT:
//       Parameters: officeCol - office column 
// OUTPUT: 
//       Return value: returns char - used to display the office letter to
//                     the user.
//**********************************************************************
char officeLetter(int officeCol)
{
   switch (officeCol)
   {
       case 0:
          return 'A';
       case 1:
          return 'B';
       case 2:
          return 'C';
       case 3:
          return 'D';
       case 4:
          return 'E';
       case 5:
          return 'F';
       case 6:
          return 'G';
       case 7:
          return 'H';
   }

}
//*********************************************************************
// FUNCTION: modifyOffices
// DESCRIPTION: makes changes to offices
// INPUT:  occupied status, officeArray, emptyOffices, choice - choice 
//         from menu prompt.
//**********************************************************************
void modifyOffices(bool occupied, office officeArray, int& emptyOffices, 
                   char choice)
{
   bool exit;
   
   int floor,
       officeNum;
       
       
   
   exit = displayOfficeStatus(occupied, officeArray, emptyOffices);
   
   if (!exit)
   {
     errorCheckInput(floor, officeNum);
     
     switch (choice)
     {
        case 'E':
           if (officeArray[floor] [officeNum] == EMPTY)
              cout << endl << endl
                   << floor << officeLetter(officeNum) << " is already empty!"
                   << endl << endl;
           else
           {
              officeArray[floor] [officeNum] = EMPTY;
              emptyOffices++;
           }
           break;
        case 'M':
           if (officeArray[floor] [officeNum] == EMPTY)
              cout << endl << endl
                   << floor << officeLetter(officeNum) << " is empty"
                   << endl << endl;
           else
              changeOccupantType(officeArray, floor, officeNum);
           break;
        case 'O':
           if (officeArray[floor] [officeNum])
              cout << endl << endl
                   << "This office is already occupied!"
                   << endl << endl;
           else
           {
              changeOccupantType(officeArray, floor, officeNum);
              emptyOffices--;
           }   
           break;   
           
     }
         
   }
}
//*********************************************************************
// FUNCTION: errorCheckInput
// DESCRIPTION: Checks input for office location
// INPUT:  floor - floor number, officeNum - corresponding office
// OUTPUT: floor and officeNum
//**********************************************************************
void errorCheckInput(int& floor, int& officeNum)
{
   char officeLetter;
   
   bool valid = true;
   
   do
   {
      cout << endl << endl
           << "Enter the floor number and office letter you wish to modify: ";
      cin >> floor >> officeLetter;
      
      if (floor >= FLOOR_MIN and floor < FLOORS)
         switch (officeLetter)
         {
            case 'a':
            case 'A':
               officeNum = 0;
               break;
            case 'b':
            case 'B':
               officeNum = 1;
               break;
            case 'c':
            case 'C':
               officeNum = 2;
               break;
            case 'd':
            case 'D':
               officeNum = 3;
               break;
            case 'e':
            case 'E':
               officeNum = 4;
               break;
            case 'f':
            case 'F':
               officeNum = 5;
               break;
            case 'g':
            case 'G':
               officeNum = 6;
               break;
            case 'h':
            case 'H':
               officeNum = 7;
               break;
            default:
               cout << endl << endl
                    << "Incorrect entry! Format is a floor number followed by "
                    << "aN office letter. example - 1A";
               valid = false;
         }
         
      else
      {
         cout << endl << endl
              << "Incorrect entry! Format is a floor number followed by "
              << "aN office letter. example - 1A";
         valid = false;
      }
         
   }
   while (!valid);
}
//*********************************************************************
// FUNCTION: changeOccupantType
// DESCRIPTION: changes occupant type stored in array
// INPUT:  Parameters: officeArray, floor, officeNum
// OUTPUT: officeArray
//**********************************************************************
void changeOccupantType(office officeArray, int floor, int officeNum)
{
  char occupant;
  
  do
  {
     cout << endl << endl
          << "L - Lawyer"
          << endl
          << "P - Paralegal"
          << endl
          << "A - Assistant"
          << endl
          << "---------------------------"
          << endl
          << "Choose an option from above: ";
     cin >> occupant;
     
     occupant = toupper(occupant);
     
     switch (occupant)
     {
        case 'L':
           officeArray[floor] [officeNum] = LAWYER;
           break;
        case 'P':
           officeArray[floor] [officeNum] = PARALEGAL;
           break;
        case 'A':
           officeArray[floor] [officeNum] = ASSISTANT;
           break;
        default:
           cout << endl << endl
                << "Invalid entry, choices are L, P, or A"
                << endl << endl;
           occupant = '!';
     }
     
  }
  while (occupant == '!');
}
//*********************************************************************
// FUNCTION: writeFile
// DESCRIPTION: Writes data stored in array out to a file.
// INPUT:  officeArray
//**********************************************************************
void writeFile(office officeArray)
{
   ofstream binFile;
   
   

   binFile.open("OFFICE.BIN", ios::binary | ios::out | ios::trunc);
   
   for (int r = 0; r < FLOORS; r++)
       for (int c = 0; c < OFFICES; c++)
           if (officeArray[r] [c] != EMPTY)
           {
              binFile.write( reinterpret_cast<char*>(&r), sizeof(int));
              binFile.write( reinterpret_cast<char*>(&c), sizeof(int));
              binFile.write( reinterpret_cast<char*>(&officeArray[r] [c]),
                             sizeof(int));
           }
                           
   binFile.close();
   
}
