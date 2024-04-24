#include "translator.hpp"

#include <fstream>
#include <string>
#include <memory>
#include <filesystem>
#include <algorithm>
#include <iostream>

using namespace std;


int main(int argc, char *argv[]) {
    printf("Hello World!");
    while (true) {
        string inputted;
        getline(cin >> ws, inputted);
        string command = inputted.substr(0, inputted.find(" "));
        string cd = string(argv[0]);
        
        cd = cd.substr(0, cd.find_last_of('\\') + 1);


        if (command == "exit") break;

        string filed = inputted.substr(inputted.find(" ") + 1, inputted.size());

        if (command == "test") {
            ifstream BS_file(cd + "testing.bs");

            if (BS_file.is_open()) {
                stringstream buffer;
                buffer << BS_file.rdbuf();
                printf("%s\n", translator(buffer.str()).c_str());
            } else {
                printf("File \"testing.bs\" does not exist!\n");
            }
        }

        if (command == "comp") {
            if (filed.find(".") != filed.size() && filed.substr(filed.find(".") + 1, filed.size()) == "bs") {
                ifstream BS_file(cd + filed);

                if (BS_file.is_open()) {
                    stringstream buffer;
                    buffer << BS_file.rdbuf();
                    printf("%s\n", translator(buffer.str()).c_str());

                } else {
                    printf("This file does not exist!\n");
                }
            } else {
                printf("file \"%s\" has the wrong extension! (must be \".bs\")\n", filed.c_str());
            }
            
            continue;
        }


        if (command == "cd") {
            printf("%s\n", cd.c_str());
            continue;
        }

        if (command == "input") {
            printf("Input some code:\n");
            string coded;
            getline(cin >> ws, coded);

            printf("%s\n", translator(coded).c_str());
            
            continue;
        }

        if (command == "find") {
            for (const auto & entry : filesystem::directory_iterator(cd))
                printf("%s\n", entry.path().c_str());
        }
    }

    return 0;
}
