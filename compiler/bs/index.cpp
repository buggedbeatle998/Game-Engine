#include "translator.cpp"

#include <fstream>
#include <iostream>
#include <string>
#include <memory>


int main() {
    while (true) {
        std::string inputted;
        std::getline(std::cin >> std::ws, inputted);
        std::string command=inputted.substr(0, inputted.find(" "));


        if (command=="exit") break;

        std::string filed=inputted.substr(inputted.find(" ")+1,inputted.size());

        if (command=="comp") {
            std::ifstream BS_file(filed);

            if (BS_file.is_open()) {
                std::stringstream buffer;
                buffer << BS_file.rdbuf();
                std::cout << translator(buffer.str());

            } else {
                std::cout << "This file does not exist!";
            }

            continue;
        }
        
    }

    return 0;
}
