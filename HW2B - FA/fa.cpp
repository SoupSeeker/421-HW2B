#include<iostream>
#include<fstream>
#include<string>
using namespace std;

//------------------------------------------------
// CS421 File fa.cpp for HW2B DFA->Scanner Function
// Your name: Jake Tremblay
//------------------------------------------------

// ** Change this to fit the HW2B specification - look for **
// ** Must have the same types of tracing couts as mytoken.
// ** Must complete the ** comments with RE

// ---------- DFAs follow -------------------------

// MYTOKEN DFA done by professor as a sample
// This FA is for c d^+
bool mytoken(string s)
{
	int state = 0;
	int charpos = 0;
	cout << endl;
	cout << "Trying the mytoken machine..." << endl;

	while (s[charpos] != '\0')
	{
		cout << "current state: " << state << endl;
		cout << "character: " << s[charpos] << endl;

		if (state == 0 && s[charpos] == 'c')
			state = 1;
		else
			if (state == 1 && s[charpos] == 'd')
				state = 2;
			else
				if (state == 2 && s[charpos] == 'd')
					state = 2;
				else
				{
					cout << "I am stuck in state " << state << endl;
					return(false);
				}
		charpos++;
	}//end of while

  // where did I end up????
	if (state == 2) return(true);  // end in a final state
	else return(false);
}// end of mytoken


// IDENT DFA 
// This FA is for RE: l ( l | d | _ )^*
// where l ==> c | d, and d ==> 8 | 9
// so RE is ==> (c|d) ( (c|d) | (8|9) | _ )^*
bool ident_token(string s)
{
	int state = 0;
	int charpos = 0;
	cout << endl;
	cout << "Trying the ident_token machine..." << endl;

	while (s[charpos] != '\0')
	{
		cout << "current state: " << state << endl;
		cout << "character: " << s[charpos] << endl;

		if (state == 0 && s[charpos] == 'c') state = 1;
		else if (state == 0 && s[charpos] == 'd') state = 1;
		else if (state == 1 && s[charpos] == 'c') state = 1;
		else if (state == 1 && s[charpos] == 'd') state = 1;
		else if (state == 1 && s[charpos] == '8') state = 1;
		else if (state == 1 && s[charpos] == '9') state = 1;
		else if (state == 1 && s[charpos] == '_') state = 1;
		else {
			//if we get here it means the input was not found in our language
			cout << "Invalid input detected..." << endl;
			cout << "I am stuck in state " << state << endl;
			return(false);
		}
		charpos++;
	}//end of while

  // where did I end up????
	if (state == 1) return(true);  // end in a final state
	else cout << "I am stuck in state " << state << endl;		//added this line to help illustrate when the DFA gets stuck at the end of processing
	return(false);
}//end of ident


// REAL DFA 
// This FA is for RE: d^* . d^+ which becomes
// (8|9)^*.(8|9)^+
bool real_token(string s)
{
	int state = 0;
	int charpos = 0;
	cout << endl;
	cout << "Trying the real_token machine..." << endl;

	while (s[charpos] != '\0')
	{
		cout << "current state: " << state << endl;
		cout << "character: " << s[charpos] << endl;

		if (state == 0 && s[charpos] == '8') state = 0;
		else if (state == 0 && s[charpos] == '9') state = 0;
		else if (state == 0 && s[charpos] == '.') state = 1;
		else if (state == 1 && s[charpos] == '8') state = 2;
		else if (state == 1 && s[charpos] == '9') state = 2;
		else if (state == 2 && s[charpos] == '8') state = 2;
		else if (state == 2 && s[charpos] == '9') state = 2;
		else {
			//if we get here it means the input was not found in our language
			cout << "Invalid input detected..." << endl;
			cout << "I am stuck in state " << state << endl;
			return(false);
		}
		charpos++;
	}//end of while

  // where did I end up????
	if (state == 2) return(true);  // end in a final state
	else cout << "I am stuck in state " << state << endl;		//added this line to help illustrate when the DFA gets stuck at the end of processing
	return(false);
}//end of real


//INT DFA 
// This FA is for RE: d^+ which becomes
// (8|9)^+ for our example
bool integer_token(string s)
{
	int state = 0;
	int charpos = 0;
	cout << endl;
	cout << "Trying the integer_token machine..." << endl;

	while (s[charpos] != '\0')
	{
		cout << "current state: " << state << endl;
		cout << "character: " << s[charpos] << endl;

		if (state == 0 && s[charpos] == '8') state = 1;
		else if (state == 0 && s[charpos] == '9') state = 1;
		else if (state == 1 && s[charpos] == '8') state = 1;
		else if (state == 1 && s[charpos] == '9') state = 1;
		else {
			//if we get here it means the input was not found in our language
			cout << "Invalid input detected..." << endl;
			cout << "I am stuck in state " << state << endl;
			return(false);
		}
		charpos++;
	}//end of while

  // where did I end up????
	if (state == 1) return(true);  // end in a final state
	else cout << "I am stuck in state " << state << endl;		//added this line to help illustrate when the DFA gets stuck at the end of processing
	return(false);
}// end of int


// -----------------------------------------------------

enum tokentype { ERROR, MYTOKEN, IDENT, REAL, INT };
int scanner(tokentype&, string&);  // to be called by main
fstream fin;   // file stream to use

// The test-driver -- NO NEED TO CHANGE THIS
int main()
{
	string fname;
	cout << "Enter the input file name:";
	cin >> fname;

	fin.open(fname.c_str(), fstream::in);

	string tokens[5] = { "ERROR", "MYTOKEN", "IDENT", "REAL", "INT" };
	tokentype thetype;								 //my compiler doesn't like this enum declaration, no issues however so oh well
	string theword;

	while (true)  // keep on going 
	{
		scanner(thetype, theword);					 // the paramers will be set by Scanner
		if (theword == "EOF") break;
		cout << ">>>>>Type is:" << tokens[thetype] << endl;
	}

	cout << ">>>>>End of File encountered" << endl;
	fin.close();
}// end of main 


// Scanner sets the_type and w - TO BE COMPLETED **
int scanner(tokentype& the_type, string& w)
{
	// This goes through all machines one by one on the input string w, will check the word against each tokenizer linearly
	// until it finds one that can tokenize the input, otherwise it returns an error.

	cout << endl;									//added some space for a cleaner output
	cout << ".....Scanner was called..." << endl;

	fin >> w;										// grab next word from fain.txt
	cout << ">>>>>Word is:" << w << endl;
	if (w == "EOF") {
		return 0;									//if EOF, just return and program will break / not process it
	}
	else if (mytoken(w))
	{
		the_type = MYTOKEN;
	}
	else if (ident_token(w)) 
	{
		the_type = IDENT;
	}
	else if (real_token(w))
	{
		the_type = REAL;
	}
	else if (integer_token(w))
	{
		the_type = INT;
	}
	else											//none of the FAs returned TRUE
	{
		cout << ">>>>>Lexical Error: The string is not in my language" << endl;
		the_type = ERROR;
	}
	return 0;
}//the very end of scanner




