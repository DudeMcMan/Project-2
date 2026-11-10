// Utils.h
// Small string helpers the loaders and the menu share: trim, uppercase,
// comment check, splitting a line on '|', and the facility ID check.
// Author: Ryker
#ifndef UTILS_H
#define UTILS_H

#include <cctype>
#include <string>
#include <vector>

namespace Utils {

// Strips whitespace from both ends (including the \r from Windows files).
// Spaces inside the string are left alone.
inline std::string trim(const std::string& s) {
    const char* ws = " \t\r\n\f\v";
    std::string::size_type first = s.find_first_not_of(ws);
    if (first == std::string::npos) return "";
    std::string::size_type last = s.find_last_not_of(ws);
    return s.substr(first, last - first + 1);
}

// Uppercase copy of the string.
inline std::string toUpper(const std::string& s) {
    std::string out = s;
    for (std::string::size_type i = 0; i < out.size(); ++i)
        out[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(out[i])));
    return out;
}

// Lowercase copy of the string.
inline std::string toLower(const std::string& s) {
    std::string out = s;
    for (std::string::size_type i = 0; i < out.size(); ++i)
        out[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(out[i])));
    return out;
}

// Compares two strings ignoring case. It doesn't trim, so do that first for typed input.
inline bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    return toUpper(a) == toUpper(b);
}

// True for an empty line or one that's only whitespace.
inline bool isBlankLine(const std::string& line) {
    return trim(line).empty();
}

// A comment is a line whose first real character is '#'.
// A '#' later in the line (like "Room #5") doesn't count.
inline bool isCommentLine(const std::string& line) {
    std::string t = trim(line);
    return !t.empty() && t[0] == '#';
}

// Splits a line on '|' and trims each piece. Empty pieces are kept so the
// caller can tell a wrong field count from an empty field.
// "F001|A||Lib" gives {"F001", "A", "", "Lib"}.
inline std::vector<std::string> splitFields(const std::string& line, char delim = '|') {
    std::vector<std::string> fields;
    std::string current;
    for (std::string::size_type i = 0; i < line.size(); ++i) {
        if (line[i] == delim) {
            fields.push_back(trim(current));
            current.clear();
        } else {
            current += line[i];
        }
    }
    fields.push_back(trim(current));
    return fields;
}

// Trim and uppercase an ID. Do this before validating or hashing it.
inline std::string normalizeId(const std::string& raw) {
    return toUpper(trim(raw));
}

// Valid means 'F' plus exactly three digits. Pass in an ID that's already
// been normalized.
inline bool isValidFacilityId(const std::string& id) {
    if (id.size() != 4 || id[0] != 'F') return false;
    for (int i = 1; i < 4; ++i)
        if (!std::isdigit(static_cast<unsigned char>(id[i]))) return false;
    return true;
}

}  // namespace Utils

#endif
