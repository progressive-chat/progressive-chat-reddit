#ifndef PROGRESSIVE_PERMALINK_HPP
#define PROGRESSIVE_PERMALINK_HPP

#include <string>

namespace progressive {

// ---- Matrix Permalink Utilities ----

struct PermalinkInfo {
    std::string fullUrl;          // https://matrix.to/#/!room:server/$event
    std::string roomId;
    std::string userId;
    std::string eventId;
    std::string roomAlias;
    std::string type;             // "room", "user", "event"
    bool valid = false;
};

// Build a matrix.to permalink for a room.
std::string buildRoomPermalink(const std::string& roomIdOrAlias);

// Build a matrix.to permalink for a user.
std::string buildUserPermalink(const std::string& userId);

// Build a matrix.to permalink for an event.
std::string buildEventPermalink(const std::string& roomId, const std::string& eventId);

// Build a matrix: scheme link (matrix:u/user, matrix:r/room, matrix:e/event).
std::string buildMatrixSchemeLink(const std::string& type, const std::string& id);

// Parse a matrix.to permalink.
PermalinkInfo parsePermalink(const std::string& url);

// Check if a URL is a valid Matrix permalink.
bool isPermalink(const std::string& url);

// Extract room ID from permalink.
std::string extractRoomIdFromPermalink(const std::string& url);

// Extract event ID from permalink.
std::string extractEventIdFromPermalink(const std::string& url);

// Extract user ID from permalink.
std::string extractUserIdFromPermalink(const std::string& url);

// Format a permalink for sharing (short form).
std::string formatPermalinkForShare(const PermalinkInfo& info);

// Check if two permalinks point to the same room.
bool isSameRoomPermalink(const std::string& url1, const std::string& url2);

} // namespace progressive

#endif // PROGRESSIVE_PERMALINK_HPP
