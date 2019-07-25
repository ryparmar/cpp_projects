#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <algorithm>

using std::size_t;

void print_help() {
    std::printf("%s%s%s",
                "WORD-COUNTER HELP\n\n", 
                "Word-counter counts all the words in given txt file or per all the files in a given directory.\n",
                "Files can be printed to the console or into the file.\n\n"
    );
    std::printf("%s%s%s%s",
                "OPTIONS\n",
                "-h, --help \t shows this help\n",
                "-p, --paralel \t process task with paralel approach (multi-threading); default = serial processing\n",
                "-f, --file \t must be followed by a name of file with names of files to process; e.g. -f input.txt\n\n"
    );
}

bool is_help(std::string const& argument) {
    return argument == "--help" || argument == "-h";
}   

bool is_paralel(std::string const& argument){
    return argument == "--paralel" || argument == "-p";
}

bool is_file(std::string const& argument){
    return argument == "--file" || argument == "-f";
}

// returns value of given input
std::string get_input(int argc, char **arguments, std::pair<std::string, std::string> const arg){
    for (int i = 0; i < argc; i++){
        if (arguments[i] == arg.first || arguments[i] == arg.second){
            if ( arguments[i+1] ) return arguments[i+1];
        }
    }

    return "";
}

// parse the input file with names of files which should be processed
void get_filenames(std::string const &file, std::vector<std::string> &filenames){
    std::ifstream infile;
    std::string line;
    infile.open(file.c_str());
    while (std::getline(infile, line)) {
        filenames.push_back(line);
    }
}

// returns false if character is not alpha-numeric or -
bool non_alpha_numeric(const char c){
    if ( (c <= 'z' && c >= 'a') || (c == '-') )
        return false;
    return true;
}

// remove all non-alpha-numerical chars from string
void parse_word(std::string &word){
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    word.erase(std::remove_if(word.begin(), word.end(), non_alpha_numeric), word.end());
}

// count words in given file
void countInFile(std::string const &file, std::map<std::string, double> &data){
    // std::ifstream infile;
    std::ifstream infile;
    std::string line;
    infile.open(file.c_str());
    if ( infile ){
        while (std::getline(infile, line)) {
            std::stringstream sstream(line);
            std::string word;
            while (sstream >> word){
                parse_word(word);
                data[word]++;
                // std::cout << word << ": " << data[word] << "\n";
            }
            // std::cout << line << '\n';
        }
        infile.close();
    }
    else{
        std::cout << "FILE DOESNT EXIST: " << file.c_str() << "\n";
    }
}

// GENERATE FILENAMES IN UNIX USING
// ls $PWD/test*.txt >> input.txt


int main(int argc, char **argv){

    if (std::any_of(argv, argv+argc, is_help)) {
        print_help();
        return 0;
    }

    std::string inputFile;
    std::vector<std::string> fileNames;
    std::map<std::string, double> data;  

    if (std::any_of(argv, argv+argc, is_file)) {
        inputFile = get_input(argc, argv, std::pair<std::string, std::string> ("-f", "--file"));
        if ( inputFile != "" ){
            get_filenames(inputFile, fileNames);
            std::cout << "INPUTED FILES:\n";
            for (auto file : fileNames)
                std::cout << file << "\n";
        }
        else{
            std::cout << "WRONG INPUT ARGUMENT - FILENAME IS MISSING.\n\n"; 
            return 0;
        }
    }

    if (std::any_of(argv, argv+argc, is_paralel)) {
        std::cout << "PARALEL PROCESSING...\n";

    }
    else{
        std::cout << "SERIAL PROCESSING...\n";
        for (auto file : fileNames){
            countInFile(file, data);
        }
        std::cout << "\nWORDCOUNTS:\n";
        for (auto el : data)        
            std::cout << el.first << ": " << el.second << "\n";
    }

    return 0;
}