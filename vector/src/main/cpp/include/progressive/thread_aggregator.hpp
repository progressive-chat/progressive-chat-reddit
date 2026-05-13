#ifndef PROGRESSIVE_THREAD_AGGREGATOR_HPP
#define PROGRESSIVE_THREAD_AGGREGATOR_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

struct ThreadInfo {
    std::string threadId;        // root thread event ID
    std::string roomId;
    std::string roomName;
    std::string accountId;       // which account (userId)
    std::string accountIndex;    // "1", "2", "3" — user-facing account number
    std::string lastMessage;     // preview of last reply
    std::string lastSender;
    int64_t lastTimestamp = 0;
    int replyCount = 0;
    bool unread = false;
};

class ThreadAggregator {
public:
    // Add or update a thread from a room.
    void addThread(const ThreadInfo& thread);

    // Remove all threads for a room.
    void removeRoom(const std::string& roomId);

    // Remove all threads for an account.
    void removeAccount(const std::string& accountId);

    // Get all threads, sorted by last activity (most recent first).
    std::vector<ThreadInfo> getAllThreads() const;

    // Get thread count.
    size_t count() const { return threads_.size(); }

    // Clear all.
    void clear();

    // Export as JSON.
    std::string exportJson() const;

private:
    // key: threadId → ThreadInfo
    std::unordered_map<std::string, ThreadInfo> threads_;
};

} // namespace progressive

#endif // PROGRESSIVE_THREAD_AGGREGATOR_HPP
