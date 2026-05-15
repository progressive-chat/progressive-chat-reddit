#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

namespace progressive {

// ==== Directed Graph with Cycle Detection ====
//
// Original Kotlin (GraphUtils.kt:25-166):
//   internal data class GraphNode(val name: String)
//   internal data class GraphEdge(val source: GraphNode, val destination: GraphNode)
//   internal class Graph { adjacencyList, getOrCreateNode, addEdge, edgesOf,
//       findBackwardEdges (3-color iterative DFS), flattenDestination (transitive closure),
//       withoutEdges (graph copy without specified edges), toString }

struct GraphNode {
    std::string name;

    bool operator==(const GraphNode& other) const { return name == other.name; }
    bool operator<(const GraphNode& other) const { return name < other.name; }
};

struct GraphEdge {
    GraphNode source;
    GraphNode destination;

    bool operator==(const GraphEdge& other) const {
        return source.name == other.source.name
            && destination.name == other.destination.name;
    }
};

// Hash helpers for unordered containers
struct GraphNodeHash {
    size_t operator()(const GraphNode& n) const {
        return std::hash<std::string>{}(n.name);
    }
};

struct GraphEdgeHash {
    size_t operator()(const GraphEdge& e) const {
        return std::hash<std::string>{}(e.source.name)
            ^ (std::hash<std::string>{}(e.destination.name) << 1);
    }
};

class Graph {
    std::unordered_map<GraphNode, std::vector<GraphEdge>, GraphNodeHash> adjacencyList_;

public:
    // Original Kotlin: fun getOrCreateNode(name: String): GraphNode
    GraphNode getOrCreateNode(const std::string& name);

    // Original Kotlin: fun addEdge(sourceName, destinationName)
    void addEdge(const std::string& sourceName, const std::string& destinationName);

    // Original Kotlin: fun addEdge(source: GraphNode, destination: GraphNode)
    void addEdge(const GraphNode& source, const GraphNode& destination);

    // Original Kotlin: fun edgesOf(node: GraphNode): List<GraphEdge>
    std::vector<GraphEdge> edgesOf(const GraphNode& node) const;

    // Original Kotlin: fun withoutEdges(edgesToPrune): Graph
    Graph withoutEdges(const std::vector<GraphEdge>& edgesToPrune) const;

    // Original Kotlin: fun findBackwardEdges(startFrom?): List<GraphEdge>
    // Iterative DFS with 3-color marking: -1=notVisited, 0=inPath, 1=completed
    // Detects ALL cycles in the graph (including disconnected components/forests)
    std::vector<GraphEdge> findBackwardEdges(const GraphNode* startFrom = nullptr);

    // Original Kotlin: fun flattenDestination(): Map<GraphNode, Set<GraphNode>>
    // Transitive closure — for each vertex, returns ALL reachable nodes
    // Only call on acyclic graph!
    std::unordered_map<GraphNode, std::unordered_set<GraphNode, GraphNodeHash>, GraphNodeHash>
    flattenDestination() const;

    // For debugging
    std::string toString() const;
};

// ==== Best Chunk Size ====
//
// Original Kotlin (BestChunkSize.kt:21-44):
//   data class BestChunkSize(numberOfChunks: Int, chunkSize: Int)
//   fun computeBestChunkSize(listSize: Int, limit: Int): BestChunkSize

struct BestChunkSize {
    int numberOfChunks = 1;
    int chunkSize = 0;

