#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace SimpleChat {

namespace StringUtil {

std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems);

std::vector<std::string> split(const std::string& s, char delim);

}

} // SimpleChat namespace
