#ifndef PROGRESSIVE_CONTENT_UTILS_HPP
#define PROGRESSIVE_CONTENT_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Matrix Content Utilities ----
// Ported from: org.matrix.android.sdk.api.session.content.ContentUrlResolver.kt
//              org.matrix.android.sdk.api.session.events.model.Content.kt
//              org.matrix.android.sdk.api.session.room.model.message.MessageType.kt
//              im.vector.app.features.home.room.detail.timeline.helper.ContentDownloadStateBinder.kt

// ---- MXC URL Resolution ----

// Resolve an MXC URI to a full HTTP download URL.
// "mxc://matrix.org/ABCDEFGHIJ" → "https://matrix.org/_matrix/media/v3/download/matrix.org/ABCDEFGHIJ"
// Original Kotlin (ContentUrlResolver.kt):
//   fun resolveDownloadUrl(mxcUrl: String): String?
std::string resolveMxcDownloadUrl(const std::string& mxcUrl, const std::string& homeServerUrl);

// Resolve an MXC URI to a thumbnail URL with given dimensions and method.
// method: "scale" or "crop"
std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServerUrl,
    int width, int height, const std::string& method = "scale");

// Check if a string is a valid MXC URI.
// "mxc://matrix.org/ABC123" → true
// "https://example.com/file.jpg" → false
bool isMxcUri(const std::string& url);

// Extract the server name from an MXC URI.
// "mxc://matrix.org/ABC123" → "matrix.org"
std::string extractMxcServerName(const std::string& mxcUrl);

// Extract the media ID from an MXC URI.
// "mxc://matrix.org/ABC123" → "ABC123"
std::string extractMxcMediaId(const std::string& mxcUrl);

// Build an MXC URI from server name and media ID.
// ("matrix.org", "ABC123") → "mxc://matrix.org/ABC123"
std::string buildMxcUri(const std::string& serverName, const std::string& mediaId);

// ---- Message Content Type Detection ----

enum class MessageType {
    Unknown,
    Text,            // m.text
    Notice,          // m.notice
    Emote,           // m.emote
    Image,           // m.image
    Video,           // m.video
    Audio,           // m.audio
    File,            // m.file
    Location,        // m.location
    Sticker,         // m.sticker (Element-specific)
    KeyVerification, // m.key.verification.request etc
};

struct MessageContent {
    MessageType type = MessageType::Unknown;
    std::string body;              // m.text body, m.image caption, etc.
    std::string format;            // "org.matrix.custom.html" for formatted text
    std::string formattedBody;     // HTML body
    std::string mxcUrl;            // m.image/m.video/m.audio/m.file URL
    std::string thumbnailUrl;      // m.image thumbnail_info thumbnail_url
    std::string filename;          // m.file filename
    std::string mimetype;          // content-type of media
    int64_t size = 0;              // byte size of media file
    int imageWidth = 0;            // m.image info.w
    int imageHeight = 0;           // m.image info.h
    int durationMs = 0;            // m.audio/m.video duration
    bool hasThumbnail = false;     // has thumbnail_url in info
    std::string geoUri;            // m.location geo_uri

    // Edits
    bool isEdit = false;           // m.relates_to.rel_type == "m.replace"
    std::string editEventId;       // event being edited

    // Replies
    bool isReply = false;          // m.relates_to.m.in_reply_to
    std::string replyEventId;      // event being replied to
};

// Parse the content field of a Matrix event to detect message type.
// Original Kotlin (MessageContent.kt):
//   fun parse(content: Content): MessageContent {
//       val msgtype = content.get("msgtype")?.asString()
//       return when (msgtype) { ... }
//   }
MessageType parseMessageType(const std::string& contentJson);

// Parse the full content JSON into a MessageContent struct.
MessageContent parseMessageContent(const std::string& contentJson);

// Check if a message type supports thumbnails.
bool supportsThumbnails(MessageType type);

// Check if a message type supports inline display (images, stickers).
bool isInlineDisplayable(MessageType type);

// Check if a message type is a media type (image, video, audio, file).
bool isMediaType(MessageType type);

// Get a human-readable label for a message type.
std::string getMessageTypeLabel(MessageType type);

// Get a file extension from mimetype.
std::string getExtensionFromMimeType(const std::string& mimetype);

// Get a human-readable file size string ("1.5 MB", "340 KB", etc.).
std::string formatFileSize(int64_t bytes);

// Format message content as JSON for the Kotlin UI layer.
std::string messageContentToJson(const MessageContent& content);

} // namespace progressive

#endif // PROGRESSIVE_CONTENT_UTILS_HPP
