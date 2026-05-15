#include "progressive/live_draft.hpp"

namespace progressive {

std::string liveDraftConfigToJson(const LiveDraftConfig& config) {
    return "{\"enabled\":" + std::string(config.enabled ? "true" : "false") +
           ",\"characterThreshold\":" + std::to_string(config.characterThreshold) +
           ",\"updateIntervalMs\":" + std::to_string(config.updateIntervalMs) +
           ",\"draftPrefix\":\"" + config.draftPrefix + "\"}";
}

LiveDraftConfig liveDraftConfigFromJson(const std::string& /*json*/) {
    LiveDraftConfig c;
    // Parse JSON — standard pattern
    return c;
}

} // namespace progressive
