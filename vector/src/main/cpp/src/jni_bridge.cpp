#include <jni.h>
#include <string>
#include <android/log.h>
#include "progressive/jumptodate.hpp"
#include "progressive/relation.hpp"
#include "progressive/exporter.hpp"
#include "progressive/eventcache.hpp"
#include "progressive/translate.hpp"
#include "progressive/proxy.hpp"
#include "progressive/yggdrasil.hpp"
#include "progressive/markdown.hpp"
#include "progressive/account_export.hpp"
#include "progressive/audio_engine.hpp"
#include "progressive/media_filter.hpp"
#include "progressive/content_filter.hpp"
#include "progressive/network_stats.hpp"
#include "progressive/masquerade.hpp"
#include "progressive/user_mask.hpp"
#include "progressive/chunked_upload.hpp"
#include "progressive/chat_features.hpp"
#include "progressive/invitation_hide.hpp"
#include "progressive/thread_aggregator.hpp"
#include "progressive/user_messages.hpp"
#include "progressive/room_version.hpp"
#include "progressive/chat_preview.hpp"
#include "progressive/ram_monitor.hpp"
#include "progressive/cache_manager.hpp"
#include "progressive/message_aggregator.hpp"
#include "progressive/room_info.hpp"
#include "progressive/deleted_archive.hpp"
#include "progressive/search_index.hpp"
#include "progressive/module_loader.hpp"
#include "progressive/notification.hpp"
#include "progressive/room_mirror.hpp"
#include "progressive/input_tools.hpp"
#include "progressive/llm.hpp"
#include "progressive/read_receipts.hpp"
#include "progressive/room_analytics.hpp"
#include "progressive/chat_tools.hpp"
#include "progressive/lang_detect.hpp"
#include "progressive/avatar_history.hpp"
#include "progressive/event_link.hpp"
#include "progressive/lightweight_call.hpp"
#include "progressive/scheduled_edit.hpp"
#include "progressive/svg_draw.hpp"
#include "progressive/profile_swiper.hpp"
#include "progressive/rainbow.hpp"
#include "progressive/text_formats.hpp"
#include "progressive/url_tools.hpp"
#include "progressive/notif_priority.hpp"
#include "progressive/matrix_patterns.hpp"
#include "progressive/desync_detector.hpp"
#include "progressive/latency_stats.hpp"
#include "progressive/string_utils.hpp"
#include "progressive/location_sharing.hpp"
#include "progressive/color_utils.hpp"
#include "progressive/e2ee_utils.hpp"
#include "progressive/thumbnail.hpp"
#include "progressive/waveform.hpp"
#include "progressive/session_timeout.hpp"
#include "progressive/password_validator.hpp"
#include "progressive/spellcheck.hpp"
#include "progressive/draft_manager.hpp"
#include "progressive/link_preview.hpp"
#include "progressive/hash_utils.hpp"
#include "progressive/room_stats.hpp"
#include "progressive/mention_parser.hpp"
#include "progressive/poll_utils.hpp"
#include "progressive/reaction_utils.hpp"
#include "progressive/file_validator.hpp"
#include "progressive/date_utils.hpp"
#include "progressive/message_queue.hpp"
#include "progressive/pinned_events.hpp"
#include "progressive/server_capabilities.hpp"
#include "progressive/username_validator.hpp"
#include "progressive/emoji_analyzer.hpp"
#include "progressive/identity_utils.hpp"
#include "progressive/notif_analyzer.hpp"
#include "progressive/sync_analyzer.hpp"
#include "progressive/user_rating.hpp"
#include "progressive/event_timeline.hpp"
#include "progressive/room_directory.hpp"
#include "progressive/widget_utils.hpp"
#include "progressive/sso_utils.hpp"
#include "progressive/backup_utils.hpp"
#include "progressive/device_manager.hpp"
#include "progressive/presence_utils.hpp"
#include "progressive/room_permissions.hpp"
#include "progressive/room_summary.hpp"
#include "progressive/membership_utils.hpp"
#include "progressive/event_validator.hpp"
#include "progressive/room_encryption.hpp"
#include "progressive/login_utils.hpp"
#include "progressive/connection_monitor.hpp"
#include "progressive/push_rules.hpp"
#include "progressive/space_utils.hpp"
#include "progressive/event_relations.hpp"
#include "progressive/e2ee_decoration.hpp"
#include "progressive/room_list.hpp"
#include "progressive/media_utils.hpp"
#include "progressive/notif_settings.hpp"
#include "progressive/invite_utils.hpp"
#include "progressive/session_manager.hpp"
#include "progressive/auth_utils.hpp"
#include "progressive/content_scanner.hpp"
#include "progressive/event_encryption.hpp"
#include "progressive/report_utils.hpp"
#include "progressive/webrtc_utils.hpp"
#include "progressive/message_retry.hpp"
#include "progressive/sync_utils.hpp"
#include "progressive/event_display.hpp"
#include "progressive/permalink.hpp"
#include "progressive/network_monitor.hpp"
#include "progressive/client_info.hpp"
#include "progressive/keyshare.hpp"
#include "progressive/displayname_utils.hpp"
#include "progressive/message_location.hpp"
#include "progressive/timeline_utils.hpp"
#include "progressive/cross_signing.hpp"
#include "progressive/edit_history.hpp"
#include "progressive/read_marker.hpp"
#include "progressive/slash_command.hpp"
#include "progressive/typing_monitor.hpp"
#include "progressive/url_preview.hpp"
#include "progressive/power_levels.hpp"
#include "progressive/well_known.hpp"
#include "progressive/room_sort.hpp"
#include "progressive/key_backup.hpp"
#include "progressive/content_utils.hpp"
#include "progressive/room_state.hpp"
#include "progressive/login_flow.hpp"
#include "progressive/device_naming.hpp"
#include "progressive/sync_filter.hpp"
#include "progressive/room_name.hpp"
#include "progressive/notif_format.hpp"
#include "progressive/matrix_error.hpp"
#include "progressive/agent_executor.hpp"
#include "progressive/push_condition.hpp"
#include "progressive/sender_notif_filter.hpp"
#include "progressive/string_order.hpp"
#include "progressive/event_classifier.hpp"
#include "progressive/content_guard.hpp"
#include "progressive/user_status.hpp"
#include "progressive/verification_utils.hpp"
#include "progressive/account_utils.hpp"
#include <sstream>
#include <chrono>

