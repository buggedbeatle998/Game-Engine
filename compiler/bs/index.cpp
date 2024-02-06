//#include "translator.hpp"
#include "translator.cpp"

#include <fstream>
#include <string>
#include <memory>
#include <filesystem>

using namespace std;


int main(int argc,char* argv[]) {
    while (true) {
        //
        string inputted;
        getline(cin >> ws, inputted);
        string command=inputted.substr(0, inputted.find(" "));
        string cd=string(argv[0]);
        
        cd=cd.substr(0,cd.find_last_of('\\')+1);


        if (command=="exit") break;

        string filed=inputted.substr(inputted.find(" ")+1,inputted.size());

        if (command=="comp") {
            ifstream BS_file(cd+filed);

            if (BS_file.is_open()) {
                stringstream buffer;
                buffer << BS_file.rdbuf();
                printf("%s\n",translator(buffer.str()).c_str());

            } else {
                printf("This file does not exist!\n");
            }

            continue;
        }

        if (command=="cd") {
            printf("%s\n",cd.c_str());
            continue;
        }

        if (command=="input") {
            printf("Input some code:\n");
            string coded;
            getline(cin >> ws, coded);

            printf("%s\n",translator(coded).c_str());
            
            continue;
        }
    }

    return 0;
}