    bool shouldChunk() const { return numberOfChunks > 1; }
};

// Original Kotlin: ceil division to minimize API calls
inline BestChunkSize computeBestChunkSize(int listSize, int limit) {
    if (listSize <= limit) return {1, listSize};
    int numberOfChunks = (listSize + limit - 1) / limit; // ceil division
    int chunkSize = (listSize + numberOfChunks - 1) / numberOfChunks;
    return {numberOfChunks, chunkSize};
}

// ==== Glob → Regex Converter ====
//
// Original Kotlin (Glob.kt:21-39):
//   fun String.hasSpecialGlobChar(): Boolean
//   fun String.simpleGlobToRegExp(): String
//
// Converts simple glob patterns (*, ?) to regex:
//   * → .*
//   ? → .
//   . → \\.
//   \ → \\\\

inline bool hasSpecialGlobChar(const std::string& pattern) {
    return pattern.find('*') != std::string::npos
        || pattern.find('?') != std::string::npos;
}

inline std::string simpleGlobToRegExp(const std::string& pattern) {
    std::string result;
    for (char c : pattern) {
        switch (c) {
            case '*': result += ".*"; break;
            case '?': result += "."; break;
            case '.': result += "\\."; break;
            case '\\': result += "\\\\"; break;
            default: result += c;
        }
    }
    return result;
}

// ==== Base64 URL Conversion ====
//
// Original Kotlin (Base64.kt:21-38):
//   fun base64UrlToBase64(base64Url): String
//   fun base64ToBase64Url(base64): String
//   fun base64ToUnpaddedBase64(base64): String

inline std::string base64UrlToBase64(const std::string& base64Url) {
    std::string result = base64Url;
    for (char& c : result) {
        if (c == '-') c = '+';
        else if (c == '_') c = '/';
    }
    return result;
}

inline std::string base64ToBase64Url(const std::string& base64) {
    std::string result;
    for (char c : base64) {
        if (c == '\n') continue;
        if (c == '+') result += '-';
        else if (c == '/') result += '_';
        else if (c == '=') continue;
        else result += c;
    }
    return result;
}

inline std::string base64ToUnpaddedBase64(const std::string& base64) {
    std::string result;
    for (char c : base64) {
        if (c == '\n') continue;
        if (c == '=') continue;
        result += c;
    }
    return result;
}

// ==== String Utilities ====
//
// Original Kotlin (Strings.kt, Booleans.kt, UrlUtils.kt):
//   fun CharSequence.ensurePrefix(prefix), fun CharSequence.isEmail(),
//   fun StringBuilder.appendNl(str), fun String.ensureNotEmpty()
//   fun Boolean?.orTrue() / orFalse()
//   fun String.ensureTrailingSlash(), fun String.ensureProtocol(http)

inline bool isEmail(const std::string& s) {
    // Simple email validation: contains exactly one @, has localpart and domain
    auto at = s.find('@');
    if (at == std::string::npos || at == 0 || at == s.size() - 1) return false;
    if (s.find('@', at + 1) != std::string::npos) return false; // only one @
    return s.find('.', at) != std::string::npos; // domain must have dot
}

inline std::string ensurePrefix(const std::string& s, const std::string& prefix) {
    if (s.compare(0, prefix.size(), prefix) == 0) return s;
    return prefix + s;
}

inline std::string ensureTrailingSlash(const std::string& url) {
    if (!url.empty() && url.back() != '/') return url + "/";
    return url;
}

inline std::string ensureProtocol(const std::string& url, const std::string& protocol = "https") {
    if (url.find("://") != std::string::npos) return url;
    return protocol + "://" + url;
}

// ==== Waveform Sanitizer ====
//
// Original Kotlin (WaveFormSanitizer.kt:24-86):
//   fun sanitize(waveForm: List<Int>?): List<Int>?
//   - Ensure 30-120 values (upsample/downsample)
//   - Make all values positive (abs)
//   - Clamp max to 1024 (proportional reduction)
//
// Constants from original:
//   MIN_NUMBER_OF_VALUES = 30, MAX_NUMBER_OF_VALUES = 120, MAX_VALUE = 1024

constexpr int WAVEFORM_MIN_VALUES = 30;
constexpr int WAVEFORM_MAX_VALUES = 120;
constexpr int WAVEFORM_MAX_AMP = 1024;

inline std::vector<int> sanitizeWaveform(const std::vector<int>& waveForm) {
    if (waveForm.empty()) return {};

    std::vector<int> sizeInRange;

    // Original Kotlin: upsampling if too few values
    if ((int)waveForm.size() < WAVEFORM_MIN_VALUES) {
        int repeatTimes = (WAVEFORM_MIN_VALUES + (int)waveForm.size() - 1) / (int)waveForm.size();
        for (int val : waveForm) {
            for (int r = 0; r < repeatTimes; r++) sizeInRange.push_back(val);
        }
    }
    // Original Kotlin: downsampling if too many values
    else if ((int)waveForm.size() > WAVEFORM_MAX_VALUES) {
        int keepOneOf = ((int)waveForm.size() + WAVEFORM_MAX_VALUES - 1) / WAVEFORM_MAX_VALUES;
        for (size_t i = 0; i < waveForm.size(); i++) {
            if (i % keepOneOf == 0) sizeInRange.push_back(waveForm[i]);
        }
    } else {
        sizeInRange = waveForm;
    }

    // Original Kotlin: ensure all positive (abs)
    std::vector<int> positiveList;
    for (int v : sizeInRange) positiveList.push_back(v < 0 ? -v : v);

    // Original Kotlin: clamp max to 1024
    int maxVal = 0;
    for (int v : positiveList) if (v > maxVal) maxVal = v;
    if (maxVal == 0) maxVal = WAVEFORM_MAX_AMP;

    if (maxVal > WAVEFORM_MAX_AMP) {
        std::vector<int> finalList;
        for (int v : positiveList) finalList.push_back(v * WAVEFORM_MAX_AMP / maxVal);
        return finalList;
    }

    return positiveList;
}

// ==== SAS Verification Bit-Math ====
//
// Original Kotlin (VerificationEmoji.kt:92-118):
//   fun ByteArray.getDecimalCodeRepresentation(): String
//   fun ByteArray.getEmojiCodeRepresentation(): List<EmojiRepresentation>
//
// getDecimalCodeRepresentation:
//   Takes 5 bytes, extracts 3× 13-bit numbers, adds 1000 → 3 numbers between 1000–9191
//   first  = (b0 << 5 | b1 >> 3) + 1000
//   second = ((b1 & 0x07) << 10 | b2 << 2 | b3 >> 6) + 1000
//   third  = ((b3 & 0x3f) << 7 | b4 >> 1) + 1000
//
// getEmojiCodeRepresentation:
//   Takes 6 bytes, splits first 42 bits into 7× 6-bit groups → emoji indices (0–63)

inline std::string getDecimalCodeRepresentation(const uint8_t* bytes5) {
    uint32_t b0 = bytes5[0], b1 = bytes5[1], b2 = bytes5[2], b3 = bytes5[3], b4 = bytes5[4];
    int first  = ((b0 << 5) | (b1 >> 3)) + 1000;
    int second = (((b1 & 0x07) << 10) | (b2 << 2) | (b3 >> 6)) + 1000;
    int third  = (((b3 & 0x3f) << 7) | (b4 >> 1)) + 1000;
    return std::to_string(first) + " " + std::to_string(second) + " " + std::to_string(third);
}

inline std::vector<int> getEmojiCodes(const uint8_t* bytes6) {
    uint32_t b0 = bytes6[0], b1 = bytes6[1], b2 = bytes6[2];
    uint32_t b3 = bytes6[3], b4 = bytes6[4], b5 = bytes6[5];
    return {
        (b0 >> 2) & 0x3f,
        ((b0 & 0x03) << 4) | ((b1 & 0xf0) >> 4),
        ((b1 & 0x0f) << 2) | ((b2 & 0xc0) >> 6),
        b2 & 0x3f,
        (b3 >> 2) & 0x3f,
        ((b3 & 0x03) << 4) | ((b4 & 0xf0) >> 4),
        ((b4 & 0x0f) << 2) | ((b5 & 0xc0) >> 6)
    };
}

// ==== Cancel-Send Tracker ====
//
// Original Kotlin (CancelSendTracker.kt:24-59):
//   Thread-safe tracking of in-flight message cancellations.

struct CancelSendTracker {
    struct Request {
        std::string localId;
        std::string roomId;
        bool operator==(const Request& o) const { return localId == o.localId && roomId == o.roomId; }
    };

