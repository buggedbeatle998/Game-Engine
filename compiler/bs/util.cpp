#include "util.hpp"

#include <string>
#include <iostream>
#include "token_types.hpp"

using namespace std;

// Converts a Token into a string
string get_cpptype(Types token_str) {
    string returner;
    switch (token_str) {
        case Type_void:
            returner = "void";
        break;

        case Type_auto:
            returner = "auto";
        break;

        case Type_boolean:
            returner = "bool";
        break;

        case Type_real:
            returner = "long double";
        break;

        case Type_string:
            returner = "string";
        break;

        case Type_array:
            returner = "vector<any>";
        break;

        case Type_tuple:
            returner = "tuple<any>";
        break;

        case Type_pair:
            returner = "pair<any,any>";
        break;

        case Type_function:
            returner = "function";
        break;

        case Type_file:
            returner = "ifstream";
        break;

        case Type_pointer:
            returner = "any*";
        break;

        default:
            throw invalid_argument("Invalid datatype: " + token_str);
        break;
    }
    return returner;
}

// Converts a BS Data Type into a Token
Types get_type(string token_str) {
    Types returner;
    switch (hash_djb2a(token_str)) {
        case "void"_sh:
            returner = Type_void;
        break;

        case "auto"_sh:
            returner = Type_real;
        break;

        case "bool"_sh:
            returner = Type_boolean;
        break;

        case "real"_sh:
            returner = Type_real;
        break;

        case "str"_sh:
            returner = Type_string;
        break;

        case "array"_sh:
            returner = Type_array;
        break;

        case "tuple"_sh:
            returner = Type_tuple;
        break;

        case "pair"_sh:
            returner = Type_pair;
        break;

        case "scr"_sh:
            returner = Type_function;
        break;

        case "file"_sh:
            returner = Type_file;
        break;

        case "ptr"_sh:
            returner = Type_pointer;
        break;

        default:
            throw invalid_argument("Invalid datatype: " + token_str);
        break;
    }
    return returner;
}
