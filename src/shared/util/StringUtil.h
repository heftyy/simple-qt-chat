#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

namespace SimpleChat {

namespace StringUtil {

std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems);

std::vector<std::string> split(const std::string& s, char delim);

std::string join(const std::vector<std::string>& elements, const char* const separator);

}

} // SimpleChat namespace
