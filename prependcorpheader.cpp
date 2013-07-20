/*
 * File: prependcorpheader.cpp 
 *  
 * Description: Prepend text from a template file to another file and fill in 
 * relevant data.
 *  
 * Author: Brian Jenkins <bj@jenkinz.com> 
 * Date: July 4, 2013  
 */
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
//#include <regex>

#define DEBUG

using namespace std;

namespace {

const char * const versionStr = "0.0.3";

// Command line arg variables
bool helpSet = false;
bool versionSet = false;
string inputFilename;
string prependFilename;

// Utility functions

/**
 * Remove extension from a filename 
 *  
 * @param s the filename
 * @param delim the extension delimeter
 * 
 * @return string the filename stripped of its extension
 */
string stripext(const string& s, const char * delim = ".")
{
    string s2(s);
    const size_t n = s2.find_first_of(delim);
    if (n != string::npos) 
        s2 = s.substr(0, n);
    return s2;

}

/**
 * Remove path from a filename. 
 *  
 * @param s the filename
 * @param delim the path-separator
 * 
 * @return string the filename stripped of its path
 */
string strip2file(const string& s, const char * delim = "/\\")
{
    string s2(s);
    const size_t n = s2.find_last_of(delim);
    if (n != string::npos) 
        s2 = s.substr(n + 1);
    return s2;

}

/**
 * Remove path and extension from a filename.
 *  
 * @param s the filename
 * 
 * @return string the stripped filename
 */
string strip2base(const string& s)
{
    string s2(s);
    s2 = strip2file(s2);
    s2 = stripext(s2);
    return s2;
}

// Command line arg processing functions

/**
 * Prints the program's version information to stdout. 
 * 
 * @param prog 
 */
void version(const string& prog)
{
    cout << prog << " version " << versionStr << " by Brian Jenkins"
         << endl << endl;
}

/**
 * Prints the program's usage information to stdout. 
 * 
 * @param prog the name of the program
 */
void help(const string& prog)
{
    cout << "usage:" 
            << "\t"   << prog << " [--help] [--version]" << endl
            << "\t" << prog << " <inputfile> <prependfile>" << endl;
}

/**
 * Parses the command line arguments into variables. 
 * 
 * @param argc the arg count
 * @param argv the arg values
 */
void options(int argc, char *argv[])
{
    const string& prog = strip2base(argv[0]);

    if (argc <= 1) { // prog called w/out any args
       versionSet = true;
       helpSet = true;
    }
    else { // argc > 1
        int fileCnt = 0;
        for (int i=1; i<argc; ++i) {
            const string& tmpStr = string(argv[i]);
            if (tmpStr == "--version") {
                versionSet = true;
            }
            else if (tmpStr == "--help") {
                helpSet = true;
            }
            else {
                bool startsWithDash = false;
                if (tmpStr[0] == '-') { // unknown flag
                    startsWithDash = true;
                }
                if (!startsWithDash && fileCnt == 0) { 
                    // first filename encountered - inputfile
                    inputFilename = tmpStr;
                    
                }
                else if (!startsWithDash && fileCnt == 1) { 
                    // second filename encountered - prependfile
                    prependFilename = tmpStr;
                }
                else {
                    cerr << "option: " << argv[i] << " not recognized!" << endl;
                    exit(0);
                }
                ++fileCnt;
            }
        } // end for
    }

    if (versionSet || helpSet) { // print version and/or help then exit
        if (versionSet) {
            version(prog);
        }
        if (helpSet) {
            help(prog);
        }
        exit(0);
    }
    else if (inputFilename.empty() || prependFilename.empty()) {
        cerr << "Too few arguments - <inputfile> and <prependfile> required" 
             << endl;
        help(prog);
        exit(0);
    }

    #ifdef DEBUG
    cout << "Input file: " << inputFilename << endl;
    cout << "Prepend file: " << prependFilename << endl;
    #endif
}

// PCH functions

/** 
 * Compare file1 to file2 based on a template regex. 
 *  
 * @param file1 the file to compare 
 * @param file2 the file to compare file1 against 
 * @todo 
 */
bool compareHdrContents(const ios *file1, const ios *file2)
{
    bool updateNeeded = true;
#if 0
#ifdef DEBUG
    cout << "Comparing headers to see if file1 needs updating...";
    cout << " TODO!" << endl;

    cout << "file1: " << file1->rdbuf();
    cout << "file2: " << file2->rdbuf();
#endif
#endif
    return updateNeeded;
}

} // end anonymous namespace

/**
 * Main entry point. 
 * 
 * @param argc 
 * @param argv 
 * 
 * @return int 0 if succesful; non-zero if unsuccessful
 */
int main(int argc, char *argv[])
{
    options(argc, argv);

    ifstream inputFile;
    inputFile.open(inputFilename.c_str(), ios_base::in);
     if (!inputFile.is_open()) {
        cerr << "file: `" << inputFilename << "' not found!" << endl;
        return -1;
    }

    fstream prependFile;
    prependFile.open(prependFilename.c_str(), ios_base::in|ios_base::out);
    if (!prependFile.is_open()) {
        cerr << "file: `" << prependFilename << "' not found!" << endl;
        return -1;
    }

    bool updateNeeded = compareHdrContents(&prependFile, &inputFile);

    if (!updateNeeded) {
        cout << "`" << prependFilename 
                    << "' corporate header is up-to-date" << endl;
    }
    else {
        cout << "Updating `" << prependFilename << "'..." << endl;
     
		char c;
		bool reading = false;
		//bool readingReplToken = false;
		//string replaceToken;
        string templateContents;

		while ((c = inputFile.get()) != EOF) {
			if (c == '/' && inputFile.peek() == '*') {
				reading = true;
			}

			if (reading) {
				
				/*
				if (c == '%' && inputFile.peek() != '%') {
					readingReplToken = true;
					continue;
				}

				if (readingReplToken) {
					replaceToken.append(1, c);
					
					if (inputFile.peek() == '%') {
						// Do the replacement...
						if (replaceToken == "DATE") {
							templateContents.append("brian_date");
						}
						inputFile.get(); // get rid of closing '%'
						readingReplToken = false; 
						replaceToken = "";
						break;
					}
					
					continue;
				} 
				*/ 

				templateContents.append(1, c);
			}
        }

		#ifdef DEBUG
        cout <<  "templateContents after read is: " << endl << templateContents 
             << endl;
        #endif

		size_t counter = 0;
		bool readingFirstComment = false;
		bool firstCharRead = false;
        string prependFileContents;
		prependFileContents.append(templateContents);

        while ((c = prependFile.get()) != EOF) {
			if (counter == 0 && c == '/' && prependFile.peek() == '*') {
				readingFirstComment = true;
			}

			++counter;

			if (!firstCharRead) {
				if (c == ' ') {
					continue;
				}

				firstCharRead = true;
			}

			if (readingFirstComment) { // keep ignoring 1st comment until */
				if (c == '*' && prependFile.peek() == '/') {
					prependFile.get(); // get rid of the '/'
					readingFirstComment = false;
				}
				continue;
			}
			
            prependFileContents.append(1, c); // append rest of file contents
        }

        #ifdef DEBUG
        cout << "prependFileContents after update is: " << endl 
             << prependFileContents
             << endl;
        #endif

        cout << "Done." << endl;
    }

    inputFile.close();
    prependFile.close();

    return 0;
} // end main

