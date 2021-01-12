#include <fstream>
#include <iostream>
#include <string>
#include "tokens.h"
#include "parse.h"
using namespace std;

int main(int argc, char *argv[])
{
    
    //string fileName;
    bool debug = false;        //set to true to check things
    bool program3 = false;     //set to true to generate program3 outputs
    bool fileFound = false;
    //istream *input;
    ParseTree *programTree;
    int lineNumber = 1;
    if (argc > 2)
    {
        cout << "TOO MANY FILENAMES" << endl;
        return 0;
    }
    else if (argc == 2)
    {
        //fileName = argv[1];
        ifstream readFile;
        readFile.open(argv[1]);
        //input = &readFile;
        fileFound = true;
        
        if (readFile.is_open() == false)
        {
            cout << "COULD NOT OPEN " << argv[1] << endl;
            return 0;
        }
        else if (readFile.is_open() == true && debug)
        {
            cout << "Opened the file" << endl;
        }
        
        if (debug)
        {
            cout << "File was found: " << fileFound << endl << "Reading from a file with name " << argv[1] << endl;
            char ch;
            while (readFile.get(ch)) { cout << ch; }
        }
        
        programTree = Prog(readFile, lineNumber);
    }
    else
    {
        //input = &cin;
        
        if (debug)
        {
            cout << "Input coming from standard in (cin)" << endl;
            char ch;
            while (cin.get(ch)) { cout << ch; }
        }
        
        programTree = Prog(cin, lineNumber);
    }
    
    /*
    int lineNumber = 1;
    ParseTree *programTree = Prog(*input, lineNumber);
    */
    
    
    // Check that a programTree was made, only print when something has a non-zero value
    if (programTree && program3)
    {
        if (programTree->NodeCount()) cout << "NODE COUNT: " << programTree->NodeCount() << endl;
        if (programTree->InteriorCount()) cout << "INTERIOR COUNT: " << programTree->InteriorCount() << endl;
        if (programTree->OpCount()) cout << "OPS COUNT: " << programTree->OpCount() << endl;
        if (programTree->StringCount()) cout << "STRING COUNT: " << programTree->StringCount() << endl;
        if (programTree->DepthCount()) cout << "MAX DEPTH: " << programTree->DepthCount() << endl;
    }
    
    if (programTree)
    {
        try {
            programTree->Eval();
        }
        catch(std::logic_error& e) {
            cout << "RUNTIME ERROR " << e.what() << endl;
        }
    }
    
    
    return 0;
}
