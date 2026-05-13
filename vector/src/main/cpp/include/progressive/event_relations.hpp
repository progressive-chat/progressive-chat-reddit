#ifndef PROGRESSIVE_EVENT_RELATIONS_HPP
#define PROGRESSIVE_EVENT_RELATIONS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Event Relations (Threads, Replies, Edits) ----

struct RelationInfo {
    std::string relType;           // "m.thread", "m.reference", "m.replace", "m.annotation"
    std::string eventId;           // the event this relation points to
    std::string key;               // for m.annotation: the emoji key
    std::string fallback;          // for m.replace: new body text
    int count = 0;                 // for m.annotation: reaction count
    int currentIndex = 0;          // for threaded relations
    bool isFallingBack = false;    // for m.replace: show old body?
};

// Parse relation info from event content JSON.
RelationInfo parseRelation(const std::string& contentJson);

// Check if an event is part of a thread.
bool isThreadRoot(const std::string& contentJson);

// Check if an event is a reply (m.reference).
bool isReply(const std::string& contentJson);

// Check if an event is an edit (m.replace).
bool isEdit(const std::string& contentJson);

// Check if an event is a reaction (m.annotation).
bool isReaction(const std::string& contentJson);

// Extract the thread root event ID from content.
std::string extractThreadRoot(const std::string& contentJson);

// Extract the reply source event ID from content.
std::string extractReplySource(const std::string& contentJson);

// Extract the edit source event ID from content.
std::string extractEditSource(const std::string& contentJson);

// Build relation content for a reply.
std::string buildReplyRelation(const std::string& eventId, const std::string& threadRoot = "");

// Build relation content for a thread reply.
std::string buildThreadRelation(const std::string& threadRoot);

// Build relation content for an edit.
std::string buildEditRelation(const std::string& eventId);

// Format relation as a human-readable description.
std::string formatRelationDescription(const RelationInfo& relation);

// ---- Thread Summary ----

struct ThreadSummary {
    std::string rootEventId;
    std::string rootMessageBody;       // first message in thread
    std::string rootSender;
    int replyCount = 0;
    std::string lastReplyBody;
    std::string lastReplySender;
    int64_t lastReplyTs = 0;
    int participantCount = 0;
    bool unread = false;
};

// Compute a thread summary from event data.
ThreadSummary computeThreadSummary(
    const std::string& rootEventId,
    const std::string& rootBody,
    const std::string& rootSender,
    const std::vector<std::string>& replyBodies,
    const std::vector<std::string>& replySenders,
    const std::vector<int64_t>& replyTimestamps,
    bool hasUnread
);

// Format thread summary as JSON.
std::string threadSummaryToJson(const ThreadSummary& summary);

} // namespace progressive

#endif // PROGRESSIVE_EVENT_RELATIONS_HPP