using namespace progressive;

// --- Singleton keyword filter ---
static progressive::KeywordFilter g_keywordFilter;

// --- Singleton network stats collector ---
static progressive::NetworkStatsCollector g_netStats;

// --- Singleton user mask registry ---
static progressive::UserMaskRegistry g_userMasks;

// --- Singleton chunked uploader ---
static progressive::ChunkedUploader g_uploader;

// --- Singleton invitation hide list ---
static progressive::InvitationHideList g_inviteHide;

// --- Singleton thread aggregator ---
static progressive::ThreadAggregator g_threadAgg;

// --- Singleton cache manager ---
static progressive::CacheManager g_cacheMgr;

// --- Singleton message aggregator ---
static progressive::MessageAggregator g_msgAgg;

// --- Singleton deleted archive ---
static progressive::DeletedMessageArchive g_deletedArchive;

// --- Singleton search index ---
static progressive::SearchIndex g_searchIndex;

// --- Singleton module loader ---
static progressive::ModuleLoader g_moduleLoader;

// --- Singleton notification keywords ---
static progressive::NotificationKeywords g_notifKeywords;

// --- Singleton room mirror manager ---
static progressive::RoomMirrorManager g_mirrorMgr;

// --- Singleton symbol bar ---
static progressive::SymbolBar g_symbolBar;

// --- Singleton replacement engine ---
static progressive::ReplacementEngine g_replacementEngine;

// --- Singleton user MXID visibility ---
static progressive::UserMxidVisibility g_mxidVisibility;

// --- Singleton user hide manager ---
static progressive::UserHideManager g_userHide;

