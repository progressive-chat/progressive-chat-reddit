#ifndef PROGRESSIVE_SERVER_CAPABILITIES_HPP
#define PROGRESSIVE_SERVER_CAPABILITIES_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

// ---- Homeserver Capabilities ----

struct ServerCapabilities {
    std::string serverName;

    // Room versions supported
    std::vector<std::string> roomVersions;
    std::string defaultRoomVersion;

    // Features
    bool msc3030Supported = false;   // /timestamp_to_event
    bool threadSupport = false;       // m.thread
    bool pollSupport = false;         // m.poll
    bool locationSupport = false;     // m.location
    bool knockSupport = false;        // m.join_rules knock
    bool restrictedRooms = false;     // m.join_rules restricted
    bool e2eeDefault = false;         // default encryption

    // Registration
    bool registrationEnabled = false;
    bool emailRequired = false;
    bool captchaRequired = false;

    // Media
    int64_t maxUploadSizeBytes = 50 * 1024 * 1024; // 50MB

    // Versions
    std::string serverVersion;
    std::string matrixVersion;
};

// Parse capabilities from /.well-known/matrix/client response.
ServerCapabilities parseServerCapabilities(const std::string& wellKnownJson);

// Parse capabilities from /_matrix/client/versions response.
void parseVersionsResponse(ServerCapabilities& caps, const std::string& versionsJson);

// Check if a specific room version is supported.
bool supportsRoomVersion(const ServerCapabilities& caps, const std::string& version);

// Get recommended room version for a given set of features.
std::string getRecommendedRoomVersion(const ServerCapabilities& caps);

// Format capabilities as JSON for settings display.
std::string capabilitiesToJson(const ServerCapabilities& caps);

// Format capabilities as human-readable text.
std::string capabilitiesToText(const ServerCapabilities& caps);

} // namespace progressive

#endif // PROGRESSIVE_SERVER_CAPABILITIES_HPP
