#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

//--------------------------------------
// CS421 File td.cpp for HW2B Table-Driven Scanner
// Your name: Jake Tremblay
//--------------------------------------

// Complete this to fit the HW2B specification - look for **
// Must have the same types of tracing couts as my demo program.


// info on each DFA
struct info
{
    string name;  // token name
    int startstate;
    int finalstate;
};

info DFAs[4];     // store up to 4 dfas' start and final

int  TRS[10][4];  // store all trs's - states 0-9 and chars a b c d -- all dfas transitions are in here 
                    // transition table is trs[state we are in][character input to process]
// ----- utility functions -----------------------

//read table will take in all the information from the files trs.txt and dfas.txt to build our
//table and each of the "dfa" in the program
int readTables()
{
    ifstream fin("trs.txt", ios::in);           // ** Read in the files into TRS and DFAs
    ifstream fin2("dfas.txt", ios::in);
   
    int tableCount = 0;
    while (!fin2.eof()) {
        info newDFA;
        fin2 >> newDFA.name >> newDFA.startstate >> newDFA.finalstate;
        DFAs[tableCount++] = newDFA;                                        //we are placing the new dfa info into the info array at spot 0 then 1..
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            int input;
            fin >> input;
            TRS[i][j] = input;
        }
    }

    return tableCount;                      // ** Return how many DFAs were read
}

//display tables will take the number of DFAs to aid in printing the dfa info
//it will also display the transition table such that the rows are states and the
// columns are the valid input
void displayTables(int numDFAs)
{
    // ** display DFAs nicely labeled
    for (int i = 0; i < numDFAs; i++) {
        cout << "Name: " << DFAs[i].name << "  Start state: " << DFAs[i].startstate << "  Final state: " << DFAs[i].finalstate << endl;
    }

    // ** display TRS nicely labeled
    cout << "        a b c d" << endl;
    for (int i = 0; i < 10; i++) {
        cout << "State " << i;
        for (int j = 0; j < 4; j++) {
            if (TRS[i][j] >= 0) cout << " " << TRS[i][j];
            else cout << "  ";
        }
        cout << endl;
    }
}

bool accept(info dfa, string word)
{
    // ** Does the dfa accept the word?
    // Start with the start state of the DFA and
    // look up the next state in TRS for each char in word.
    // At the end of the word, make sure you are in the 
    // final state of the DFA.
    // Use a formula to convert chars to TRS col numbers.
    int currentState = dfa.startstate;              //dont alter the dfa objects starting state, just use a local variable

    for (auto ch : word) {                          //for each character in the word

        int col = ch - 'a';                           //convert to col using the ch - 'a' trick
        cout << "state: " << currentState << "  char: " << ch << endl;

        if (TRS[currentState][col] >= 0) {               //if TRS[state][input] is not -1, we have a new state to go to
            cout << "new state: " << TRS[currentState][col] << endl;
            currentState = TRS[currentState][col];
        }
        else {
            currentState = TRS[currentState][col];
            cout << "new state: " << currentState << endl;
            return false;                                  //if our current state is -1, we have hit a character that does not belong and we can stop processing
        }
    }

    if (currentState == dfa.finalstate) {                   //at the end of each word we process we want to check if we have a final state
        cout << "token found!: " << dfa.name << endl;
        return true;
    }

    return false;
}

// start of main -- primary driver for the program
int main()
{
    cout << "This is a table driven scanner. Needs trs.txt and dfas.txt." << endl;
    cout << "States are 0 to 9 and chars are a to d" << endl;

    int numDFA = readTables(); // how many DFAs were read
    displayTables(numDFA);     // DISPLAY TABLES
    cout << ".....done reading tables...." << endl;

    string word;
    while (true)
    {
        cout << "@@Enter a string: ";
        cin >> word;
        if (word == "exit") break;                          //the ctrl-c gives me an error on my machine so adding this

        for (int i = 0; i < numDFA; i++) {                  //cycle through the DFA, if accept is true we can break because we found a match
            cout << "trying dfa " << i << "-------" << endl;    //else just print a message to clean up output..
            if (accept(DFAs[i], word)) break;
            else cout << "lexical error!" << endl;
            
        }
        // ** try the DFAs one by one and see 
        //      if the word is accepted
        //      if so, display the word and the token name
        // ** if no DFA does, generate a lexical error message.
        cout << "do control-C or type 'exit' to quit" << endl;
    }
    return 0;
}//the end