// --- Singleton message queue ---
static progressive::MessageQueue g_msgQueue;

// --- Singleton language hide manager ---
static progressive::LanguageHideManager g_langHide;

// --- Singleton chat push down ---
static progressive::ChatPushDownManager g_chatPushDown;

// --- Singleton emoji blacklist ---
static progressive::EmojiBlacklist g_emojiBlacklist;

// --- Singleton avatar history ---
static progressive::AvatarHistory g_avatarHistory;

// --- Singleton lightweight call manager ---
static progressive::LightweightCallManager g_lightCall;

// --- Singleton scheduled edit queue ---
static progressive::ScheduledEditQueue g_schedEdits;

// --- Singleton drawing canvas ---
static progressive::DrawingCanvas g_drawCanvas;

// --- Singleton profile swiper ---
static progressive::ProfileSwiper g_profileSwiper;

// --- Singleton connection monitor ---
static progressive::ConnectionMonitor g_connectionMonitor;

// --- Singleton desync detector ---
static progressive::DesyncDetector g_desyncDetector;

// --- Singleton latency tracker ---
static progressive::LatencyTracker g_latencyTracker;

// --- Singleton location sharing manager ---
static progressive::LocationSharingManager g_locationSharing;

#define LOG_TAG "ProgressiveNative"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" {

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeValidateAndBuild
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;
 *
 * Returns JSON string: {"url": "...", ...} or {"error": "..."}
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeValidateAndBuild(
    JNIEnv* env,
    jclass /* this */,
    jstring jRoomId,
    jstring jDateString,
    jstring jServerUrl,
    jstring jAccessToken,
    jboolean jIsEnabled
) {
    // Feature flag check — C++ gates the feature
    if (!jIsEnabled) {
        const char* err = R"({"error":"/jumptodate is disabled. Enable it in Settings → Labs."})";
        return env->NewStringUTF(err);
    }
    if (!jRoomId || !jDateString || !jServerUrl || !jAccessToken) {
        jclass exClass = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(exClass, "All parameters must be non-null");
        return nullptr;
    }

    // Convert Java strings to C++ strings
    const char* roomIdCh     = env->GetStringUTFChars(jRoomId, nullptr);
    const char* dateCh       = env->GetStringUTFChars(jDateString, nullptr);
    const char* serverUrlCh  = env->GetStringUTFChars(jServerUrl, nullptr);
    const char* accessTokenCh = env->GetStringUTFChars(jAccessToken, nullptr);

    progressive::JumpToDateRequest request;
    request.roomId        = std::string(roomIdCh);
    request.dateString    = std::string(dateCh);
    request.serverBaseUrl = std::string(serverUrlCh);
    request.accessToken   = std::string(accessTokenCh);

    // Release Java strings
    env->ReleaseStringUTFChars(jRoomId, roomIdCh);
    env->ReleaseStringUTFChars(jDateString, dateCh);
    env->ReleaseStringUTFChars(jServerUrl, serverUrlCh);
    env->ReleaseStringUTFChars(jAccessToken, accessTokenCh);

    LOGD("nativeJumpToDate called: roomId=%s date=%s", request.roomId.c_str(), request.dateString.c_str());

    // Validate date and compute timestamp
    if (!progressive::validateAndComputeTimestamp(request)) {
        std::string errorJson = R"({"error":")" + request.errorMessage + R"("})";
        return env->NewStringUTF(errorJson.c_str());
    }

    // Build the MSC3030 URL
    std::string url = progressive::buildMsc3030Url(request);
    LOGD("MSC3030 URL: %s", url.c_str());

    // Return the URL and timestamp to Kotlin — HTTP call happens in Kotlin layer
    std::string resultJson =
        R"({"url":")" + url +
        R"(","accessToken":")" + request.accessToken +
        R"(","timestamp":)" + std::to_string(request.originServerTs) +
        R"(})";
    return env->NewStringUTF(resultJson.c_str());
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeParseResponse
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 *
 * Parses the HTTP response body and returns JSON with eventId or error.
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseResponse(
    JNIEnv* env,
    jclass /* this */,
    jstring jResponseBody,
    jint httpStatus
) {
    if (!jResponseBody) {
        jclass exClass = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(exClass, "Response body must be non-null");
        return nullptr;
    }

    const char* bodyCh = env->GetStringUTFChars(jResponseBody, nullptr);
    std::string body(bodyCh);
    env->ReleaseStringUTFChars(jResponseBody, bodyCh);

    LOGD("nativeParseResponse: status=%d body_len=%zu", httpStatus, body.size());

    auto result = progressive::parseTimestampToEventResponse(body, httpStatus);

    if (result.success) {
        std::string json = R"({"eventId":")" + result.eventId + R"("})";
        return env->NewStringUTF(json.c_str());
    } else {
        std::string json =
            R"({"error":")" + result.errorMessage +
            R"(","statusCode":)" + std::to_string(result.statusCode) + R"(})";
        return env->NewStringUTF(json.c_str());
    }
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeParseRelation
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 *
 * Parses event JSON to find the source event this event relates to.
 * @param eventJson  Full JSON of the Matrix event
 * @param allowedTypes  Comma-separated list of allowed relation types (e.g. "m.annotation,m.reference")
 *                      or empty string for all types.
 *
 * Returns JSON: {"sourceEventId": "$xyz", "relationType": "m.annotation"} or {"isRelation": false}
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseRelation(
    JNIEnv* env,
    jclass /* this */,
    jstring jEventJson,
    jstring jAllowedTypes
) {
    if (!jEventJson) {
        jclass exClass = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(exClass, "Event JSON must be non-null");
        return nullptr;
    }

    const char* jsonCh = env->GetStringUTFChars(jEventJson, nullptr);
    std::string json(jsonCh);
    env->ReleaseStringUTFChars(jEventJson, jsonCh);

    LOGD("nativeParseRelation: eventJson_len=%zu", json.size());

    auto relation = progressive::parseRelation(json);

    if (!relation.isRelation) {
        return env->NewStringUTF(R"({"isRelation": false})");
    }

    // Check if this relation type is allowed
    bool allowed = true;
    if (jAllowedTypes) {
        const char* allowedCh = env->GetStringUTFChars(jAllowedTypes, nullptr);
        std::string allowedStr(allowedCh);
        env->ReleaseStringUTFChars(jAllowedTypes, allowedCh);

        if (!allowedStr.empty()) {
            allowed = progressive::isJumpableRelationType(relation.relationType) &&
                      allowedStr.find(relation.relationType) != std::string::npos;
        }
    } else {
        allowed = progressive::isJumpableRelationType(relation.relationType);
    }

    if (!allowed) {
        return env->NewStringUTF(R"({"isRelation": false})");
    }

    std::string resultJson =
        R"({"isRelation": true, "sourceEventId": ")" + relation.sourceEventId +
        R"(", "relationType": ")" + relation.relationType + R"("})";
    return env->NewStringUTF(resultJson.c_str());
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeFormatEventHtml
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;
 *
 * Formats a single event as HTML.
 * @param senderName  Display name of the sender
 * @param timestamp   ISO 8601 timestamp string
 * @param body        Plain text body
 * @param msgType     Message type (m.text, m.image, etc.)
 * @param fileName    Attachment filename if applicable
 * @param mediaSize   Attachment size in bytes as string (empty if none)
 * @param relationType Relation type string (m.annotation, m.reference, etc.)
 * @param isContinuation Whether this event is from the same sender as previous
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatEventHtml(
    JNIEnv* env,
    jclass /* this */,
    jstring jSenderName,
    jstring jTimestamp,
    jstring jBody,
    jstring jMsgType,
    jstring jFileName,
    jstring jMediaSize,
    jstring jRelationType,
    jboolean jIsContinuation
) {
    ExportEvent event;
    event.senderName   = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    event.timestamp    = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
    event.body         = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    event.msgType      = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    event.fileName     = jFileName ? std::string(env->GetStringUTFChars(jFileName, nullptr)) : "";
    event.relationType = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";

    if (jMediaSize) {
        auto sizeStr = std::string(env->GetStringUTFChars(jMediaSize, nullptr));
        if (!sizeStr.empty()) event.mediaSize = std::stoll(sizeStr);
        env->ReleaseStringUTFChars(jMediaSize, sizeStr.c_str());
    }

    // Release all strings
    if (jSenderName)   env->ReleaseStringUTFChars(jSenderName, event.senderName.c_str());
    if (jTimestamp)    env->ReleaseStringUTFChars(jTimestamp, event.timestamp.c_str());
    if (jBody)         env->ReleaseStringUTFChars(jBody, event.body.c_str());
    if (jMsgType)      env->ReleaseStringUTFChars(jMsgType, event.msgType.c_str());
    if (jFileName)     env->ReleaseStringUTFChars(jFileName, event.fileName.c_str());
    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, event.relationType.c_str());

    auto html = progressive::formatEventHtml(event, jIsContinuation);
    return env->NewStringUTF(html.c_str());
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeFormatEventPlainText
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 *
 * Formats a single event as plain text line.
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatEventPlainText(
    JNIEnv* env,
    jclass /* this */,
    jstring jSenderName,
    jstring jTimestamp,
    jstring jBody,
    jstring jMsgType,
    jstring jFileName,
    jstring jRelationType
) {
    ExportEvent event;
    event.senderName   = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    event.timestamp    = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
    event.body         = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    event.msgType      = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    event.fileName     = jFileName ? std::string(env->GetStringUTFChars(jFileName, nullptr)) : "";
    event.relationType = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";

    if (jSenderName)   env->ReleaseStringUTFChars(jSenderName, event.senderName.c_str());
    if (jTimestamp)    env->ReleaseStringUTFChars(jTimestamp, event.timestamp.c_str());
    if (jBody)         env->ReleaseStringUTFChars(jBody, event.body.c_str());
    if (jMsgType)      env->ReleaseStringUTFChars(jMsgType, event.msgType.c_str());
    if (jFileName)     env->ReleaseStringUTFChars(jFileName, event.fileName.c_str());
    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, event.relationType.c_str());

    auto text = progressive::formatEventPlainText(event);
    return env->NewStringUTF(text.c_str());
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeBuildHtmlExport
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)Ljava/lang/String;
 *
 * Builds a complete HTML export document from event HTML strings.
 * @param roomName    Room display name
 * @param roomTopic   Room topic
 * @param exportDate  ISO date of export
 * @param eventHtmls  Array of pre-rendered event HTML strings
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildHtmlExport(
    JNIEnv* env,
    jclass /* this */,
    jstring jRoomName,
    jstring jRoomTopic,
    jstring jExportDate,
    jobjectArray jEventHtmls
) {
    auto roomName   = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    auto roomTopic  = jRoomTopic ? std::string(env->GetStringUTFChars(jRoomTopic, nullptr)) : "";
    auto exportDate = jExportDate ? std::string(env->GetStringUTFChars(jExportDate, nullptr)) : "";

    if (jRoomName)  env->ReleaseStringUTFChars(jRoomName, roomName.c_str());
    if (jRoomTopic) env->ReleaseStringUTFChars(jRoomTopic, roomTopic.c_str());
    if (jExportDate) env->ReleaseStringUTFChars(jExportDate, exportDate.c_str());

    // Build HTML document — events are already formatted, wrap them
    std::ostringstream html;
    html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
    html << "<meta charset=\"UTF-8\">\n";
    html << "<title>" << escapeHtml(roomName) << " — Chat Export</title>\n";
    html << R"(<style>
body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; margin: 0; padding: 16px; background: #f5f5f5; }
.mx_ExportHeader { background: #fff; border-radius: 8px; padding: 16px; margin-bottom: 16px; box-shadow: 0 1px 3px rgba(0,0,0,0.1); }
.mx_ExportHeader h1 { margin: 0 0 8px; font-size: 1.5em; }
.mx_ExportHeader p { margin: 4px 0; color: #666; font-size: 0.9em; }
.mx_EventTile { background: #fff; border-radius: 8px; padding: 12px 16px; margin-bottom: 8px; box-shadow: 0 1px 2px rgba(0,0,0,0.05); }
.mx_EventTile_continuation { margin-top: -4px; border-radius: 0 0 8px 8px; }
.mx_EventTile_info { margin-bottom: 6px; }
.mx_EventTile_sender { font-weight: 600; color: #333; margin-right: 8px; }
.mx_MessageTimestamp { color: #999; font-size: 0.85em; }
.mx_EventTile_body { color: #222; line-height: 1.5; }
.mx_EventTile_attachment { background: #f0f0f0; border-radius: 4px; padding: 8px; margin-bottom: 8px; }
.mx_Attachment_name { font-weight: 500; }
.mx_Attachment_size { color: #999; margin-left: 8px; }
.mx_EventTile_reaction { font-style: italic; color: #666; }
.mx_EventTile_content { white-space: pre-wrap; word-wrap: break-word; }
hr { border: none; border-top: 1px solid #e0e0e0; margin: 16px 0; }
</style>)" << "\n";
    html << "</head>\n<body>\n";
    html << "<div class=\"mx_ExportHeader\">\n";
    html << "  <h1>" << escapeHtml(roomName) << "</h1>\n";
    if (!roomTopic.empty()) html << "  <p>" << escapeHtml(roomTopic) << "</p>\n";
    html << "  <p>Exported: " << exportDate << "</p>\n";

    jsize count = env->GetArrayLength(jEventHtmls);
    html << "  <p>Total messages: " << count << "</p>\n";
    html << "</div>\n";

    for (jsize i = 0; i < count; ++i) {
        auto jHtml = (jstring)env->GetObjectArrayElement(jEventHtmls, i);
        if (jHtml) {
            auto htmlStr = std::string(env->GetStringUTFChars(jHtml, nullptr));
            html << htmlStr;
            env->ReleaseStringUTFChars(jHtml, htmlStr.c_str());
        }
    }

    html << "<hr>\n<p style=\"color:#999;text-align:center;\">Exported with Progressive Chat</p>\n";
    html << "</body>\n</html>";

    return env->NewStringUTF(html.str().c_str());
}

// --- Event Cache (global singleton for Stage 2 acceleration) ---
static progressive::EventCache g_eventCache;

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeCachePut
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCachePut(
    JNIEnv* env,
    jclass /* this */,
    jstring jEventId,
    jstring jSenderId,
    jstring jSenderName,
    jstring jTimestamp,
    jstring jBody,
    jstring jMsgType,
    jstring jEventType,
    jstring jRelationType,
    jstring jSourceEventId,
    jboolean jSentByMe
) {
    CachedEvent event;
    event.eventId     = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
    event.senderId    = jSenderId ? std::string(env->GetStringUTFChars(jSenderId, nullptr)) : "";
    event.senderName  = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    event.timestamp   = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
    event.body        = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    event.msgType     = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    event.eventType   = jEventType ? std::string(env->GetStringUTFChars(jEventType, nullptr)) : "";
    event.relationType = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";
    event.sourceEventId = jSourceEventId ? std::string(env->GetStringUTFChars(jSourceEventId, nullptr)) : "";
    event.sentByMe    = jSentByMe;

    // Release strings
    if (jEventId)     env->ReleaseStringUTFChars(jEventId, event.eventId.c_str());
    if (jSenderId)    env->ReleaseStringUTFChars(jSenderId, event.senderId.c_str());
    if (jSenderName)  env->ReleaseStringUTFChars(jSenderName, event.senderName.c_str());
    if (jTimestamp)   env->ReleaseStringUTFChars(jTimestamp, event.timestamp.c_str());
    if (jBody)        env->ReleaseStringUTFChars(jBody, event.body.c_str());
    if (jMsgType)     env->ReleaseStringUTFChars(jMsgType, event.msgType.c_str());
    if (jEventType)   env->ReleaseStringUTFChars(jEventType, event.eventType.c_str());
    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, event.relationType.c_str());
    if (jSourceEventId) env->ReleaseStringUTFChars(jSourceEventId, event.sourceEventId.c_str());

    g_eventCache.put(event);
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeCacheGetContext
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheGetContext(
    JNIEnv* env,
    jclass /* this */,
    jstring jEventId
) {
    if (!jEventId) {
        return env->NewStringUTF(R"({"cached": false})");
    }

    auto eventId = std::string(env->GetStringUTFChars(jEventId, nullptr));
    env->ReleaseStringUTFChars(jEventId, eventId.c_str());

    auto json = g_eventCache.getContextData(eventId);
    return env->NewStringUTF(json.c_str());
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeCacheSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheSize(
    JNIEnv* /* env */,
    jclass /* this */
) {
    return static_cast<jint>(g_eventCache.size());
}

// --- Translation ---

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeBuildTranslateRequest
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildTranslateRequest(
    JNIEnv* env, jclass,
    jstring jText, jstring jSourceLang, jstring jTargetLang,
    jstring jApiEndpoint, jstring jApiToken, jstring jModel
) {
    TranslateRequest req;
    req.text           = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    req.sourceLanguage = jSourceLang ? std::string(env->GetStringUTFChars(jSourceLang, nullptr)) : "";
    req.targetLanguage = jTargetLang ? std::string(env->GetStringUTFChars(jTargetLang, nullptr)) : "";
    req.apiEndpoint    = jApiEndpoint ? std::string(env->GetStringUTFChars(jApiEndpoint, nullptr)) : "";
    req.apiToken       = jApiToken ? std::string(env->GetStringUTFChars(jApiToken, nullptr)) : "";
    req.model          = jModel ? std::string(env->GetStringUTFChars(jModel, nullptr)) : "gpt-4o-mini";

    if (jText)        env->ReleaseStringUTFChars(jText, req.text.c_str());
    if (jSourceLang)  env->ReleaseStringUTFChars(jSourceLang, req.sourceLanguage.c_str());
    if (jTargetLang)  env->ReleaseStringUTFChars(jTargetLang, req.targetLanguage.c_str());
    if (jApiEndpoint) env->ReleaseStringUTFChars(jApiEndpoint, req.apiEndpoint.c_str());
    if (jApiToken)    env->ReleaseStringUTFChars(jApiToken, req.apiToken.c_str());
    if (jModel)       env->ReleaseStringUTFChars(jModel, req.model.c_str());

    auto body = progressive::buildTranslateRequestBody(req);
    return env->NewStringUTF(body.c_str());
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeParseTranslateResponse
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseTranslateResponse(
    JNIEnv* env, jclass,
    jstring jResponseBody, jint jHttpStatus
) {
    if (!jResponseBody) {
        return env->NewStringUTF(R"({"success": false, "error": "Empty response"})");
    }
    auto body = std::string(env->GetStringUTFChars(jResponseBody, nullptr));
    env->ReleaseStringUTFChars(jResponseBody, body.c_str());

    auto result = progressive::parseTranslateResponse(body, jHttpStatus);

    if (result.success) {
        std::string json = R"({"success": true, "translatedText": ")" + result.translatedText + R"("})";
        return env->NewStringUTF(json.c_str());
    } else {
        std::string json = R"({"success": false, "error": ")" + result.errorMessage + R"(", "statusCode": )"
            + std::to_string(result.statusCode) + "}";
        return env->NewStringUTF(json.c_str());
    }
}

// --- Proxy Configuration ---

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeComputeProxyConfig
 * Signature: (IILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeProxyConfig(
    JNIEnv* env, jclass,
    jint jConnType, jint jProxyType,
    jstring jHost, jint jPort,
    jstring jUsername, jstring jPassword
) {
    auto connType = static_cast<ConnectionType>(jConnType);
    auto proxyType = static_cast<ProxyType>(jProxyType);

    auto host     = jHost ? std::string(env->GetStringUTFChars(jHost, nullptr)) : "";
    auto username = jUsername ? std::string(env->GetStringUTFChars(jUsername, nullptr)) : "";
    auto password = jPassword ? std::string(env->GetStringUTFChars(jPassword, nullptr)) : "";

    if (jHost)     env->ReleaseStringUTFChars(jHost, host.c_str());
    if (jUsername) env->ReleaseStringUTFChars(jUsername, username.c_str());
    if (jPassword) env->ReleaseStringUTFChars(jPassword, password.c_str());

    auto config = progressive::computeProxyConfig(
        connType, proxyType, host, jPort, username, password
    );

    auto json = config.toJson();
    return env->NewStringUTF(json.c_str());
}

// --- Yggdrasil ---

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsYggdrasilAddress(
    JNIEnv* env, jclass, jstring jAddr
) {
    if (!jAddr) return JNI_FALSE;
    auto addr = std::string(env->GetStringUTFChars(jAddr, nullptr));
    env->ReleaseStringUTFChars(jAddr, addr.c_str());
    return progressive::isYggdrasilAddress(addr) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsYggdrasilDomain(
    JNIEnv* env, jclass, jstring jHost
) {
    if (!jHost) return JNI_FALSE;
    auto host = std::string(env->GetStringUTFChars(jHost, nullptr));
    env->ReleaseStringUTFChars(jHost, host.c_str());
    return progressive::isYggdrasilDomain(host) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildYggHomeserverUrl(
    JNIEnv* env, jclass, jstring jAddr, jint jPort, jboolean jTls
) {
    if (!jAddr) return env->NewStringUTF("");
    auto addr = std::string(env->GetStringUTFChars(jAddr, nullptr));
    env->ReleaseStringUTFChars(jAddr, addr.c_str());
    auto url = progressive::buildYggHomeserverUrl(addr, jPort, jTls);
    return env->NewStringUTF(url.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeRewriteHomeserverUrl(
    JNIEnv* env, jclass, jstring jOriginalUrl, jstring jYggAddr
) {
    if (!jOriginalUrl || !jYggAddr) return env->NewStringUTF("");
    auto original = std::string(env->GetStringUTFChars(jOriginalUrl, nullptr));
    auto ygg = std::string(env->GetStringUTFChars(jYggAddr, nullptr));
    env->ReleaseStringUTFChars(jOriginalUrl, original.c_str());
    env->ReleaseStringUTFChars(jYggAddr, ygg.c_str());
    auto result = progressive::rewriteHomeserverUrl(original, ygg);
    return env->NewStringUTF(result.c_str());
}

// --- Read Marker Time Label ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatJumpToUnreadLabel(
    JNIEnv* env, jclass, jstring jReadMarkerJson, jlong jNowMs
) {
    auto json = jReadMarkerJson ? std::string(env->GetStringUTFChars(jReadMarkerJson, nullptr)) : "{}";
    if (jReadMarkerJson) env->ReleaseStringUTFChars(jReadMarkerJson, json.c_str());

    progressive::ReadMarkerState state;
    // Quick parse from JSON
    if (json.find("\"hasUnread\": true") != std::string::npos ||
        json.find("\"hasUnread\":true") != std::string::npos) state.hasUnread = true;
    // Extract timestamp
    auto tsPos = json.find("\"firstUnreadTimestampMs\"");
    if (tsPos != std::string::npos) {
        auto colon = json.find(':', tsPos);
        if (colon != std::string::npos) {
            colon++;
            while (colon < json.size() && (json[colon] == ' ' || json[colon] == '\t')) colon++;
            while (colon < json.size() && json[colon] >= '0' && json[colon] <= '9') {
                state.firstUnreadTimestampMs = state.firstUnreadTimestampMs * 10 + (json[colon] - '0');
                colon++;
            }
        }
    }

    auto label = progressive::formatJumpToUnreadLabel(state, jNowMs);
    return env->NewStringUTF(label.c_str());
}

} // extern "C"
