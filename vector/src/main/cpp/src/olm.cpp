#include "progressive/olm.hpp"
#include <sstream>
#include <cstring>

// libolm C API headers (from https://gitlab.matrix.org/matrix-org/olm)
#include <olm/olm.h>
#include <olm/outbound_group_session.h>
#include <olm/inbound_group_session.h>
#include <olm/sas.h>

namespace progressive {

// Type aliases for libolm C opaque types
typedef struct OlmAccount OlmAccountC;
typedef struct OlmSession OlmSessionC;
typedef struct OlmOutboundGroupSession OlmOutboundGroupSessionC;
typedef struct OlmInboundGroupSession OlmInboundGroupSessionC;

// ---- Utility ----

std::string generateRandomBytes(int count) {
    std::string result(count, 0);
    for (int i = 0; i < count; ++i) {
        result[i] = static_cast<char>(rand() % 256);
    }
    return result;
}

// ---- Utility ----

std::string generateRandomBytes(int count) {
    std::string result(count, 0);
    // Use libolm's RNG or fallback to system
    for (int i = 0; i < count; ++i) {
        result[i] = static_cast<char>(rand() % 256);
    }
    return result;
}

std::string olmErrorToString(OlmError error) {
    switch (error) {
        case OlmError::None: return "No error";
        case OlmError::NotEnoughRandom: return "Not enough random data";
        case OlmError::OutputBufferTooSmall: return "Output buffer too small";
        case OlmError::BadMessageVersion: return "Bad message version";
        case OlmError::BadMessageFormat: return "Bad message format";
        case OlmError::BadMessageMac: return "Bad message MAC";
        case OlmError::BadMessageKeyId: return "Bad message key ID";
        case OlmError::InvalidBase64: return "Invalid base64";
        case OlmError::BadAccountKey: return "Bad account key";
        case OlmError::UnknownPickleVersion: return "Unknown pickle version";
        case OlmError::Corruption: return "Data corruption";
        case OlmError::SessionNotFound: return "Session not found";
        default: return "Unknown error";
    }
}

std::string formatPickle(const std::string& type, const std::string& pickle) {
    std::ostringstream out;
    out << type << ":" << pickle;
    return out.str();
}

// ---- OlmAccount ----

OlmAccount::OlmAccount() {
    size_t sz = olm_account_size();
    account_ = new uint8_t[sz];
    memset(account_, 0, sz);
    olm_account(account_);
}

OlmAccount::~OlmAccount() {
    olm_clear_account(static_cast<OlmAccountC*>(account_));
    delete[] static_cast<uint8_t*>(account_);
}

OlmAccountResult OlmAccount::create() {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    size_t randLen = olm_create_account_random_length(acc);
    auto random = generateRandomBytes(randLen);
    int rc = olm_create_account(acc, random.data(), random.size());
    if (rc == -1) {
        const char* err;
        olm_account_last_error(acc, &err);
        result.error = OlmError::UnknownError;
        return result;
    }
    result.success = true;
    return result;
}

OlmAccountResult OlmAccount::identityKeys() {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    size_t len = olm_account_identity_keys_length(acc);
    std::string out(len, 0);
    size_t written = olm_account_identity_keys(acc, &out[0], len);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::OutputBufferTooSmall;
        return result;
    }
    out.resize(written);
    result.success = true;
    result.data = out;
    return result;
}

OlmAccountResult OlmAccount::generateOneTimeKeys(int count) {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    size_t randLen = olm_account_generate_one_time_keys_random_length(acc, count);
    auto random = generateRandomBytes(randLen);
    int rc = olm_account_generate_one_time_keys(acc, count, random.data(), random.size());
    if (rc == -1) {
        result.error = OlmError::NotEnoughRandom;
        return result;
    }
    result.success = true;
    return result;
}

OlmAccountResult OlmAccount::sign(const std::string& message) {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    size_t sigLen = olm_account_signature_length(acc);
    std::string sig(sigLen, 0);
    size_t written = olm_account_sign(acc, message.data(), message.size(), &sig[0], sigLen);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::OutputBufferTooSmall;
        return result;
    }
    sig.resize(written);
    result.success = true;
    result.data = sig;
    return result;
}

OlmAccountResult OlmAccount::pickle(const std::string& key) {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    size_t len = olm_pickle_account_length(acc);
    std::string out(len, 0);
    size_t written = olm_pickle_account(acc, key.data(), key.size(), &out[0], len);
    if (written == static_cast<size_t>(-1)) {
        const char* err;
        olm_account_last_error(acc, &err);
        result.error = OlmError::UnknownPickleVersion;
        return result;
    }
    out.resize(written);
    result.success = true;
    result.data = out;
    return result;
}

OlmAccountResult OlmAccount::unpickle(const std::string& key, const std::string& pickle) {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    int rc = olm_unpickle_account(acc, key.data(), key.size(), pickle.data(), pickle.size());
    if (rc == -1) {
        const char* err;
        olm_account_last_error(acc, &err);
        result.error = OlmError::BadAccountKey;
        return result;
    }
    result.success = true;
    return result;
}

OlmAccountResult OlmAccount::ed25519Key() {
    auto keys = identityKeys();
    if (!keys.success) return keys;
    // Parse {"ed25519":"xxxx"} from JSON
    auto pos = keys.data.find("\"ed25519\":\"");
    if (pos == std::string::npos) {
        keys.success = false;
        return keys;
    }
    pos += 12;
    auto end = keys.data.find('"', pos);
    keys.data = keys.data.substr(pos, end - pos);
    return keys;
}

