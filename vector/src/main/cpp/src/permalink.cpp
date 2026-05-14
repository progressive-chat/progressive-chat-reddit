#include "progressive/permalink.hpp"
#include "progressive/url_tools.hpp"
#include <sstream>
#include <unordered_map>

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

PermalinkResult parsePermalink(const std::string& url) {
    PermalinkResult info;
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

std::string formatPermalinkForShare(const PermalinkResult& info) {
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

// ---- Enhanced Permalink Parser (from PermalinkParser.kt:45-88) ----
// Original Kotlin:
//   fun parse(uri: Uri): PermalinkData {
//       val matrixToUri = MatrixToConverter.convert(uri) ?: return PermalinkData.FallbackLink(uri)
//       val fragment = matrixToUri.toString().substringAfter("#")
//       val safeFragment = fragment.substringBefore('?')
//       val params = safeFragment.split("/").filter { it.isNotEmpty() }.take(2)
//       val decodedIdentifier = decodedParams.getOrNull(0)
//       return when {
//           isUserId(decodedIdentifier) -> UserLink(userId = decodedIdentifier)
//           isRoomId(decodedIdentifier) -> handleRoomIdCase(...)
//           isRoomAlias(decodedIdentifier) -> RoomLink(roomIdOrAlias = ..., isRoomAlias = true)
//           else -> FallbackLink(uri)
//       }
//   }

PermalinkResult parsePermalinkFull(const std::string& url) {
    PermalinkResult result;
    result.fullUrl = url;

    // First try the existing parser
    result = parsePermalink(url);
    if (!result.valid) {
        result.fullUrl = url;
        return result;
    }

    // Extract fragment (everything after #)
    auto hashPos = url.find('#');
    if (hashPos == std::string::npos) return result;
    std::string fragment = url.substr(hashPos + 1);

    // Get safe fragment (before ?)
    auto queryPos = fragment.find('?');
    std::string safeFragment = (queryPos != std::string::npos) ? fragment.substr(0, queryPos) : fragment;

    // Extract via parameters
    result.viaParameters = extractViaParameters(fragment);

    // Check for email invite parameters
    auto emailPos = fragment.find("email=");
    auto signurlPos = fragment.find("signurl=");
    if (emailPos != std::string::npos && signurlPos != std::string::npos) {
        result.isEmailInvite = true;
        // Extract email
        emailPos += 6;
        auto emailEnd = fragment.find('&', emailPos);
        result.email = urlDecode(fragment.substr(emailPos, emailEnd - emailPos));
        // Extract signurl
        signurlPos += 8;
        auto signurlEnd = fragment.find('&', signurlPos);
        result.signUrl = urlDecode(fragment.substr(signurlPos, signurlEnd - signurlPos));
        // Extract other params
        auto extractParam = [&](const std::string& key) -> std::string {
            auto kp = fragment.find(key + "=");
            if (kp == std::string::npos) return "";
            kp += key.size() + 1;
            auto ke = fragment.find('&', kp);
            return urlDecode(fragment.substr(kp, ke - kp));
        };
        result.roomName = extractParam("room_name");
        result.inviterName = extractParam("inviter_name");
        result.roomAvatarUrl = extractParam("room_avatar_url");
        result.roomType = extractParam("room_type");
        result.token = extractParam("token");
        result.privateKey = extractParam("private_key");
    }

    result.isRoomAlias = !result.roomAlias.empty();
    return result;
}

std::vector<std::string> extractViaParameters(const std::string& fragment) {
    std::vector<std::string> vias;
    // Original Kotlin: UrlQuerySanitizer(this).parameterList.filter { it.mParameter == "via" }
    size_t pos = 0;
    while (true) {
        pos = fragment.find("via=", pos);
        if (pos == std::string::npos) break;
        pos += 4;
        auto end = fragment.find('&', pos);
        std::string value = (end != std::string::npos) ? fragment.substr(pos, end - pos) : fragment.substr(pos);
        vias.push_back(urlDecode(value));
        if (end == std::string::npos) break;
        pos = end;
    }
    return vias;
}

bool isEmailInviteLink(const std::string& url) {
    auto hashPos = url.find('#');
    if (hashPos == std::string::npos) return false;
    std::string fragment = url.substr(hashPos + 1);
    return fragment.find("email=") != std::string::npos && fragment.find("signurl=") != std::string::npos;
}

std::string urlDecode(const std::string& encoded) {
    std::string result;
    for (size_t i = 0; i < encoded.size(); ++i) {
        if (encoded[i] == '%' && i + 2 < encoded.size()) {
            char high = encoded[i + 1];
            char low = encoded[i + 2];
            auto hexVal = [](char c) -> int {
                if (c >= '0' && c <= '9') return c - '0';
                if (c >= 'A' && c <= 'F') return c - 'A' + 10;
                if (c >= 'a' && c <= 'f') return c - 'a' + 10;
                return -1;
            };
            int h = hexVal(high);
            int l = hexVal(low);
            if (h >= 0 && l >= 0) {
                result += static_cast<char>((h << 4) | l);
                i += 2;
                continue;
            }
        }
        if (encoded[i] == '+') result += ' ';
        else result += encoded[i];
    }
    return result;
}

// ==== Via Parameter Computation (from ViaParameterFinder.kt:36-64) ====
// Original Kotlin:
//   fun computeViaParams(userId: String, roomId: String, max: Int): List<String> {
//       val userHomeserver = userId.getServerName()
//       return getUserIdsOfJoinedMembers(roomId)
//           .map { it.getServerName() }
//           .groupBy { it }.mapValues { it.value.size }.toMutableMap()
//           .apply { this[userHomeserver] = Int.MAX_VALUE }
//           .let { map -> map.keys.sortedByDescending { map[it] } }
//           .take(max)
//   }

std::vector<std::string> computeViaParams(
    const std::string& myUserId,
    const std::vector<std::string>& memberUserIds,
    int maxServers)
{
    // Extract the current user's server name
    std::string myServer;
    {
        auto colon = myUserId.rfind(':');
        if (colon != std::string::npos) myServer = myUserId.substr(colon + 1);
    }

    // Extract server names from all member MXIDs
    std::vector<std::string> servers;
    for (const auto& uid : memberUserIds) {
        auto colon = uid.rfind(':');
        if (colon != std::string::npos) {
            servers.push_back(uid.substr(colon + 1));
        }
    }

    // Group by server and count members per server
    std::unordered_map<std::string, int> serverCounts;
    for (const auto& srv : servers) {
        serverCounts[srv]++;
    }

    // Original: .apply { this[userHomeserver] = Int.MAX_VALUE }
    // Ensure the user's own server is included (highest priority)
    if (!myServer.empty()) serverCounts[myServer] = INT32_MAX;

    // Sort servers by count (descending)
    std::vector<std::pair<std::string, int>> sorted;
    for (const auto& [srv, count] : serverCounts) {
        sorted.push_back({srv, count});
    }
    std::sort(sorted.begin(), sorted.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    // Take top N
    std::vector<std::string> result;
    for (int i = 0; i < maxServers && i < static_cast<int>(sorted.size()); ++i) {
        result.push_back(sorted[i].first);
    }

    return result;
}

std::string formatViaParamsUrl(const std::vector<std::string>& viaServers) {
    if (viaServers.empty()) return "";

    std::ostringstream out;
    out << "?via=";
    for (size_t i = 0; i < viaServers.size(); ++i) {
        if (i > 0) out << "&via=";
        out << viaServers[i];  // URL-encoding could be added
    }
    return out.str();
}

} // namespace progressive
