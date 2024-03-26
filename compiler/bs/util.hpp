#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include "token_types.hpp"

inline constexpr unsigned long hash_djb2a(const std::string_view sv);

inline constexpr unsigned long operator"" _sh(const char *str, size_t len);

std::string get_cpptype(Types token_str);

#endif
