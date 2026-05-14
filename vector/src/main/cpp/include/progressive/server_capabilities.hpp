#ifndef PROGRESSIVE_SERVER_CAPABILITIES_HPP
#define PROGRESSIVE_SERVER_CAPABILITIES_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace progressive {

// ---- HomeServer Capabilities ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.session.homeserver.HomeServerCapabilities.kt (192 lines)
//
// This data class holds all server capabilities discovered via:
//   1. GET /_matrix/client/v3/capabilities
//   2. .well-known/matrix/client (identity server, room versions)
//   3. /auth_metadata discovery (MSC4191 OAuth)
//   4. MSC3824 delegated OIDC
//
// Key methods from original:
//   isFeatureSupported(feature) → RoomCapabilitySupport (UNKNOWN/UNSUPPORTED/SUPPORTED/SUPPORTED_UNSTABLE)
//   isFeatureSupported(feature, byRoomVersion) → Boolean
//   versionOverrideForFeature(feature) → recommended room version
//   getLogoutDeviceURL(deviceId) → OAuth device management URL

enum class RoomCapabilitySupport {
    Unknown,           // server doesn't implement room caps
    Unsupported,       // feature not supported
    Supported,         // stable version supported
    SupportedUnstable  // unstable version supported (dev only)
};

enum class RoomVersionStatus {
    Stable,            // fully released
    Unstable           // experimental/preview
};

struct RoomVersionInfo {
    std::string version;       // e.g. "9", "10"
    RoomVersionStatus status = RoomVersionStatus::Stable;
};

struct FeatureInfo {
    std::string preferred;              // preferred room version for this feature
    std::vector<std::string> support;   // all versions that support this feature
};

struct RoomVersionCapabilities {
    std::string defaultVersion;                  // default room version
    std::vector<RoomVersionInfo> supportedVersion; // available room versions
    std::map<std::string, FeatureInfo> capabilities; // feature → version mapping
};

struct HomeServerCapabilities {
    // Account management
    bool canChangePassword = true;
    bool canChangeDisplayName = true;
    bool canChangeAvatar = true;
    bool canChange3pid = true;
    bool canControlLogoutDevices = false;

    // File upload
    int64_t maxUploadFileSize = -1;  // MAX_UPLOAD_FILE_SIZE_UNKNOWN = -1

    // Identity server
    bool lastVersionIdentityServerSupported = false;
    std::string defaultIdentityServerUrl;

    // Room versions
    RoomVersionCapabilities roomVersions;

    // Threads
    bool canUseThreading = false;
    bool canUseThreadReadReceiptsAndNotifications = false;

    // QR login
    bool canLoginWithQrCode = false;

    // Push notifications
    bool canRemotelyTogglePushNotificationsOfDevices = false;

    // Redactions
    bool canRedactRelatedEvents = false;

    // Authenticated media (Matrix 1.11)
    bool canUseAuthenticatedMedia = false;

    // OAuth / MSC4191
    std::string externalAccountManagementUrl;
    std::vector<std::string> externalAccountManagementSupportedActions;

    // Delegated OIDC / MSC3824
    std::string authenticationIssuer;

    // Wellknown-provided
    bool disableNetworkConstraint = false;
};

// ---- Capability Checking (from HomeServerCapabilities.kt:125-161) ----
// Original Kotlin:
//   fun isFeatureSupported(feature: String): RoomCapabilitySupport {
//       val info = roomVersions.capabilities[feature] ?: return UNSUPPORTED
//       val preferred = info.preferred ?: info.support.lastOrNull()
//       val versionCap = roomVersions.supportedVersion.firstOrNull { it.version == preferred }
//       return when { versionCap?.status == STABLE -> SUPPORTED; else -> SUPPORTED_UNSTABLE }
//   }

RoomCapabilitySupport isFeatureSupported(
    const RoomVersionCapabilities& caps, const std::string& feature);

// Original: fun isFeatureSupported(feature: String, byRoomVersion: String): Boolean
bool isFeatureSupportedByVersion(
    const RoomVersionCapabilities& caps, const std::string& feature, const std::string& roomVersion);

// Original: fun versionOverrideForFeature(feature: String): String?
std::string versionOverrideForFeature(
    const RoomVersionCapabilities& caps, const std::string& feature);

// ---- OAuth Logout URL Builder (from HomeServerCapabilities.kt:171-191) ----
// Original: fun getLogoutDeviceURL(deviceId: String): String?
// Builds URL like: https://account.example.com?action=org.matrix.device_delete&device_id=ABC123
// Falls back through MSC4191 versions: org.matrix.device_delete → org.matrix.session_end → session_end
std::string buildLogoutDeviceUrl(
    const HomeServerCapabilities& caps, const std::string& deviceId);

// ---- Default Capabilities ----
// Get default capabilities for servers that don't report them.
HomeServerCapabilities getDefaultCapabilities();

// Parse capabilities from GET /capabilities JSON response.
HomeServerCapabilities parseCapabilities(const std::string& json);

// ---- Known Room Capabilities (from companion object) ----
constexpr const char* ROOM_CAP_KNOCK = "knock";
constexpr const char* ROOM_CAP_RESTRICTED = "restricted";
constexpr int64_t MAX_UPLOAD_FILE_SIZE_UNKNOWN = -1;

// Check if delegated OIDC auth is enabled.
bool isDelegatedOidcEnabled(const HomeServerCapabilities& caps);

// Format capabilities as JSON.
std::string capabilitiesToJson(const HomeServerCapabilities& caps);

} // namespace progressive

#endif // PROGRESSIVE_SERVER_CAPABILITIES_HPP
