// Auto-generated stubs from linker errors
#include "progressive/composer_manager.hpp"
#include "progressive/cross_signing_manager.hpp"
#include "progressive/device_manager_full.hpp"
#include "progressive/poll_manager.hpp"
#include "progressive/room_directory_manager.hpp"
#include "progressive/room_state_manager.hpp"
#include "progressive/server_notice_manager.hpp"
#include "progressive/space_graph.hpp"
#include "progressive/content_utils.hpp"
#include "progressive/login_flow.hpp"
#include "progressive/oidc_manager.hpp"
#include "progressive/media_viewer.hpp"
namespace progressive {

// SKIP: autoReplaceEmojis(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: buildMxcUri(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: buildQuotedBody(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
std::string ComposerManager::applyBold(std::string const&, int, int) { return {}; }
std::string ComposerManager::applyItalic(std::string const&, int, int) { return {}; }
ComposerManager::ComposerManager() {}
void ComposerManager::enterEditMode(std::string const&) { return {}; }
void ComposerManager::enterQuoteMode(std::string const&) { return {}; }
void ComposerManager::enterRegularMode(bool) { return {}; }
void ComposerManager::enterReplyMode(std::string const&) { return {}; }
void ComposerManager::setText(std::string const&) { return {}; }
std::string ComposerManager::stateToJson() const { return {}; }
std::string CrossSigningManager::buildCrossSigningInfo(std::string const&, progressive::CSM_CrossSigningKey const&, progressive::CSM_CrossSigningKey const&, progressive::CSM_CrossSigningKey const&) { return {}; }
std::string CrossSigningManager::buildMasterKey(std::string const&, std::string const&) { return {}; }
std::string CrossSigningManager::buildSelfSigningKey(std::string const&, std::string const&) { return {}; }
std::string CrossSigningManager::buildUserSigningKey(std::string const&, std::string const&) { return {}; }
bool CrossSigningManager::canCrossSign() const { return {}; }
bool CrossSigningManager::checkSelfTrust() const { return {}; }
std::string CrossSigningManager::crossSigningInfoToJson(progressive::CSM_CrossSigningInfo const&) const { return {}; }
CrossSigningManager::CrossSigningManager() {}
void CrossSigningManager::importPrivateKeys(std::string const&, std::string const&, std::string const&) { return {}; }
bool CrossSigningManager::isInitialized() const { return {}; }
void CrossSigningManager::markMyMasterKeyAsTrusted() { return {}; }
std::string CrossSigningManager::trustResultToJson(progressive::UserTrustResult const&) const { return {}; }
std::string DeviceManager::buildDeleteRequest(progressive::DeviceDeletionRequest const&) const { return {}; }
std::string DeviceManager::buildRenameRequest(progressive::DeviceRenameRequest const&) const { return {}; }
std::string DeviceManager::cryptoDeviceToJson(progressive::CryptoDeviceInfo const&) const { return {}; }
DeviceManager::DeviceManager() {}
std::string DeviceManager::devicesToJson(std::__ndk1::vector<progressive::DeviceInfo, std::__ndk1::allocator<progressive::DeviceInfo> > const&) const { return {}; }
std::string DeviceManager::deviceToJson(progressive::DeviceInfo const&) const { return {}; }
void DeviceManager::formatFingerprint(std::string const&) const { return {}; }
void DeviceManager::formatLastSeen(long long) const { return {}; }
void DeviceManager::getTrustLabel(progressive::DeviceTrustLevel const&) const { return {}; }
void DeviceManager::isDeviceInactive(long long, int) const { return {}; }
void DeviceManager::parseCryptoDeviceInfo(std::string const&, std::string const&, std::string const&) { return {}; }
void DeviceManager::parseDeviceInfo(std::string const&, std::string const&) { return {}; }
void DeviceManager::parseDevicesList(std::string const&) { return {}; }
void DeviceManager::satisfiesMinVersion(std::string const&, std::string const&) const { return {}; }
// SKIP: ensureCorrectFormattedBodyInTextReply(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: extractMentionQuery(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int)
// SKIP: extractUsefulTextFromReply(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: formatSpoilerTextFromHtml(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: getEditedTargetEventId(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: getExtensionFromMimeType(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: getLatestEditEventId(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: hasTextWithImage(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: normalizeMimeType(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: parseSpaceChild(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
void PollManager::buildPollEndContent(std::string const&, std::string const&, bool) { return {}; }
void PollManager::buildPollResponseContent(std::string const&, std::__ndk1::vector<std::string, std::__ndk1::allocator<std::string > > const&, bool) { return {}; }
void PollManager::buildPollStartContent(std::string const&, std::__ndk1::vector<std::string, std::__ndk1::allocator<std::string > > const&, progressive::PollKind, int, bool, std::string&) { return {}; }
void PollManager::formatPollEvent(progressive::PollResultFull const&) { return {}; }
void PollManager::getWinnerText(progressive::PollResultFull const&) const { return {}; }
void PollManager::isValidPollQuestion(std::string const&) { return {}; }
void PollManager::parsePollStartContent(std::string const&, bool) { return {}; }
PollManager::PollManager() {}
void PollManager::tallyVotes(progressive::PollContent const&, std::__ndk1::vector<progressive::PollVote, std::__ndk1::allocator<progressive::PollVote> > const&) { return {}; }
// SKIP: resolveMxcThumbnailUrl(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int, int, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
void RoomDirectoryManager::aliasResultToJson(progressive::AliasAvailabilityResult const&) const { return {}; }
void RoomDirectoryManager::buildPublicRoomsRequest(progressive::PublicRoomsParams const&) const { return {}; }
void RoomDirectoryManager::buildVisibilityRequest(progressive::RoomDirectoryVisibility) const { return {}; }
void RoomDirectoryManager::formatRoomPreview(progressive::PublicRoom const&) const { return {}; }
void RoomDirectoryManager::parseAliasAvailability(std::string const&, std::string const&) const { return {}; }
void RoomDirectoryManager::parsePublicRoomsResponse(std::string const&) const { return {}; }
void RoomDirectoryManager::parseVisibilityResponse(std::string const&) const { return {}; }
std::string RoomDirectoryManager::responseToJson(progressive::PublicRoomsResponse const&) const { return {}; }
RoomDirectoryManager::RoomDirectoryManager() {}
bool RoomStateManager::isInviteOnly(std::string const&) const { return {}; }
bool RoomStateManager::isPublicRoom(std::string const&) const { return {}; }
RoomStateManager::RoomStateManager() {}
void RoomStateManager::setHistoryVisibility(std::string const&, progressive::RSM_RoomHistoryVisibility) { return {}; }
void RoomStateManager::setJoinRule(std::string const&, progressive::RoomJoinRule) { return {}; }
std::string ServerNoticeManager::formatDowntime(long long) { return {}; }
void ServerNoticeManager::formatResourceLimitError(progressive::ServerNoticeInfo const&, progressive::ResourceLimitMode) { return {}; }
std::string ServerNoticeManager::getBannerColor(progressive::ServerNoticeInfo const&) { return {}; }
void ServerNoticeManager::getErrorCodeDescription(std::string const&) { return {}; }
bool ServerNoticeManager::isConsentError(std::string const&) { return {}; }
bool ServerNoticeManager::isRateLimitError(std::string const&) { return {}; }
bool ServerNoticeManager::isResourceLimitError(std::string const&) { return {}; }
void ServerNoticeManager::parseMatrixError(std::string const&) { return {}; }
ServerNoticeManager::ServerNoticeManager() {}
void ServerNoticeManager::serverNoticeToJson(progressive::ServerNoticeInfo const&) { return {}; }
void SpaceGraph::addChild(std::string const&, progressive::SpaceChildEntry const&) { return {}; }
void SpaceGraph::flatListToJson(std::__ndk1::vector<progressive::SpaceNode, std::__ndk1::allocator<progressive::SpaceNode> > const&) const { return {}; }
void SpaceGraph::getChildren(std::string const&) const { return {}; }
void SpaceGraph::getDepth(std::string const&) const { return {}; }
void SpaceGraph::getParents(std::string const&) const { return {}; }
void SpaceGraph::graphResultToJson(progressive::SpaceGraphResult const&) const { return {}; }
void SpaceGraph::isInSpace(std::string const&, std::string const&) const { return {}; }
void SpaceGraph::searchSpaceRooms(std::string const&, std::string const&) const { return {}; }
void SpaceGraph::setNodeMetadata(std::string const&, std::string const&, std::string const&, std::string const&, std::string const&, bool) { return {}; }
void SpaceGraph::setRoot(std::string const&, std::string const&, std::string const&, std::string const&) { return {}; }
SpaceGraph::SpaceGraph() {}
void SpaceGraph::spaceToTreeJson(std::string const&, int) const { return {}; }
void SpaceGraph::traverse(progressive::SpaceTraversalOptions const&) const { return {}; }
// SKIP: validateMessage(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int)
// SKIP: visibilityToString(progressive::RoomDirectoryVisibility)

std::string autoReplaceEmojis(std::string const&) { return {}; }
std::string buildMxcUri(std::string const&, std::string const&) { return {}; }
std::string buildQuotedBody(std::string const&, std::string const&, std::string const&) { return {}; }
void ensureCorrectFormattedBodyInTextReply(std::string const&, std::string const&, std::string const&) { return {}; }
void extractMentionQuery(std::string const&, int) { return {}; }
void extractUsefulTextFromReply(std::string const&) { return {}; }
std::string formatSpoilerTextFromHtml(std::string const&) { return {}; }
void getEditedTargetEventId(std::string const&) { return {}; }
void getExtensionFromMimeType(std::string const&) { return {}; }
void getLatestEditEventId(std::string const&, std::string const&) { return {}; }
void hasTextWithImage(std::string const&) { return {}; }
void normalizeMimeType(std::string const&) { return {}; }
std::string parseSpaceChild(std::string const&, std::string const&) { return {}; }
void resolveMxcThumbnailUrl(std::string const&, std::string const&, int, int, std::string const&) { return {}; }
void validateMessage(std::string const&, int) { return {}; }
bool visibilityToString(progressive::RoomDirectoryVisibility) { return {}; }
} // namespace progressive