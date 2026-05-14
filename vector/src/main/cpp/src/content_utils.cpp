#include "progressive/content_utils.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace progressive {

// ---- MXC URL Resolution ----
// Original Kotlin (ContentUrlResolver.kt):
//   class ContentUrlResolver(private val homeServerUrl: String) {
//       fun resolveDownloadUrl(mxc: String): String {
//           val parts = mxc.removePrefix("mxc://").split("/")
//           return "$homeServerUrl/_matrix/media/v3/download/${parts[0]}/${parts[1]}"
//       }
//   }

std::string resolveMxcDownloadUrl(const std::string& mxcUrl, const std::string& homeServerUrl) {
    if (!isMxcUri(mxcUrl)) return mxcUrl; // already an HTTP URL

    auto server = extractMxcServerName(mxcUrl);
    auto mediaId = extractMxcMediaId(mxcUrl);
    if (server.empty() || mediaId.empty()) return mxcUrl;

    // Matrix spec: GET /_matrix/media/v3/download/{serverName}/{mediaId}
    std::string base = homeServerUrl;
    while (!base.empty() && base.back() == '/') base.pop_back();
    return base + "/_matrix/media/v3/download/" + server + "/" + mediaId;
}

std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServerUrl,
    int width, int height, const std::string& method) {
    if (!isMxcUri(mxcUrl)) return mxcUrl;

    auto server = extractMxcServerName(mxcUrl);
    auto mediaId = extractMxcMediaId(mxcUrl);
    if (server.empty() || mediaId.empty()) return mxcUrl;

    std::string base = homeServerUrl;
    while (!base.empty() && base.back() == '/') base.pop_back();

    // Matrix spec: GET /_matrix/media/v3/thumbnail/{serverName}/{mediaId}?w=...&h=...&method=...
    std::ostringstream out;
    out << base << "/_matrix/media/v3/thumbnail/" << server << "/" << mediaId;
    out << "?width=" << width << "&height=" << height << "&method=" << method;
    return out.str();
}

bool isMxcUri(const std::string& url) {
    return url.find("mxc://") == 0;
}

std::string extractMxcServerName(const std::string& mxcUrl) {
    // "mxc://server.name/media_id"
    auto prefix = std::string("mxc://");
    if (mxcUrl.find(prefix) != 0) return "";
    auto start = prefix.size();
    auto slashPos = mxcUrl.find('/', start);
    if (slashPos == std::string::npos) return "";
    return mxcUrl.substr(start, slashPos - start);
}

std::string extractMxcMediaId(const std::string& mxcUrl) {
    auto prefix = std::string("mxc://");
    if (mxcUrl.find(prefix) != 0) return "";
    auto start = prefix.size();
    auto slashPos = mxcUrl.find('/', start);
    if (slashPos == std::string::npos) return "";
    return mxcUrl.substr(slashPos + 1);
}

std::string buildMxcUri(const std::string& serverName, const std::string& mediaId) {
    return "mxc://" + serverName + "/" + mediaId;
}

// ---- Message Content Parsing ----
// Original Kotlin (MessageContent.kt):
//   fun parse(content: Content): MessageContent {
//       val msgtype = content.get("msgtype")?.asString()
//       return when (msgtype) {
//           "m.text" -> TextContent(...)
//           "m.image" -> ImageContent(...)
//           ...
//       }
//   }

MessageType parseMessageType(const std::string& contentJson) {
    // Extract msgtype field:
    // {"msgtype":"m.text","body":"Hello"}
    auto search = "\"msgtype\":\"";
    auto pos = contentJson.find(search);
    if (pos == std::string::npos) {
        search = "\"msgtype\": \"";
        pos = contentJson.find(search);
    }
    if (pos == std::string::npos) return MessageType::Unknown;

    pos += search.size();
    auto end = contentJson.find('"', pos);
    if (end == std::string::npos) return MessageType::Unknown;

    std::string msgtype = contentJson.substr(pos, end - pos);

    if (msgtype == "m.text") return MessageType::Text;
    if (msgtype == "m.notice") return MessageType::Notice;
    if (msgtype == "m.emote") return MessageType::Emote;
    if (msgtype == "m.image") return MessageType::Image;
    if (msgtype == "m.video") return MessageType::Video;
    if (msgtype == "m.audio") return MessageType::Audio;
    if (msgtype == "m.file") return MessageType::File;
    if (msgtype == "m.location") return MessageType::Location;
    if (msgtype == "m.sticker") return MessageType::Sticker;
    if (msgtype.find("m.key.verification") == 0) return MessageType::KeyVerification;

    return MessageType::Unknown;
}

