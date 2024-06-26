#include "translator.hpp"

#include "token_types.hpp"
#include "to_token.hpp"
#include "parser.hpp"
#include "to_cpp.hpp"

#include <string>
#include <vector>
#include <memory>
#include <utility>

//Generate a C++ program from a BS program
string translator(string program) {

    //Convert the program to a vector of Tokens
    Token_vector tokened = to_token(program + ";");

    //Convert the Tokens to a Left-Right Abstract Syntax Tree
    AST_vector ATS_tree = parser(tokened);

    //Converts the AST to a C++ Program
    string cpp_program = to_cpp(move(ATS_tree));


    return cpp_program;
}
