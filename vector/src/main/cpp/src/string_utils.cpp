#include "progressive/string_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

namespace progressive {

std::string sanitizeRoomName(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (c >= 32 && c <= 126) {
            result += c;
        }
    }
    // Collapse multiple spaces
    std::regex multiSpace("  +");
    result = std::regex_replace(result, multiSpace, " ");
    return trim(result);
}

std::string replaceSpaceChars(const std::string& input) {
    std::string result = input;
    // Replace various Unicode space characters with regular space
    std::vector<std::string> spaces = {
        "\u00A0", "\u2000", "\u2001", "\u2002", "\u2003",
        "\u2004", "\u2005", "\u2006", "\u2007", "\u2008",
        "\u2009", "\u200A", "\u202F", "\u205F", "\u3000"
    };
    for (const auto& sp : spaces) {
        size_t pos = 0;
        while ((pos = result.find(sp, pos)) != std::string::npos) {
            result.replace(pos, sp.size(), " ");
            pos++;
        }
    }
    return result;
}

std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::istringstream stream(input);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        if (!token.empty()) result.push_back(token);
    }
    return result;
}

std::string joinStrings(const std::vector<std::string>& parts, const std::string& delimiter) {
    std::ostringstream out;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) out << delimiter;
        out << parts[i];
    }
    return out.str();
}

std::string trim(const std::string& input) {
    auto start = input.begin();
    while (start != input.end() && std::isspace(static_cast<unsigned char>(*start))) ++start;
    auto end = input.end();
    while (end != start && std::isspace(static_cast<unsigned char>(*(end - 1)))) --end;
    return std::string(start, end);
}

bool startsWith(const std::string& s, const std::string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

bool endsWith(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string toUpper(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string replaceAll(const std::string& input, const std::string& from, const std::string& to) {
    if (from.empty()) return input;
    std::string result = input;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.size(), to);
        pos += to.size();
    }
    return result;
}

bool isDigitsOnly(const std::string& input) {
    return !input.empty() && std::all_of(input.begin(), input.end(), ::isdigit);
}

bool isLettersOnly(const std::string& input) {
    return !input.empty() && std::all_of(input.begin(), input.end(), ::isalpha);
}

bool isBlank(const std::string& input) {
    return input.empty() || std::all_of(input.begin(), input.end(), ::isspace);
}

std::string stripHtmlTags(const std::string& input) {
    std::regex tagRe(R"(<[^>]*>)");
    return std::regex_replace(input, tagRe, "");
}

int wordCount(const std::string& input) {
    if (isBlank(input)) return 0;
    std::istringstream stream(input);
    std::string word;
    int count = 0;
    while (stream >> word) count++;
    return count;
}

int estimateReadingTimeSeconds(const std::string& input) {
    int words = wordCount(input);
    // 200 words per minute → words / 200 * 60 seconds
    return std::max(1, (words * 60) / 200);
}

std::string firstNWords(const std::string& input, int n) {
    if (n <= 0) return {};
    std::istringstream stream(input);
    std::string word;
    std::ostringstream result;
    int count = 0;
    while (count < n && stream >> word) {
        if (count > 0) result << " ";
        result << word;
        count++;
    }
    return result.str();
}

} // namespace progressive
