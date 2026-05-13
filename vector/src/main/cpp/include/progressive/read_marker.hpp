#ifndef PROGRESSIVE_READ_MARKER_HPP
#define PROGRESSIVE_READ_MARKER_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Read Marker & Unread Count ----
// Ported from: im.vector.app.features.home.room.detail.TimelineViewModel.kt
//              (read marker positioning logic, ~50 lines of index math)
//              org.matrix.android.sdk.api.session.room.read.ReadMarkers.kt
//              (server-side read marker management)

struct ReadMarkerState {
    std::string roomId;
    std::string lastReadEventId;       // server-side read marker
    std::string firstUnreadEventId;    // first event after read marker
    int unreadCount = 0;               // events between read marker and latest
    int unreadMentions = 0;            // @mentions in unread range
    int unreadHighlights = 0;          // @room + keyword matches
    bool hasUnread = false;
    bool showReadMarker = false;       // show "New messages" divider
    int readMarkerIndex = -1;          // position in the loaded event list
};

// Compute read marker position in the loaded event list.
// Original Kotlin (TimelineViewModel.kt:1023-1035):
//   val indexOfEvent = timeline.getIndexOfEvent(targetEventId)
//   if (indexOfEvent == null) timeline.restartWithEventId(targetEventId)
//   setState { copy(highlightedEventId = targetEventId) }
ReadMarkerState computeReadMarker(const std::string& lastReadEventId,
    const std::vector<std::string>& loadedEventIds,
    const std::vector<std::string>& loadedSenders,
    const std::vector<bool>& isMention,
    const std::vector<bool>& isHighlight,
    const std::string& myUserId);

// Check if we should show the "jump to first unread" button.
bool shouldShowJumpToUnread(const ReadMarkerState& state);

// Format unread count for the jump button: "14 new messages".
std::string formatUnreadJumpLabel(const ReadMarkerState& state);

// Update read marker after sending own message (auto-advance).
std::string advanceReadMarker(const std::string& currentRoomId, const std::string& latestEventId);

// Format read marker state as JSON for the Kotlin UI layer.
std::string readMarkerToJson(const ReadMarkerState& state);

} // namespace progressive

#endif // PROGRESSIVE_READ_MARKER_HPP
