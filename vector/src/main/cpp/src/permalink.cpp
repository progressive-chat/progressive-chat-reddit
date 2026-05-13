#include "progressive/permalink.hpp"
#include "progressive/url_tools.hpp"
#include <sstream>

namespace progressive {

std::string buildRoomPermalink(const std::string& roomIdOrAlias) {
    return "https://matrix.to/#/" + roomIdOrAlias;
}

std::string buildUserPermalink(const std::string& userId) {
    return "https://matrix.to/#/" + userId;
}

std::string buildEventPermalink(const std::string& roomId, const std::string& eventId) {
    return "https://matrix.to/#/" + roomId + "/" + eventId;
}

std::string buildMatrixSchemeLink(const std::string& type, const std::string& id) {
    return "matrix:" + type + "/" + id;
}

PermalinkInfo parsePermalink(const std::string& url) {
    PermalinkInfo info;
    info.fullUrl = url;

    // Strip https://matrix.to/#/
    std::string prefix = "https://matrix.to/#/";
    if (url.rfind(prefix, 0) != 0) return info;

    auto rest = url.substr(prefix.size());

    if (rest.empty()) return info;

    if (rest[0] == '@') {
        // User permalink
        info.type = "user";
        info.userId = rest;
        info.valid = true;
    } else if (rest[0] == '#') {
        // Room alias permalink
        info.type = "room";
        info.roomAlias = rest;
        info.valid = true;
    } else if (rest[0] == '!') {
        // Room ID permalink, optionally with /$event
        auto slash = rest.find('/');
        if (slash != std::string::npos) {
            info.type = "event";
            info.roomId = rest.substr(0, slash);
            info.eventId = rest.substr(slash + 1);
        } else {
            info.type = "room";
            info.roomId = rest;
        }
        info.valid = true;
    }

    return info;
}

bool isPermalink(const std::string& url) {
    return url.rfind("https://matrix.to/#/", 0) == 0;
}

std::string extractRoomIdFromPermalink(const std::string& url) {
    auto info = parsePermalink(url);
    return info.roomId;
}

std::string extractEventIdFromPermalink(const std::string& url) {
    auto info = parsePermalink(url);
    return info.eventId;
}

std::string extractUserIdFromPermalink(const std::string& url) {
    auto info = parsePermalink(url);
    return info.userId;
}

std::string formatPermalinkForShare(const PermalinkInfo& info) {
    std::ostringstream out;
    if (info.type == "room") {
        out << "Join room: " << info.fullUrl;
    } else if (info.type == "user") {
        out << "Contact: " << info.fullUrl;
    } else if (info.type == "event") {
        out << "Message: " << info.fullUrl;
    }
    return out.str();
}

bool isSameRoomPermalink(const std::string& url1, const std::string& url2) {
    return extractRoomIdFromPermalink(url1) == extractRoomIdFromPermalink(url2);
}

} // namespace progressive
