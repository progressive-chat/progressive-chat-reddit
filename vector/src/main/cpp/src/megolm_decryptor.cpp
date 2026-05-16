#include "progressive/megolm_decryptor.hpp"
#include <olm/inbound_group_session.h>
#include <olm/olm.h>
#include <cstring>
#include <algorithm>
#include <android/log.h>

#define LOG_TAG "MegolmDecryptor"
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

namespace progressive {

// ==== Base64 Decoder (RFC 4648 standard, no padding) ====

static const char BASE64_CHARS[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::vector<uint8_t> base64Decode(const std::string& input) {
    std::vector<uint8_t> result;
    int val = 0, valb = -8;
    for (char c : input) {
        if (c == '=') break;
        const char* p = strchr(BASE64_CHARS, c);
        if (!p) continue;
        val = (val << 6) + (int)(p - BASE64_CHARS);
        valb += 6;
        if (valb >= 0) {
            result.push_back((uint8_t)((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return result;
}

// ==== Megolm Session ====

static size_t olmGroupSessionSize() {
    return olm_inbound_group_session_size();
}

MegolmSession createInboundMegolmSession(const std::vector<uint8_t>& sessionKey) {
    MegolmSession result;

    size_t size = olm_inbound_group_session_size();
    if (size == 0) {
        LOGW("olm_inbound_group_session_size returned 0");
        return result;
    }

    void* session = malloc(size);
    if (!session) return result;

    auto* olmSession = olm_inbound_group_session(session);
    if (!olmSession) {
        free(session);
        return result;
    }

    // Import the session key
    size_t ret = olm_import_inbound_group_session(
        olmSession, sessionKey.data(), sessionKey.size());
    if (ret == olm_error()) {
        const char* err = olm_inbound_group_session_last_error(olmSession);
        LOGW("olm_import_inbound_group_session failed: %s", err ? err : "unknown");
        olm_clear_inbound_group_session(olmSession);
        free(session);
        return result;
    }

    // Get session ID
    size_t idLen = olm_inbound_group_session_id_length(olmSession);
    std::string sessionId(idLen, '\0');
    olm_inbound_group_session_id(olmSession, &sessionId[0], idLen);
    while (!sessionId.empty() && sessionId.back() == '\0') sessionId.pop_back();

    result.session = session;
    result.sessionId = sessionId;
    result.firstKnownIndex = (uint32_t)olm_inbound_group_session_first_known_index(olmSession);
    result.valid = true;

    return result;
}

void destroyMegolmSession(MegolmSession& session) {
    if (session.session) {
        olm_clear_inbound_group_session(olm_inbound_group_session(session.session));
        free(session.session);
        session.session = nullptr;
    }
    session.valid = false;
}

std::string megolmDecrypt(MegolmSession& session, const std::string& ciphertext) {
    if (!session.valid || !session.session) return "";

    auto* olmSession = olm_inbound_group_session(session.session);
    if (!olmSession) return "";

    // Get max plaintext size
    size_t maxLen = olm_group_decrypt_max_plaintext_length(
        olmSession, reinterpret_cast<const uint8_t*>(ciphertext.data()), ciphertext.size());
    if (maxLen == olm_error()) {
        return ""; // Unknown message index
    }

    // Decrypt
    std::string plaintext(maxLen, '\0');
    uint32_t messageIndex = 0;
    size_t ret = olm_group_decrypt(
        olmSession,
        reinterpret_cast<const uint8_t*>(ciphertext.data()), ciphertext.size(),
        reinterpret_cast<uint8_t*>(&plaintext[0]), maxLen,
        &messageIndex);
    if (ret == olm_error()) {
        const char* err = olm_inbound_group_session_last_error(olmSession);
        LOGW("olm_group_decrypt failed: %s", err ? err : "unknown");
        return "";
    }

    plaintext.resize(ret);
    return plaintext;
}

std::string getMegolmSessionId(const MegolmSession& session) {
    return session.sessionId;
}

std::string exportMegolmSession(const MegolmSession& session) {
    if (!session.valid || !session.session) return "";

    auto* olmSession = olm_inbound_group_session(session.session);
    size_t len = olm_export_inbound_group_session_length(olmSession);
    if (len == olm_error()) return "";

    std::string key(len, '\0');
    size_t ret = olm_export_inbound_group_session(olmSession, &key[0], len);
    if (ret == olm_error()) return "";
    key.resize(ret);
    return key;
}

// ==== Session Manager ====

bool MegolmSessionManager::addSession(const std::string& roomId, const std::string& senderKey,
                                       const std::string& sessionId, const std::string& sessionKeyBase64) {
    // Decode base64
    auto keyBytes = base64Decode(sessionKeyBase64);
    if (keyBytes.empty()) {
        LOGW("addSession: base64 decode failed");
        return false;
    }

    // Create session
    auto session = createInboundMegolmSession(keyBytes);
    if (!session.valid) {
        LOGW("addSession: createInboundMegolmSession failed");
        return false;
    }

    // Store
    SessionKey key{roomId, senderKey, sessionId};
    sessions_[key] = std::move(session);
    return true;
}

MegolmSession* MegolmSessionManager::findSession(const std::string& roomId, const std::string& senderKey,
                                                   const std::string& sessionId) {
    SessionKey key{roomId, senderKey, sessionId};
    auto it = sessions_.find(key);
    return it != sessions_.end() ? &it->second : nullptr;
}

void MegolmSessionManager::clearRoom(const std::string& roomId) {
    auto it = sessions_.begin();
    while (it != sessions_.end()) {
        if (it->first.roomId == roomId) {
            destroyMegolmSession(it->second);
            it = sessions_.erase(it);
        } else {
            ++it;
        }
    }
}

void MegolmSessionManager::clearAll() {
    for (auto& pair : sessions_) {
        destroyMegolmSession(pair.second);
    }
    sessions_.clear();
}

} // namespace progressive
