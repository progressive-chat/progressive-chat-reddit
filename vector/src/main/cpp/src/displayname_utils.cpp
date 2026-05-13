#include "progressive/displayname_utils.hpp"
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <unordered_set>

namespace progressive {

std::string userIdToDisplayName(const std::string& userId, bool capitalize) {
    // Extract localpart: @alice_johnson:matrix.org → alice_johnson
    if (userId.empty() || userId[0] != '@') return userId;
    auto colon = userId.find(':');
    std::string localpart = (colon != std::string::npos)
        ? userId.substr(1, colon - 1) : userId.substr(1);

    if (!capitalize) return localpart;

    // Replace delimiters with spaces and capitalize words
    std::string result;
    bool newWord = true;
    for (char c : localpart) {
        if (c == '_' || c == '.' || c == '-') {
            result += ' ';
            newWord = true;
        } else if (newWord) {
            result += std::toupper(static_cast<unsigned char>(c));
            newWord = false;
        } else {
            result += std::tolower(static_cast<unsigned char>(c));
        }
    }
    return result;
}

std::string emailToDisplayName(const std::string& email) {
    if (email.empty()) return {};
    auto at = email.find('@');
    if (at == std::string::npos) return email;

    std::string localpart = email.substr(0, at);
    std::string result;
    bool newWord = true;
    for (char c : localpart) {
        if (c == '.' || c == '_' || c == '-') {
            result += ' ';
            newWord = true;
        } else if (newWord) {
            result += std::toupper(static_cast<unsigned char>(c));
            newWord = false;
        } else {
            result += c;
        }
    }
    return result;
}

std::string userIdToColor(const std::string& userId) {
    return stringToColor(userId);
}

std::string stringToColor(const std::string& input) {
    // Simple deterministic hash
    uint32_t hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }

    // Use HSL with fixed saturation and lightness for readable colors
    double hue = (hash % 360);
    double s = 0.65;
    double l = 0.55;

    // HSL to RGB
    double c = (1.0 - std::abs(2.0 * l - 1.0)) * s;
    double x = c * (1.0 - std::abs(std::fmod(hue / 60.0, 2.0) - 1.0));
    double m = l - c / 2.0;

    double r, g, b;
    if (hue < 60)      { r = c; g = x; b = 0; }
    else if (hue < 120) { r = x; g = c; b = 0; }
    else if (hue < 180) { r = 0; g = c; b = x; }
    else if (hue < 240) { r = 0; g = x; b = c; }
    else if (hue < 300) { r = x; g = 0; b = c; }
    else                { r = c; g = 0; b = x; }

    auto toHex = [m](double v) -> int { return static_cast<int>((v + m) * 255); };

    std::ostringstream out;
    out << "#";
    out << std::hex << std::setfill('0') << std::setw(2) << toHex(r);
    out << std::setw(2) << toHex(g);
    out << std::setw(2) << toHex(b);
    return out.str();
}

std::string getFirstLetter(const std::string& name) {
    for (char c : name) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            return std::string(1, std::toupper(static_cast<unsigned char>(c)));
        }
    }
    return "?";
}

std::string getInitials(const std::string& name, int maxChars) {
    std::string result;
    bool takeNext = true;
    for (char c : name) {
        if (takeNext && !std::isspace(static_cast<unsigned char>(c))) {
            result += std::toupper(static_cast<unsigned char>(c));
            takeNext = false;
            if (static_cast<int>(result.size()) >= maxChars) break;
        }
        if (std::isspace(static_cast<unsigned char>(c)) || c == '_' || c == '.') {
            takeNext = true;
        }
    }
    if (result.empty()) result = "?";
    return result;
}

bool needsDisambiguation(const std::string& name, const std::vector<std::string>& existingNames) {
    auto lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    int count = 0;
    for (const auto& n : existingNames) {
        auto lowerN = n;
        std::transform(lowerN.begin(), lowerN.end(), lowerN.begin(), ::tolower);
        if (lowerN == lowerName) count++;
    }
    return count >= 2;
}

std::string disambiguateName(const std::string& name, const std::string& userId) {
    return name + " (" + userId + ")";
}

std::string formatMemberName(const std::string& displayName, const std::string& userId,
    int powerLevel, bool showPowerBadge) {
    std::ostringstream out;
    out << displayName;
    if (showPowerBadge && powerLevel >= 100) out << " ★";
    else if (showPowerBadge && powerLevel >= 50) out << " ☆";

    if (displayName.empty() && !userId.empty()) {
        out << userIdToDisplayName(userId);
    }
    return out.str();
}

bool namesMatch(const std::string& a, const std::string& b) {
    auto lowerA = a, lowerB = b;
    // Trim
    auto trim = [](std::string& s) {
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.erase(0, 1);
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
    };
    trim(lowerA); trim(lowerB);
    std::transform(lowerA.begin(), lowerA.end(), lowerA.begin(), ::tolower);
    std::transform(lowerB.begin(), lowerB.end(), lowerB.begin(), ::tolower);
    return lowerA == lowerB;
}

std::string getBestDisplayName(const std::string& displayName, const std::string& userId) {
    if (!displayName.empty()) return displayName;
    if (!userId.empty()) return userIdToDisplayName(userId);
    return "Unknown";
}

} // namespace progressive
