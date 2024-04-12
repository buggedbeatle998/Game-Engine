#include "util.hpp"

#include <string>
#include <iostream>
#include "token_types.hpp"

using namespace std;

//Hasher
inline constexpr unsigned long hash_djb2a(const string_view sv) {
    unsigned long hash {5381};
    for (unsigned char c : sv) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}
 
//Turns it into a method
inline constexpr unsigned long operator ""_sh(const char* str, size_t len) {
    return hash_djb2a(string_view{str, len});
}

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

        case "array"_sh:
            returner = "vector<any>";
        break;

        case "tuple"_sh:
            returner = "tuple<any>";
        break;

        case "pair"_sh:
            returner = "pair<any,any>";
        break;

        case "scr"_sh:
            returner = "function";
        break;

        case "file"_sh:
            returner = "ifstream";
        break;

        case "ptr"_sh:
            returner = "any*";
        break;

        default:
            throw invalid_argument("Invalid datatype: " + token_str);
        break;
    }
    return returner;
}
