#ifndef PROGRESSIVE_SLASH_COMMAND_HPP
#define PROGRESSIVE_SLASH_COMMAND_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Slash Command Parser ----
// Ported from: im.vector.app.features.command.SlashCommandParser.kt
//              im.vector.app.features.command.SlashCommand.kt
//              im.vector.app.features.command.Command.kt

enum class SlashCommandType {
    Unknown,
    Me,            // /me action
    Join,          // /join #room:server
    Part,          // /part [reason]
    Invite,        // /invite @user:server
    Kick,          // /kick @user:server [reason]
    Ban,           // /ban @user:server [reason]
    Unban,         // /unban @user:server
    Op,            // /op @user:server [power_level]
    Deop,          // /deop @user:server
    Nick,          // /nick new_display_name
    RoomTopic,     // /topic new_topic
    RoomName,      // /roomname new_name
    Notice,        // /notice message
    Shrug,         // /shrug [message] → append ¯\_(ツ)_/¯
    Tableflip,     // /tableflip → append (╯°□°）╯︵ ┻━┻
    Unflip,        // /unflip → append ┬──┬ ノ( ゜-゜ノ)
    Lenny,         // /lenny → append ( ͡° ͜ʖ ͡°)
    Rainbow,       // /rainbow message → rainbow-coloured text
    RainbowMe,     // /rainbowme action
    Plain,         // /plain → send body without markdown
    Spoiler,       // /spoiler message → wrap in ||spoiler||
    ChangeRoomAvatar,  // /avatar
    DiscardSession,    // /discardsession
    ClearScalarToken,  // /clearscalartoken
    Markdown,      // /markdown → force markdown for next message
};

struct SlashCommand {
    SlashCommandType type = SlashCommandType::Unknown;
    std::string command;        // full command text e.g. "/me"
    std::string arguments;      // everything after the command
    bool isSlashCommand = false;
    bool needsMatrixİd = false;  // expects a Matrix ID parameter
};

// Parse a message text to detect and extract slash command.
// Original Kotlin (SlashCommandParser.kt:parseSlashCommand):
//   fun parseSlashCommand(text: String): SlashCommand?
SlashCommand parseSlashCommand(const std::string& text);

// Format the output of a slash command (e.g., "/me" → "* <user> message")
// Original Kotlin (SlashCommand.kt:formatCommand):
//   fun formatCommand(cmd: SlashCommand, sender: String): String
std::string formatSlashCommand(const SlashCommand& cmd, const std::string& senderDisplayName);

// Check if a message starts with a known slash command.
bool isKnownSlashCommand(const std::string& text);

// Get all available command names for autocomplete.
std::vector<std::string> getAvailableCommands();

// Get a description for each command.
std::string getCommandDescription(SlashCommandType type);

// Check if this command type expects a room parameter (#room:server).
bool needsRoomParameter(SlashCommandType type);

// Check if this command type expects a user parameter (@user:server).
bool needsUserParameter(SlashCommandType type);

// Check if the command sends a message (me, notice, shrug, etc.)
// vs performs a room action (join, kick, ban, etc.)
bool isMessageCommand(SlashCommandType type);

// Check if the command is a room admin command (op, deop, kick, ban).
bool isAdminCommand(SlashCommandType type);

} // namespace progressive

#endif // PROGRESSIVE_SLASH_COMMAND_HPP
