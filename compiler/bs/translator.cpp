//#include "translator.hpp"
#include "to_cpp.cpp"

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>
#include <utility>

//Generate a C++ program from a BS program
std::string translator(std::string program) {

    //Convert the program to a vector of Tokens
    std::vector<std::pair<int,std::string>> tokened=to_token(program+";");

    //Convert the Tokens to a Left-Right Abstract Syntax Tree
    std::vector<std::unique_ptr<ASTNode>> ATS_tree=parser(tokened);

    //Converts the AST to a C++ Program
    std::string cpp_program=to_cpp(std::move(ATS_tree));


    return cpp_program;
}