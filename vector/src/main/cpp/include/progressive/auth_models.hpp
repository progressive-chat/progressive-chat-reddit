#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

// ==== Login Flow Types ====
//
// Original Kotlin (LoginFlowTypes.kt:21-31):
//   object LoginFlowTypes { const val PASSWORD = "m.login.password", ... }

namespace LoginFlowTypes {
    constexpr const char* PASSWORD = "m.login.password";
    constexpr const char* OAUTH2 = "m.login.oauth2";
    constexpr const char* EMAIL_CODE = "m.login.email.code";
    constexpr const char* EMAIL_URL = "m.login.email.url";
    constexpr const char* EMAIL_IDENTITY = "m.login.email.identity";
    constexpr const char* MSISDN = "m.login.msisdn";
    constexpr const char* RECAPTCHA = "m.login.recaptcha";
    constexpr const char* DUMMY = "m.login.dummy";
    constexpr const char* TERMS = "m.login.terms";
    constexpr const char* TOKEN = "m.login.token";
    constexpr const char* SSO = "m.login.sso";
}

// ==== Well-Known Discovery ====
//
// Original Kotlin (WellKnown.kt:43-70):
//   data class WellKnown(homeServer, identityServer, integrations,
//       unstableDelegatedAuthConfig, disableNetworkConstraint)
//
// JSON: {"m.homeserver":{"base_url":"..."},"m.identity_server":{"base_url":"..."}}

struct WellKnownBaseConfig {
    std::string baseUrl;             // "base_url" key
};

struct DelegatedAuthConfig {
    std::string issuer;              // OIDC issuer URL
    std::string accountUrl;          // account management URL
};

struct WellKnown {
    WellKnownBaseConfig homeServer;           // "m.homeserver" key
    WellKnownBaseConfig identityServer;       // "m.identity_server" key
    std::string integrationsJson;              // "m.integrations" key — raw JSON
    DelegatedAuthConfig delegatedAuth;        // "org.matrix.msc2965.authentication"
    bool disableNetworkConstraint = false;    // "io.element.disable_network_constraint"
};

// ==== Credentials ====
//
// Original Kotlin (Credentials.kt:33-63):
//   data class Credentials(userId, accessToken, refreshToken, homeServer, deviceId, discoveryInformation)
//
// Response from POST /_matrix/client/r0/login

struct DiscoveryInformation {
    WellKnownBaseConfig homeServer;
    WellKnownBaseConfig identityServer;
};

struct Credentials {
    std::string userId;              // "@user:example.org"
    std::string accessToken;         // "syt_..."
    std::string refreshToken;        // optional
    std::string homeServer;          // deprecated, extract from userId
    std::string deviceId;            // "ABCDEFGH"
    DiscoveryInformation discoveryInfo; // well_known from login response

    bool isValid() const {
        return !userId.empty() && !accessToken.empty();
    }
};

// ==== HomeServer Connection Config ====
//
// Original Kotlin (HomeServerConnectionConfig.kt:38-248):
//   data class HomeServerConnectionConfig(homeServerUri, homeServerUriBase,
//       identityServerUri, allowedFingerprints, shouldPin, tlsVersions, etc.)

struct HomeServerConnectionConfig {
    std::string homeServerUrl;           // user-entered homeserver URL
    std::string homeServerUrlBase;       // actual API base URL (may differ after well-known redirect)
    std::string identityServerUrl;       // optional
    std::string antiVirusServerUrl;      // optional
    std::vector<std::string> allowedFingerprints; // TLS certificate fingerprints
    bool shouldPin = false;              // only allow matching fingerprints
    bool shouldAcceptTlsExtensions = true;
    bool allowHttp = false;              // allow non-HTTPS connections
    bool forceUsageTlsVersions = false;
};

// ==== Session Params ====
//
// Original Kotlin (SessionParams.kt:27-80):
//   data class SessionParams(credentials, homeServerConnectionConfig, isTokenValid, loginType)

enum class LoginType {
    UNKNOWN = 0,
    PASSWORD = 1,
    SSO = 2,
    TOKEN = 3
};

struct SessionParams {
    Credentials credentials;
    HomeServerConnectionConfig homeServerConfig;
    bool isTokenValid = true;
    LoginType loginType = LoginType::UNKNOWN;

    // Shortcuts from original Kotlin
    std::string userId() const { return credentials.userId; }
    std::string deviceId() const { return credentials.deviceId; }
    std::string homeServerUrl() const { return homeServerConfig.homeServerUrl; }
    std::string homeServerUrlBase() const { return homeServerConfig.homeServerUrlBase; }
};

// ==== User Presence ====
//
// Original Kotlin (UserPresence.kt:21-24), (PresenceEnum.kt:22-39):
//   data class UserPresence(lastActiveAgo, statusMessage, isCurrentlyActive, presence)

enum class PresenceEnum {
    ONLINE = 0,      // "online"
    OFFLINE = 1,     // "offline"
    UNAVAILABLE = 2, // "unavailable"
    BUSY = 3         // "busy" (MSC3026)
};
const char* presenceEnumToString(PresenceEnum p);
PresenceEnum presenceEnumFromString(const std::string& s);

struct UserPresence {
    int64_t lastActiveAgo = 0;           // milliseconds since last activity
    std::string statusMessage;           // user-set status message
    bool isCurrentlyActive = false;      // user is currently online
    PresenceEnum presence = PresenceEnum::OFFLINE;
};

// ==== Thread Details ====
//
// Original Kotlin (ThreadDetails.kt:26-35), (ThreadNotificationState.kt:24-38):
//   data class ThreadDetails(isRootThread, numberOfThreads, threadSummarySenderInfo,
//       threadSummaryLatestEvent, lastMessageTimestamp, threadNotificationState, isThread, lastRootThreadEdition)

enum class ThreadNotificationState {
    NO_NEW_MESSAGE = 0,
    NEW_MESSAGE = 1,
    NEW_HIGHLIGHTED_MESSAGE = 2
};

struct ThreadDetails {
    bool isRootThread = false;
    int numberOfThreads = 0;
    std::string threadSummarySenderInfoJson; // SenderInfo serialized
    std::string threadSummaryLatestEventJson; // Event serialized
    int64_t lastMessageTimestamp = 0;
    ThreadNotificationState threadNotificationState = ThreadNotificationState::NO_NEW_MESSAGE;
    bool isThread = false;
    std::string lastRootThreadEdition;     // event ID of last root thread edit
};

// ==== JSON Parsing ====

WellKnown parseWellKnown(const std::string& json);
Credentials parseCredentials(const std::string& json);
SessionParams parseSessionParams(const std::string& json);
UserPresence parseUserPresence(const std::string& json);

std::string credentialsToJson(const Credentials& creds);
std::string sessionParamsToJson(const SessionParams& params);
std::string userPresenceToJson(const UserPresence& presence);

} // namespace progressive