MessageContent parseMessageContent(const std::string& contentJson) {
    MessageContent content;
    content.type = parseMessageType(contentJson);

    // Helper: extract string field
    auto extractStr = [&](const std::string& field) -> std::string {
        auto search = "\"" + field + "\":\"";
        auto pos = contentJson.find(search);
        if (pos == std::string::npos) {
            search = "\"" + field + "\": \"";
            pos = contentJson.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = contentJson.find('"', pos);
        if (end == std::string::npos) return "";
        return contentJson.substr(pos, end - pos);
    };

    // Helper: extract int field
    auto extractInt = [&](const std::string& field) -> int {
        auto search = "\"" + field + "\":";
        auto pos = contentJson.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.size();
        while (pos < contentJson.size() && (contentJson[pos] == ' ' || contentJson[pos] == '\t')) pos++;
        int val = 0;
        while (pos < contentJson.size() && contentJson[pos] >= '0' && contentJson[pos] <= '9') {
            val = val * 10 + (contentJson[pos] - '0');
            pos++;
        }
        return val;
    };

    content.body = extractStr("body");
    content.format = extractStr("format");
    content.formattedBody = extractStr("formatted_body");
    content.filename = extractStr("filename");
    content.mimetype = extractStr("mimetype");
    content.geoUri = extractStr("geo_uri");

    // Extract URL/media info from nested info/url fields
    content.mxcUrl = extractStr("url");

    // Extract thumbnail info
    if (contentJson.find("\"thumbnail_url\"") != std::string::npos) {
        content.hasThumbnail = true;
        content.thumbnailUrl = extractStr("thumbnail_url");
    }

    // Extract dimensions (info.w, info.h) from media events
    if (content.type == MessageType::Image || content.type == MessageType::Video) {
        content.imageWidth = extractInt("w");
        content.imageHeight = extractInt("h");
        if (contentJson.find("\"thumbnail_info\"") != std::string::npos) {
            content.hasThumbnail = true;
        }
    }

    // Size
    content.size = static_cast<int64_t>(extractInt("size"));
    content.durationMs = extractInt("duration");

    // Check for edit (m.relates_to → m.replace)
    if (contentJson.find("\"rel_type\":\"m.replace\"") != std::string::npos ||
        contentJson.find("\"rel_type\": \"m.replace\"") != std::string::npos) {
        content.isEdit = true;
    }

    // Check for reply (m.relates_to → m.in_reply_to)
    if (contentJson.find("\"m.in_reply_to\"") != std::string::npos) {
        content.isReply = true;
    }

    return content;
}

bool supportsThumbnails(MessageType type) {
    return type == MessageType::Image || type == MessageType::Video ||
           type == MessageType::Sticker || type == MessageType::File;
}

bool isInlineDisplayable(MessageType type) {
    return type == MessageType::Image || type == MessageType::Sticker;
}

bool isMediaType(MessageType type) {
    return type == MessageType::Image || type == MessageType::Video ||
           type == MessageType::Audio || type == MessageType::File ||
           type == MessageType::Sticker || type == MessageType::Location;
}

std::string getMessageTypeLabel(MessageType type) {
    switch (type) {
        case MessageType::Text: return "Text Message";
        case MessageType::Notice: return "Notice";
        case MessageType::Emote: return "Emote";
        case MessageType::Image: return "Image";
        case MessageType::Video: return "Video";
        case MessageType::Audio: return "Audio";
        case MessageType::File: return "File";
        case MessageType::Location: return "Location";
        case MessageType::Sticker: return "Sticker";
        case MessageType::KeyVerification: return "Key Verification";
        default: return "Unknown";
    }
}

std::string getExtensionFromMimeType(const std::string& mimetype) {
    if (mimetype == "image/jpeg") return ".jpg";
    if (mimetype == "image/png") return ".png";
    if (mimetype == "image/gif") return ".gif";
    if (mimetype == "image/webp") return ".webp";
    if (mimetype == "image/svg+xml") return ".svg";
    if (mimetype == "video/mp4") return ".mp4";
    if (mimetype == "video/webm") return ".webm";
    if (mimetype == "audio/mpeg") return ".mp3";
    if (mimetype == "audio/ogg") return ".ogg";
    if (mimetype == "audio/wav") return ".wav";
    if (mimetype == "application/pdf") return ".pdf";
    if (mimetype == "application/zip") return ".zip";
    if (mimetype == "text/plain") return ".txt";
    return "";
}

std::string formatFileSize(int64_t bytes) {
    if (bytes <= 0) return "0 B";

    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(1) << size << " " << units[unitIndex];
    return out.str();
}

std::string messageContentToJson(const MessageContent& content) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("type": )" << static_cast<int>(content.type) << ",";
    json << R"("typeLabel": ")" << esc(getMessageTypeLabel(content.type)) << R"(",)";
    json << R"("body": ")" << esc(content.body) << R"(",)";
    json << R"("format": ")" << esc(content.format) << R"(",)";
    json << R"("mxcUrl": ")" << esc(content.mxcUrl) << R"(",)";
    json << R"("mimetype": ")" << esc(content.mimetype) << R"(",)";
    json << R"("filename": ")" << esc(content.filename) << R"(",)";
    json << R"("size": )" << content.size << ",";
    json << R"("imageWidth": )" << content.imageWidth << ",";
    json << R"("imageHeight": )" << content.imageHeight << ",";
    json << R"("durationMs": )" << content.durationMs << ",";
    json << R"("hasThumbnail": )" << (content.hasThumbnail ? "true" : "false") << ",";
    json << R"("isMedia": )" << (isMediaType(content.type) ? "true" : "false") << ",";
    json << R"("isEdit": )" << (content.isEdit ? "true" : "false");
    json << "}";
    return json.str();
}

} // namespace progressive
