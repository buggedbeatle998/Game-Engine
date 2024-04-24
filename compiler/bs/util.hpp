#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include "token_types.hpp"

using namespace std;

// Hasher
inline constexpr unsigned long hash_djb2a(const string_view sv) {
    unsigned long hash {5381};
    for (unsigned char c : sv) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}

// Turns it into a method
inline constexpr unsigned long operator ""_sh(const char* str, size_t len) {
    return hash_djb2a(string_view{str, len});
}

string get_cpptype(Types token_str);

Types get_type(string token_str);

#endif
