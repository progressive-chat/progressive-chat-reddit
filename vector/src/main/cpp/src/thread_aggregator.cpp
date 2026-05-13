#include "progressive/thread_aggregator.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

void ThreadAggregator::addThread(const ThreadInfo& thread) {
    threads_[thread.threadId] = thread;
}

void ThreadAggregator::removeRoom(const std::string& roomId) {
    for (auto it = threads_.begin(); it != threads_.end(); ) {
        if (it->second.roomId == roomId) {
            it = threads_.erase(it);
        } else {
            ++it;
        }
    }
}

void ThreadAggregator::removeAccount(const std::string& accountId) {
    for (auto it = threads_.begin(); it != threads_.end(); ) {
        if (it->second.accountId == accountId) {
            it = threads_.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<ThreadInfo> ThreadAggregator::getAllThreads() const {
    std::vector<ThreadInfo> result;
    for (const auto& [_, t] : threads_) {
        result.push_back(t);
    }
    // Sort by last timestamp, most recent first
    std::sort(result.begin(), result.end(), [](const ThreadInfo& a, const ThreadInfo& b) {
        return a.lastTimestamp > b.lastTimestamp;
    });
    return result;
}

void ThreadAggregator::clear() {
    threads_.clear();
}

std::string ThreadAggregator::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    auto threads = getAllThreads();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < threads.size(); ++i) {
        if (i > 0) json << ",";
        const auto& t = threads[i];
        json << "{";
        json << R"("threadId": ")" << esc(t.threadId) << R"(",)";
        json << R"("roomId": ")" << esc(t.roomId) << R"(",)";
        json << R"("roomName": ")" << esc(t.roomName) << R"(",)";
        json << R"("accountId": ")" << esc(t.accountId) << R"(",)";
        json << R"("accountIndex": ")" << esc(t.accountIndex) << R"(",)";
        json << R"("lastMessage": ")" << esc(t.lastMessage) << R"(",)";
        json << R"("lastSender": ")" << esc(t.lastSender) << R"(",)";
        json << R"("lastTimestamp": )" << t.lastTimestamp << ",";
        json << R"("replyCount": )" << t.replyCount << ",";
        json << R"("unread": )" << (t.unread ? "true" : "false");
        json << "}";
    }
    json << "]";
    return json.str();
}

} // namespace progressive