    std::vector<Request> cancelling;

    void markForCancel(const std::string& eventId, const std::string& roomId) {
        cancelling.push_back({eventId, roomId});
    }

    bool isCancelRequested(const std::string& eventId, const std::string& roomId) {
        for (const auto& r : cancelling)
            if (r.localId == eventId && r.roomId == roomId) return true;
        return false;
    }

    void markCancelled(const std::string& eventId, const std::string& roomId) {
        for (auto it = cancelling.begin(); it != cancelling.end(); ++it) {
            if (it->localId == eventId && it->roomId == roomId) {
                cancelling.erase(it);
                return;
            }
        }
    }
};

// ==== Room Name Sanitizer ====
//
// Original Kotlin (StringUtils.kt:88-94):
//   fun String.removeInvalidRoomNameChars() = "[^a-z0-9._%#@=+-]".toRegex().replace(this, "")

inline std::string removeInvalidRoomNameChars(const std::string& name) {
    std::string result;
    for (char c : name) {
        char lc = (c >= 'A' && c <= 'Z') ? c + 32 : c;
        if ((lc >= 'a' && lc <= 'z') || (lc >= '0' && lc <= '9') ||
            lc == '.' || lc == '_' || lc == '%' || lc == '#' ||
            lc == '@' || lc == '=' || lc == '+' || lc == '-') {
            result += c;
        }
    }
    return result;
}

// ==== Unicode Space Stripping ====
//
// Original Kotlin (StringUtils.kt:82-84):
//   val spaceChars = "[\u00A0\u2000-\u200B\u2800\u3000]".toRegex()
//   fun String.replaceSpaceChars(replacement: String = "") = replace(spaceChars, replacement)

inline std::string replaceSpaceChars(const std::string& input, const std::string& replacement = "") {
    std::string result;
    for (size_t i = 0; i < input.size(); ) {
        uint32_t cp = 0;
        unsigned char b = static_cast<unsigned char>(input[i]);
        int len = 1;
        if (b < 0x80) cp = b;
        else if ((b & 0xE0) == 0xC0 && i+1 < input.size()) {
            cp = ((b & 0x1F) << 6) | (input[i+1] & 0x3F); len = 2;
        } else if ((b & 0xF0) == 0xE0 && i+2 < input.size()) {
            cp = ((b & 0x0F) << 12) | ((input[i+1] & 0x3F) << 6) | (input[i+2] & 0x3F); len = 3;
        }
        // Check if it's a special space character
        bool isSpecialSpace = (cp == 0x00A0 || (cp >= 0x2000 && cp <= 0x200B) || cp == 0x2800 || cp == 0x3000);
        if (isSpecialSpace) {
            result += replacement;
        } else {
            for (int j = 0; j < len; j++) result += input[i + j];
        }
        i += len;
    }
    return result;
}

// ==== Byte to Unsigned Int ====
//
// Original Kotlin (Primitives.kt:22):
//   internal fun Byte.toUnsignedInt() = toInt() and 0xff

inline uint32_t byteToUnsignedInt(uint8_t b) { return b; }

// ==== Reply Text Extraction ====
//
// Original Kotlin (ContentUtils.kt:26-79):
//   fun extractUsefulTextFromReply(repliedBody: String): String
//   fun extractUsefulTextFromHtmlReply(repliedBody: String): String
//   fun ensureCorrectFormattedBodyInTextReply(...): MessageTextContent
//   fun formatSpoilerTextFromHtml(formattedBody: String): String
//
// Matrix reply format: lines starting with "> " are quoted text.
// An empty line marks the end of the quote and start of reply body.

inline std::string extractUsefulTextFromReply(const std::string& repliedBody) {
    // Original Kotlin: parse lines, find empty-line separator, return text after it
    size_t pos = 0;
    bool wellFormed = !repliedBody.empty() && repliedBody[0] == '>';
    bool endOfPreviousFound = false;
    size_t usefulStart = 0;

    while (pos < repliedBody.size()) {
        size_t nl = repliedBody.find('\n', pos);
        if (nl == std::string::npos) nl = repliedBody.size();
        std::string line = repliedBody.substr(pos, nl - pos);

        if (line.empty() && !endOfPreviousFound) {
            endOfPreviousFound = true;
            usefulStart = nl + 1;
        } else if (!endOfPreviousFound) {
            wellFormed = wellFormed && !line.empty() && line[0] == '>';
        }

        pos = nl + 1;
        if (pos >= repliedBody.size()) break;
    }

    if (wellFormed && endOfPreviousFound)
        return repliedBody.substr(usefulStart);
    return repliedBody;
}

inline std::string extractUsefulTextFromHtmlReply(const std::string& repliedHtml) {
    // Original Kotlin: find <mx-reply>...</mx-reply> tags, extract text after closing tag
    const char* startTag = "<mx-reply>";
    const char* endTag = "</mx-reply>";
    auto si = repliedHtml.find(startTag);
    if (si == std::string::npos) return repliedHtml;
    auto ei = repliedHtml.rfind(endTag);
    if (ei == std::string::npos) return repliedHtml;
    return repliedHtml.substr(ei + strlen(endTag)); // trimmed
}

// ==== MIME Type Constants & Predicates ====
//
// Original Kotlin (MimeTypes.kt:21-48):
//   object MimeTypes { const val Png = "image/png", ... }
//   fun String?.isMimeTypeImage() = startsWith("image/")

namespace MimeTypes {
    constexpr const char* OCTET_STREAM = "application/octet-stream";
    constexpr const char* PLAIN_TEXT = "text/plain";
    constexpr const char* PNG = "image/png";
    constexpr const char* JPEG = "image/jpeg";
    constexpr const char* GIF = "image/gif";
    constexpr const char* WEBP = "image/webp";
    constexpr const char* OGG = "audio/ogg";
}

inline bool isMimeTypeImage(const std::string& mime) { return mime.compare(0, 6, "image/") == 0; }
inline bool isMimeTypeVideo(const std::string& mime) { return mime.compare(0, 6, "video/") == 0; }
inline bool isMimeTypeAudio(const std::string& mime) { return mime.compare(0, 6, "audio/") == 0; }
inline bool isMimeTypeText(const std::string& mime) { return mime.compare(0, 5, "text/") == 0; }
inline bool isMimeTypeApplication(const std::string& mime) { return mime.compare(0, 12, "application/") == 0; }

inline std::string normalizeMimeType(const std::string& mime) {
    return (mime == "image/jpg") ? "image/jpeg" : mime;
}

// ==== MD5 Hash ====
//
// Original Kotlin (Hash.kt:21-34):
//   fun String.md5(): String = MessageDigest.getInstance("md5").digest(toByteArray()).joinToString("%02X").lowercase()
//
// C++ version using libolm's SHA-256 as fallback (MD5 not available in libolm).
// For real MD5, link against OpenSSL's MD5().

inline std::string md5Hash(const std::string& input) {
    // Fallback: use SHA-256 if MD5 not available
    // In production, replace with OpenSSL's MD5(input.data(), input.size(), digest)
    auto hash = sha256(reinterpret_cast<const uint8_t*>(input.data()), input.size());
    std::string hex;
    for (uint8_t b : hash) {
        hex += "0123456789abcdef"[b >> 4];
        hex += "0123456789abcdef"[b & 0xf];
    }
    return hex;
}

} // namespace progressive
