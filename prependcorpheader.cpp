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

using namespace std;

namespace {

const char * const versionStr = "0.0.1";

// Command line arg variables
bool helpSet = false;
bool versionSet = false;
string inputFilename;
string prependFilename;

// Utility functions
string stripext(const string& s, const char * delim = ".")
{
    string s2(s);
    const size_t n = s2.find_first_of(delim);
    if (n != string::npos) 
        s2 = s.substr(0, n);
    return s2;

}

string strip2file(const string& s, const char * delim = "/\\")
{
    string s2(s);
    const size_t n = s2.find_last_of(delim);
    if (n != string::npos) 
        s2 = s.substr(n + 1);
    return s2;

}

string strip2base(const string& s)
{
    string s2(s);
    s2 = strip2file(s2);
    s2 = stripext(s2);
    return s2;
}

// Command line functions
void version(const string& prog)
{
    cout << prog << " version " << versionStr << " by Brian Jenkins"
         << endl << endl;
}

void help(const string& prog)
{
    cout << "usage:" 
            << "\t"   << prog << " [--help] [--version]" << endl
            << "\t\t" << prog << " <inputfile> <prependfile>" << endl;
}

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
                    cout << "option: " << argv[i] << " not recognized!" << endl;
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
        cout << "Too few arguments - <inputfile> and <prependfile> required" 
             << endl;
        help(prog);
        exit(0);
    }

    #ifdef DEBUG
    cout << "Input file: " << inputFilename << endl;
    cout << "Prepend file: " << prependFilename << endl;
    #endif
}

// Compare file1 to file2 based on a template regex.
// Return true if file1 needs updating depending on file2.
bool compareHdrContents(const ios *file1, const ios *file2)
{
    bool updateNeeded = true;

    #ifdef DEBUG
    cout << "Comparing headers to see if file1 needs updating...";
    cout << " TODO!" << endl;

    cout << "file1: " << file1->rdbuf();
    cout << "file2: " << file2->rdbuf();
    #endif

    return updateNeeded;
}

} // end anonymous namespace

int main(int argc, char *argv[])
{
    options(argc, argv);

    ifstream *inputFile = new ifstream(inputFilename.c_str(), ios_base::in);
    if (!(*inputFile)) {
        cout << "file: `" << inputFilename << "' not found!" << endl;
        delete inputFile;
        return -1;
    }

    ofstream *prependFile = new ofstream(prependFilename.c_str(),
                                         ios_base::in|ios_base::out);
    if (!(*prependFile)) {
        cout << "file: `" << prependFilename << "' not found!" << endl;
        delete prependFile;
        return -1;
    }

    bool updateNeeded = compareHdrContents(prependFile, inputFile);

    if (!updateNeeded) {
        cout << "`" << prependFilename 
                    << "' corporate header is up-to-date" << endl;
    }
    else {
        cout << "`" << prependFilename << "' is out-of-date. Updating..." 
             << endl;
        // TODO read inputFile's template contents, transform with current data
        // (e.g. current date) and prepend to prependFile....
        cout << "Done." << endl;

        #ifdef DEBUG
        cout << "updated prependFile: " << prependFile->rdbuf();
        #endif
    }

    delete inputFile;
    delete prependFile;

    return 0;

#if 0
    // Equivalent to while (std::cin >> token) { insert token into vector }
    std::vector<std::string> tokens;
    std::istream * input = &std::cin;
    std::ifstream * in = 0;
    if (!inputfile.empty()) {
        in = new std::ifstream(inputfile.c_str());
        if (!(*in)) {
            std::cout << "file: " << inputfile << " not found!" << std::endl;
            help(argv[0]);
            delete in;
            return -1;
        }
        input = in;
        stdin_used = false;     // std::in is implied, this isn't necessary
    }

    std::istream_iterator<std::string> file_iter(*input), end_of_stream;
    std::copy(file_iter, end_of_stream, std::back_inserter(tokens));

    typedef std::set<unsigned> DictValue;
    typedef std::map<std::string, DictValue> DictType;
    DictType dict;
    const int N = tokens.size();
    assert(N > 0);
    for (int i = 0; i < N; ++i) {
        const std::string token(tokens[i]);
        // std::cout << "vector token = " << token << std::endl;
        const std::string tok = transform_token(token);
        DictType::iterator it =  dict.find(tok);

        // if found in dictionary, test, filter, and add to set of indexes
        // indicating number of duplicates.
        // Otherwise - its a new entry and add to the dictionary to allow for
        // testing for any other duplicates that may occur later in the vector
        if (it != dict.end()) {
            // stripped filename found - insert index into value set subject
            // to the filter iterating over the set where set has more than
            // 1 entry - assertion of relationship between map and set
            // that is set has N entries
            assert(it->second.size() >= 1);
            const bool isdup = dupfilename_filter(i, tokens, it->second);
            if (isdup)
                it->second.insert(i);
        }
        else {
            // insert unique pathless filename into dictionary for later
            // lookup qualification.
            DictValue v; 
            v.insert(i);
            dict.insert(DictType::value_type(tok, v));
        }
    } // end for

    // Find duplicates if any and print out
    int dups_found = 0, dup_pairs_found = 0;
    for (DictType::iterator it = dict.begin(); it != dict.end(); ++it) {
        if (it->second.size() > 1) {
            if (dups_found == 0) { 
                std::cout << "Duplicates Found: "
                          << "  => Exact match or suffix match equivalency"
                          << "\n-----------------\n";
            }
            dups_found += it->second.size();    
            ++dup_pairs_found;
            for (DictValue::iterator i = it->second.begin(); 
                 i != it->second.end(); ++i) {
                const std::string& dup = tokens[*i];
                std::cout << '\t' << dup << "\n";
            }
            std::cout << std::endl;
        }
    }

    delete in;
    if (dup_pairs_found)
        std::cout << "duplicate pairs found = " << dup_pairs_found << std::endl;
    return dups_found;  // or return dup_pairs_found;

#endif

} // end main

