#include "progressive/server_capabilities.hpp"
#include <sstream>

namespace progressive {

// ---- Capability Checking (from HomeServerCapabilities.kt:125-143) ----
// Original Kotlin:
//   fun isFeatureSupported(feature: String): RoomCapabilitySupport {
//       if (roomVersions?.capabilities == null) return RoomCapabilitySupport.UNKNOWN
//       val info = roomVersions.capabilities[feature] ?: return RoomCapabilitySupport.UNSUPPORTED
//       val preferred = info.preferred ?: info.support.lastOrNull()
//       val versionCap = roomVersions.supportedVersion.firstOrNull { it.version == preferred }
//       return when {
//           versionCap == null -> UNKNOWN
//           versionCap.status == STABLE -> SUPPORTED
//           else -> SUPPORTED_UNSTABLE
//       }
//   }

RoomCapabilitySupport isFeatureSupported(
    const RoomVersionCapabilities& caps, const std::string& feature)
{
    // Original: capabilities == null → UNKNOWN
    if (caps.capabilities.empty()) return RoomCapabilitySupport::Unknown;

    // Original: capabilities[feature] ?: return UNSUPPORTED
    auto it = caps.capabilities.find(feature);
    if (it == caps.capabilities.end()) return RoomCapabilitySupport::Unsupported;

    const auto& info = it->second;

    // Original: val preferred = info.preferred ?: info.support.lastOrNull()
    std::string preferred = info.preferred;
    if (preferred.empty() && !info.support.empty()) {
        preferred = info.support.back();
    }
    if (preferred.empty()) return RoomCapabilitySupport::Unknown;

    // Original: val versionCap = supportedVersion.firstOrNull { it.version == preferred }
    const RoomVersionInfo* versionCap = nullptr;
    for (const auto& v : caps.supportedVersion) {
        if (v.version == preferred) { versionCap = &v; break; }
    }
    if (!versionCap) return RoomCapabilitySupport::Unknown;

    // Original: versionCap.status == STABLE → SUPPORTED else SUPPORTED_UNSTABLE
    return (versionCap->status == RoomVersionStatus::Stable)
        ? RoomCapabilitySupport::Supported
        : RoomCapabilitySupport::SupportedUnstable;
}

// Original: fun isFeatureSupported(feature: String, byRoomVersion: String): Boolean
//   val info = roomVersions.capabilities[feature] ?: return false
//   return info.preferred == byRoomVersion || info.support.contains(byRoomVersion)

bool isFeatureSupportedByVersion(
    const RoomVersionCapabilities& caps, const std::string& feature, const std::string& roomVersion)
{
    if (caps.capabilities.empty()) return false;

    auto it = caps.capabilities.find(feature);
    if (it == caps.capabilities.end()) return false;

    const auto& info = it->second;
    if (info.preferred == roomVersion) return true;

    for (const auto& v : info.support) {
        if (v == roomVersion) return true;
    }
    return false;
}

// Original: fun versionOverrideForFeature(feature: String): String?
//   val cap = roomVersions?.capabilities?.get(feature)
//   return cap?.preferred ?: cap?.support?.lastOrNull()

std::string versionOverrideForFeature(
    const RoomVersionCapabilities& caps, const std::string& feature)
{
    if (caps.capabilities.empty()) return "";

    auto it = caps.capabilities.find(feature);
    if (it == caps.capabilities.end()) return "";

    const auto& info = it->second;
    if (!info.preferred.empty()) return info.preferred;
    if (!info.support.empty()) return info.support.back();
    return "";
}

// ---- OAuth Logout URL Builder (from HomeServerCapabilities.kt:171-191) ----
// Original: fun getLogoutDeviceURL(deviceId: String): String?
//   if (externalAccountManagementUrl == null) return null
//   var action = "org.matrix.device_delete"
//   externalAccountManagementSupportedActions?.also { actions ->
//       if (actions.contains("org.matrix.device_delete")) { }
//       else if (actions.contains("org.matrix.session_end")) { action = "org.matrix.session_end" }
//       else if (actions.contains("session_end")) { action = "session_end" }
//   }
//   return externalAccountManagementUrl.removeSuffix("/") + "?action=${action}&device_id=${deviceId}"

std::string buildLogoutDeviceUrl(
    const HomeServerCapabilities& caps, const std::string& deviceId)
{
    if (caps.externalAccountManagementUrl.empty() || deviceId.empty()) return "";

    // Original: var action = "org.matrix.device_delete" (default stable value)
    std::string action = "org.matrix.device_delete";

    const auto& actions = caps.externalAccountManagementSupportedActions;
    bool hasDeviceDelete = false, hasSessionEnd = false, hasRawSessionEnd = false;
    for (const auto& a : actions) {
        if (a == "org.matrix.device_delete") hasDeviceDelete = true;
        if (a == "org.matrix.session_end") hasSessionEnd = true;
        if (a == "session_end") hasRawSessionEnd = true;
    }

    if (!actions.empty() && !hasDeviceDelete) {
        if (hasSessionEnd) {
            action = "org.matrix.session_end";    // earlier MSC4191
        } else if (hasRawSessionEnd) {
            action = "session_end";              // previous unspecified
        }
    }

    std::string baseUrl = caps.externalAccountManagementUrl;
    while (!baseUrl.empty() && baseUrl.back() == '/') baseUrl.pop_back();

    // URL-encode device_id (simple version — just alphanum escape)
    std::ostringstream url;
    url << baseUrl << "?action=" << action << "&device_id=" << deviceId;
    return url.str();
}

// ---- Default Capabilities ----
HomeServerCapabilities getDefaultCapabilities() {
    return HomeServerCapabilities{};
}

// ---- Parse from JSON ----
HomeServerCapabilities parseCapabilities(const std::string& json) {
    HomeServerCapabilities caps;

    // Simple JSON extractors
    auto extractBool = [&](const std::string& key, bool defaultVal = false) -> bool {
        auto search = "\"" + key + "\":";
        auto pos = json.find(search);
        if (pos == std::string::npos) return defaultVal;
        pos += search.size();
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        if (json.find("true", pos) == pos) return true;
        return false;
    };

    auto extractStr = [&](const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    caps.canChangePassword = extractBool("m.change_password", true);
    caps.canChangeDisplayName = extractBool("m.change_displayname", true);
    caps.canChangeAvatar = extractBool("m.change_avatar_url", true);
    caps.canChange3pid = extractBool("m.3pid_changes", true);
    caps.canUseThreading = extractBool("m.thread", false);
    caps.canUseAuthenticatedMedia = extractBool("m.authenticated_media", false);
    caps.canLoginWithQrCode = extractBool("m.login_qr", false);
    caps.canRedactRelatedEvents = extractBool("m.redact_related_events", false);
    caps.authenticationIssuer = extractStr("m.authentication_issuer");

    return caps;
}

bool isDelegatedOidcEnabled(const HomeServerCapabilities& caps) {
    // Original: val delegatedOidcAuthEnabled: Boolean = authenticationIssuer != null
    return !caps.authenticationIssuer.empty();
}

std::string capabilitiesToJson(const HomeServerCapabilities& caps) {
    std::ostringstream json;
    json << "{";
    json << R"("canChangePassword": )" << (caps.canChangePassword ? "true" : "false") << ",";
    json << R"("canChangeDisplayName": )" << (caps.canChangeDisplayName ? "true" : "false") << ",";
    json << R"("canChangeAvatar": )" << (caps.canChangeAvatar ? "true" : "false") << ",";
    json << R"("canChange3pid": )" << (caps.canChange3pid ? "true" : "false") << ",";
    json << R"("maxUploadFileSize": )" << caps.maxUploadFileSize << ",";
    json << R"("canUseThreading": )" << (caps.canUseThreading ? "true" : "false") << ",";
    json << R"("canUseThreadReadReceipts": )" << (caps.canUseThreadReadReceiptsAndNotifications ? "true" : "false") << ",";
    json << R"("canLoginWithQrCode": )" << (caps.canLoginWithQrCode ? "true" : "false") << ",";
    json << R"("canUseAuthenticatedMedia": )" << (caps.canUseAuthenticatedMedia ? "true" : "false") << ",";
    json << R"("delegatedOidcEnabled": )" << (isDelegatedOidcEnabled(caps) ? "true" : "false");
    json << "}";
    return json.str();
}

} // namespace progressive
