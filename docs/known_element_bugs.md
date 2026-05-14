# Known Element Android Classic Bugs

This document tracks bugs in Element Android Classic that Progressive Chat's C++ port
intentionally fixes rather than inherits.

## Reaction Count Miscounting

**Symptom:** Reaction counts under messages are sometimes off by 1-3. The count shows
more reactions than actually exist, or flickers between values during sync.

**Root cause in Element Android Classic:**
1. Local echo reactions are added to the count immediately (before `/send` completes)
2. When the server echoes back the event, it's counted AGAIN (double-counting)
3. Redacted/removed reactions aren't reliably removed from the annotation summary
4. `ReactionsSummaryFactory.kt` uses `event.annotations?.reactionsSummary` which may
   be stale during incremental sync

**Fix in Progressive Chat:**
- `reaction_utils.cpp` tracks `ReactionInfo.synced` flag — local echoes set `synced=false`
- Only `synced=true` reactions contribute to `count` in `aggregateReactions()`
- When server response arrives, the local echo is replaced (not accumulated)
- Redactions are handled via `redact` flag in event timeline, removing the reaction
  from the annotation map before aggregation

**Verification:** Compare reaction counts in Element Classic vs Progressive Chat on
the same room — our count should always be ≤ Element's count.

## Thread Notification Count Inaccuracy

**Symptom:** Thread notification badges show incorrect counts — sometimes 0 when there
are unread replies, sometimes showing notifications for read threads.

**Root cause in Element Android Classic:**
1. `ThreadNotificationCount` depends on push rules evaluation which can be stale
2. The `read_receipts` position in threads is not always correctly tracked
3. Thread read markers rely on `m.fully_read` which has known race conditions

**Fix in Progressive Chat:**
- `read_receipts.cpp` computes unread count from the actual timeline position
- Thread-specific read markers are tracked separately from room read markers
- Incremental sync updates don't reset thread unread state

## Timeline Jump/Position Loss on Rotate

**Symptom:** When rotating the device, the timeline scrolls to the bottom and loses
the user's reading position.

**Root cause in Element Android Classic:**
- Activity recreation during rotation resets the RecyclerView position
- `TimelineEventController.kt` restores to "latest event" instead of saved position
- The saved instance state doesn't reliably preserve scroll position

**Fix in Progressive Chat:**
- C++ `timeline_utils.cpp` stores the current event ID position
- On rotation, `nativeGetPosition` returns the last known event position
- Kotlin restores to exact event, not just "bottom of timeline"

## Display Name Disambiguation Flashing

**Symptom:** Display names in the timeline briefly show as "Alice (@alice:matrix.org)"
before resolving to just "Alice".

**Root cause in Element Android Classic:**
- `DisplayNameResolver.kt` computes disambiguation asynchronously
- The initial render uses disambiguated name before member list is loaded
- When member list arrives, the name is re-resolved

**Fix in Progressive Chat:**
- C++ `room_name.cpp` pre-computes disambiguation on the native side
- The disambiguation decision is made before the event reaches the renderer
- No flash: the correct name (with or without MXID suffix) is shown immediately

---

*Last updated: 2026-05-14*
*To add a bug: describe symptom, root cause in original, fix in Progressive Chat*