OlmAccountResult OlmAccount::curve25519Key() {
    auto keys = identityKeys();
    if (!keys.success) return keys;
    auto pos = keys.data.find("\"curve25519\":\"");
    if (pos == std::string::npos) {
        keys.success = false;
        return keys;
    }
    pos += 15;
    auto end = keys.data.find('"', pos);
    keys.data = keys.data.substr(pos, end - pos);
    return keys;
}

int OlmAccount::maxOneTimeKeys() {
    auto* acc = static_cast<OlmAccountC*>(account_);
    return olm_account_max_number_of_one_time_keys(acc);
}

// ---- OlmSession ----

OlmSession::OlmSession() {
    size_t sz = olm_session_size();
    session_ = new uint8_t[sz];
    memset(session_, 0, sz);
    olm_session(session_);
}

OlmSession::~OlmSession() {
    olm_clear_session(static_cast<OlmSessionC*>(session_));
    delete[] static_cast<uint8_t*>(session_);
}

OlmSessionResult OlmSession::createOutbound(OlmAccount& account,
    const std::string& theirIdentityKey, const std::string& theirOneTimeKey) {
    OlmSessionResult result;
    auto* sess = static_cast<OlmSessionC*>(session_);
    auto* acc = static_cast<OlmAccountC*>(account.account_);
    size_t randLen = olm_create_outbound_session_random_length(sess);
    auto random = generateRandomBytes(randLen);
    size_t rc = olm_create_outbound_session(sess, acc,
        theirIdentityKey.data(), theirIdentityKey.size(),
        theirOneTimeKey.data(), theirOneTimeKey.size(),
        random.data(), random.size());
    if (rc == static_cast<size_t>(-1)) {
        const char* err;
        olm_session_last_error(sess, &err);
        result.error = OlmError::BadMessageFormat;
        return result;
    }
    result.success = true;
    return result;
}

OlmSessionResult OlmSession::createInbound(OlmAccount& account, const std::string& preKeyMessage) {
    OlmSessionResult result;
    auto* sess = static_cast<OlmSessionC*>(session_);
    auto* acc = static_cast<OlmAccountC*>(account.account_);
    size_t rc = olm_create_inbound_session(sess, acc, (void*)preKeyMessage.data(), preKeyMessage.size());
    if (rc == static_cast<size_t>(-1)) {
        const char* err;
        olm_session_last_error(sess, &err);
        result.error = OlmError::BadMessageFormat;
        return result;
    }
    result.success = true;
    return result;
}

OlmSessionResult OlmSession::createInboundFrom(OlmAccount& account,
    const std::string& theirIdentityKey, const std::string& encryptedMessage) {
    OlmSessionResult result;
    auto* sess = static_cast<OlmSessionC*>(session_);
    auto* acc = static_cast<OlmAccountC*>(account.account_);
    size_t rc = olm_create_inbound_session_from(sess, acc,
        theirIdentityKey.data(), theirIdentityKey.size(),
        (void*)encryptedMessage.data(), encryptedMessage.size());
    if (rc == static_cast<size_t>(-1)) {
        result.error = OlmError::BadMessageFormat;
        return result;
    }
    result.success = true;
    return result;
}

OlmSessionResult OlmSession::encrypt(const std::string& plaintext) {
    OlmSessionResult result;
    auto* sess = static_cast<OlmSessionC*>(session_);
    size_t msgLen = olm_encrypt_message_length(sess, plaintext.size());
    std::string msg(msgLen, 0);
    size_t written = olm_encrypt(sess, plaintext.data(), plaintext.size(), &msg[0], msgLen);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::OutputBufferTooSmall;
        return result;
    }
    msg.resize(written);
    result.success = true;
    result.data = msg;
    result.messageType = olm_encrypt_message_type(sess);
    return result;
}

OlmSessionResult OlmSession::decrypt(const std::string& encryptedMessage, int messageType) {
    OlmSessionResult result;
    auto* sess = static_cast<OlmSessionC*>(session_);
    size_t ptLen = olm_decrypt_max_plaintext_length(sess, messageType,
        encryptedMessage.data(), encryptedMessage.size());
    std::string pt(ptLen, 0);
    size_t written = olm_decrypt(sess, messageType,
        encryptedMessage.data(), encryptedMessage.size(), &pt[0], ptLen);
    if (written == static_cast<size_t>(-1)) {
        const char* err;
        olm_session_last_error(sess, &err);
        result.error = OlmError::BadMessageMac;
        result.data = std::string(err);
        return result;
    }
    pt.resize(written);
    result.success = true;
    result.data = pt;
    return result;
}

bool OlmSession::matchesInbound(const std::string& preKeyMessage) {
    auto* sess = static_cast<OlmSessionC*>(session_);
    return olm_matches_inbound_session(sess, preKeyMessage.data(), preKeyMessage.size()) == 1;
}

// (pickle/unpickle implementations would follow the same pattern)

// ---- Megolm Outbound ----
// (implementation stubs — follow same C API wrapping pattern)

// ---- Megolm Inbound ----
// (implementation stubs)

// ---- SAS ----
// (implementation stubs)

} // namespace progressive
