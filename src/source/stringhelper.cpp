#include "stringhelper.h"

std::string StringHelper::int_to_string(int i) { return std::to_string(i); }
std::string StringHelper::uint_to_string(uint ui) { return std::to_string(ui); }
std::string StringHelper::char_to_string(char c) { std::string res(""); res.push_back(c); return res; }


