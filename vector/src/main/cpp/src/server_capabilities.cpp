#include "progressive/server_capabilities.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

ServerCapabilities parseServerCapabilities(const std::string& wellKnownJson) {
    ServerCapabilities caps;

    // Parse room versions
    auto roomVersions = parseJsonStringValue(wellKnownJson, "m.room_versions");
    if (!roomVersions.empty()) {
        // Parse comma-separated or JSON array
        std::istringstream stream(roomVersions);
        std::string version;
        while (std::getline(stream, version, ',')) {
            while (!version.empty() && version.front() == ' ') version.erase(0, 1);
            while (!version.empty() && version.back() == ' ') version.pop_back();
            if (!version.empty()) caps.roomVersions.push_back(version);
        }
    }

    caps.defaultRoomVersion = parseJsonStringValue(wellKnownJson, "m.room_default_version");
    if (caps.defaultRoomVersion.empty()) caps.defaultRoomVersion = "10";

    // Parse registration
    auto regEnabled = parseJsonStringValue(wellKnownJson, "m.registration_enabled");
    caps.registrationEnabled = (regEnabled == "true");

    // Check for email/captcha in registration flows
    auto regFlows = parseJsonStringValue(wellKnownJson, "m.login.flows");
    auto lower = wellKnownJson;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    caps.emailRequired = lower.find("email") != std::string::npos;
    caps.captchaRequired = lower.find("captcha") != std::string::npos ||
                           lower.find("recaptcha") != std::string::npos;

    // Feature support (MSC flags)
    auto unstableFeatures = parseJsonStringValue(wellKnownJson, "m.unstable_features");
    auto featuresLower = unstableFeatures;
    std::transform(featuresLower.begin(), featuresLower.end(), featuresLower.begin(), ::tolower);

    caps.msc3030Supported = featuresLower.find("msc3030") != std::string::npos;
    caps.threadSupport = featuresLower.find("thread") != std::string::npos;
    caps.pollSupport = featuresLower.find("poll") != std::string::npos;
    caps.knockSupport = featuresLower.find("knock") != std::string::npos;
    caps.restrictedRooms = featuresLower.find("restricted") != std::string::npos;

    // Upload size
    auto maxSize = parseJsonStringValue(wellKnownJson, "m.upload.size");
    if (!maxSize.empty()) caps.maxUploadSizeBytes = std::stoll(maxSize);

    return caps;
}

void parseVersionsResponse(ServerCapabilities& caps, const std::string& versionsJson) {
    // Parse /_matrix/client/versions response
    // {"versions": ["v1.1", "v1.2", ...], "unstable_features": {...}}

    auto versions = parseJsonStringValue(versionsJson, "versions");
    if (!versions.empty()) {
        caps.roomVersions.clear();
        std::istringstream stream(versions);
        std::string version;
        while (std::getline(stream, version, ',')) {
            while (!version.empty() && version.front() == ' ') version.erase(0, 1);
            while (!version.empty() && version.back() == ' ') version.pop_back();
            if (!version.empty()) caps.roomVersions.push_back(version);
        }
    }

    caps.matrixVersion = parseJsonStringValue(versionsJson, "v");
}

bool supportsRoomVersion(const ServerCapabilities& caps, const std::string& version) {
    for (const auto& v : caps.roomVersions) {
        if (v == version) return true;
    }
    return false;
}

std::string getRecommendedRoomVersion(const ServerCapabilities& caps) {
    // Prefer default, fall back to latest known stable
    if (!caps.defaultRoomVersion.empty() && supportsRoomVersion(caps, caps.defaultRoomVersion)) {
        return caps.defaultRoomVersion;
    }
    // Try popular versions
    for (const auto& v : {"10", "9", "11", "8", "7", "6"}) {
        if (supportsRoomVersion(caps, v)) return v;
    }
    return caps.roomVersions.empty() ? "10" : caps.roomVersions.back();
}

std::string capabilitiesToJson(const ServerCapabilities& caps) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("serverName": ")" << esc(caps.serverName) << R"(",)";
    json << R"("defaultRoomVersion": ")" << caps.defaultRoomVersion << R"(",)";
    json << R"("msc3030Supported": )" << (caps.msc3030Supported ? "true" : "false") << ",";
    json << R"("threadSupport": )" << (caps.threadSupport ? "true" : "false") << ",";
    json << R"("pollSupport": )" << (caps.pollSupport ? "true" : "false") << ",";
    json << R"("registrationEnabled": )" << (caps.registrationEnabled ? "true" : "false") << ",";
    json << R"("maxUploadSizeBytes": )" << caps.maxUploadSizeBytes;
    json << "}";
    return json.str();
}

std::string capabilitiesToText(const ServerCapabilities& caps) {
    std::ostringstream out;
    out << "Server: " << caps.serverName << "\n";
    out << "Matrix version: " << caps.matrixVersion << "\n";
    out << "Default room version: " << caps.defaultRoomVersion << "\n";
    out << "MSC3030 (jump to date): " << (caps.msc3030Supported ? "Yes" : "No") << "\n";
    out << "Threads: " << (caps.threadSupport ? "Yes" : "No") << "\n";
    out << "Polls: " << (caps.pollSupport ? "Yes" : "No") << "\n";
    out << "Registration open: " << (caps.registrationEnabled ? "Yes" : "No") << "\n";
    out << "Max upload: " << (caps.maxUploadSizeBytes / 1048576) << " MB\n";
    return out.str();
}

} // namespace progressive
