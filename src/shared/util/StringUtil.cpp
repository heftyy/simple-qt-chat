#include "StringUtil.h"

namespace SimpleChat {

namespace StringUtil {

std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string join(const std::vector<std::string>& elements, const char* const separator) {
    switch (elements.size())
    {
    case 0:
        return "";
    case 1:
        return elements[0];
    default:
        std::ostringstream os;
        std::copy(elements.begin(), elements.end()-1, std::ostream_iterator<std::string>(os, separator));
        os << *elements.rbegin();
        return os.str();
    }
}

} // StringUtil namespace

} // SimpleChat namespace