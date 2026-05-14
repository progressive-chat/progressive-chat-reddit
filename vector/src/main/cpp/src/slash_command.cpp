#include "progressive/slash_command.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

// ---- Command table ----
// Original Kotlin (Command.kt): companion object with all known commands
struct CommandEntry {
    const char* prefix;          // e.g. "/me"
    SlashCommandType type;
    bool needsRoom;
    bool needsUser;
    bool isMessage;
    bool isAdmin;
};

static const CommandEntry COMMAND_TABLE[] = {
    {"/me",         SlashCommandType::Me,             false, false, true,  false},
    {"/join",       SlashCommandType::Join,           true,  false, false, false},
    {"/part",       SlashCommandType::Part,           false, false, false, false},
    {"/invite",     SlashCommandType::Invite,         true,  true,  false, false},
    {"/kick",       SlashCommandType::Kick,           false, true,  false, true},
    {"/ban",        SlashCommandType::Ban,            false, true,  false, true},
    {"/unban",      SlashCommandType::Unban,          false, true,  false, true},
    {"/op",         SlashCommandType::Op,             false, true,  false, true},
    {"/deop",       SlashCommandType::Deop,           false, true,  false, true},
    {"/nick",       SlashCommandType::Nick,           false, false, false, false},
    {"/topic",      SlashCommandType::RoomTopic,      false, false, false, false},
    {"/roomname",   SlashCommandType::RoomName,       false, false, false, false},
    {"/notice",     SlashCommandType::Notice,         false, false, true,  false},
    {"/shrug",      SlashCommandType::Shrug,          false, false, true,  false},
    {"/tableflip",  SlashCommandType::Tableflip,      false, false, true,  false},
    {"/unflip",     SlashCommandType::Unflip,         false, false, true,  false},
    {"/lenny",      SlashCommandType::Lenny,          false, false, true,  false},
    {"/rainbow",    SlashCommandType::Rainbow,        false, false, true,  false},
    {"/rainbowme",  SlashCommandType::RainbowMe,      false, false, true,  false},
    {"/plain",      SlashCommandType::Plain,          false, false, true,  false},
    {"/spoiler",    SlashCommandType::Spoiler,        false, false, true,  false},
    {"/avatar",     SlashCommandType::ChangeRoomAvatar, false,false,false, false},
    {"/discardsession", SlashCommandType::DiscardSession, false,false,false,false},
    {"/clearscalartoken", SlashCommandType::ClearScalarToken, false,false,false,false},
    {"/markdown",   SlashCommandType::Markdown,       false, false, false, false},
    {nullptr,       SlashCommandType::Unknown,        false, false, false, false},
};

static const CommandEntry* findCommand(const std::string& lowered) {
    for (int i = 0; COMMAND_TABLE[i].prefix != nullptr; ++i) {
        if (lowered.find(COMMAND_TABLE[i].prefix) == 0) {
            // Check word boundary: prefix must be followed by space or end
            size_t len = strlen(COMMAND_TABLE[i].prefix);
            if (lowered.size() == len || lowered[len] == ' ') {
                return &COMMAND_TABLE[i];
            }
        }
    }
    return nullptr;
}

SlashCommand parseSlashCommand(const std::string& text) {
    SlashCommand result;
    if (text.empty() || text[0] != '/') return result;

    // Lowercase the command part for case-insensitive matching
    // Original Kotlin: text.toLowerCase(Locale.ROOT).startsWith("/")
    auto spacePos = text.find(' ');
    std::string cmdPart = (spacePos == std::string::npos) ? text : text.substr(0, spacePos);
    std::string lowered = cmdPart;
    for (char& c : lowered) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    // Also lowercase any following chars until space for matching e.g. "/Me"
    std::string fullLowered = text;
    for (size_t i = 0; i < fullLowered.size() && fullLowered[i] != ' '; ++i) {
        fullLowered[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(fullLowered[i])));
    }

    const CommandEntry* entry = findCommand(fullLowered);
    if (!entry) return result;

    result.type = entry->type;
    result.isSlashCommand = true;
    result.command = text.substr(0, spacePos == std::string::npos ? text.size() : spacePos);
    result.needsMatrixİd = entry->needsRoom || entry->needsUser;

    if (spacePos != std::string::npos && spacePos + 1 < text.size()) {
        result.arguments = text.substr(spacePos + 1);
        // Trim leading whitespace from arguments
        while (!result.arguments.empty() && result.arguments[0] == ' ') {
            result.arguments.erase(0, 1);
        }
    }

    return result;
}

std::string formatSlashCommand(const SlashCommand& cmd, const std::string& senderDisplayName) {
    // Original Kotlin (SlashCommand.kt):
    //   when (cmd.type) {
    //     Me -> "* $sender $cmd.args"
    //     Notice -> cmd.args
    //     Shrug -> "${cmd.args} ¯\\_(ツ)_/¯"
    //     ...
    //   }
    switch (cmd.type) {
        case SlashCommandType::Me:
        case SlashCommandType::RainbowMe: {
            std::ostringstream out;
            out << "* " << senderDisplayName << " " << cmd.arguments;
            return out.str();
        }
        case SlashCommandType::Notice:
            return cmd.arguments;
        case SlashCommandType::Shrug: {
            std::ostringstream out;
            out << cmd.arguments;
            if (!cmd.arguments.empty()) out << " ";
            out << "¯\\_(ツ)_/¯";
            return out.str();
        }
        case SlashCommandType::Tableflip:
            return "(╯°□°）╯︵ ┻━┻";
        case SlashCommandType::Unflip:
            return "┬──┬ ノ( ゜-゜ノ)";
        case SlashCommandType::Lenny:
            return "( ͡° ͜ʖ ͡°)";
        case SlashCommandType::Spoiler: {
            std::ostringstream out;
            out << "||" << cmd.arguments << "||";
            return out.str();
        }
        case SlashCommandType::Rainbow:
        case SlashCommandType::Plain:
        case SlashCommandType::Markdown:
            return cmd.arguments;
        default:
            return cmd.arguments;
    }
}

bool isKnownSlashCommand(const std::string& text) {
    if (text.empty() || text[0] != '/') return false;
    std::string lowered = text;
    for (char& c : lowered) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return findCommand(lowered) != nullptr;
}

std::vector<std::string> getAvailableCommands() {
    std::vector<std::string> result;
    for (int i = 0; COMMAND_TABLE[i].prefix != nullptr; ++i) {
        result.push_back(COMMAND_TABLE[i].prefix);
    }
    return result;
}

std::string getCommandDescription(SlashCommandType type) {
    switch (type) {
        case SlashCommandType::Me: return "Send an action message (/me waves hello)";
        case SlashCommandType::Join: return "Join a room (/join #room:server)";
        case SlashCommandType::Part: return "Leave the current room (/part [reason])";
        case SlashCommandType::Invite: return "Invite a user to the room (/invite @user:server)";
        case SlashCommandType::Kick: return "Kick a user from the room (/kick @user:server)";
        case SlashCommandType::Ban: return "Ban a user from the room (/ban @user:server)";
        case SlashCommandType::Unban: return "Unban a user (/unban @user:server)";
        case SlashCommandType::Op: return "Set user power level (/op @user:server)";
        case SlashCommandType::Deop: return "Remove user power level (/deop @user:server)";
        case SlashCommandType::Nick: return "Change display name (/nick New Name)";
        case SlashCommandType::RoomTopic: return "Change room topic (/topic New Topic)";
        case SlashCommandType::RoomName: return "Change room name (/roomname New Name)";
        case SlashCommandType::Notice: return "Send a plain text notice";
        case SlashCommandType::Shrug: return "Append ¯\\_(ツ)_/¯";
        case SlashCommandType::Tableflip: return "Append (╯°□°）╯︵ ┻━┻";
        case SlashCommandType::Unflip: return "Append ┬──┬ ノ( ゜-゜ノ)";
        case SlashCommandType::Lenny: return "Append ( ͡° ͜ʖ ͡°)";
        case SlashCommandType::Rainbow: return "Rainbow-coloured text";
        case SlashCommandType::RainbowMe: return "Rainbow-coloured action";
        case SlashCommandType::Plain: return "Send message without markdown";
        case SlashCommandType::Spoiler: return "Hide message in spoiler tags";
        case SlashCommandType::ChangeRoomAvatar: return "Change room avatar";
        case SlashCommandType::DiscardSession: return "Discard current session";
        case SlashCommandType::ClearScalarToken: return "Clear Scalar integration token";
        case SlashCommandType::Markdown: return "Force markdown formatting";
        default: return "Unknown command";
    }
}

bool needsRoomParameter(SlashCommandType type) {
    for (int i = 0; COMMAND_TABLE[i].prefix != nullptr; ++i) {
        if (COMMAND_TABLE[i].type == type) return COMMAND_TABLE[i].needsRoom;
    }
    return false;
}

bool needsUserParameter(SlashCommandType type) {
    for (int i = 0; COMMAND_TABLE[i].prefix != nullptr; ++i) {
        if (COMMAND_TABLE[i].type == type) return COMMAND_TABLE[i].needsUser;
    }
    return false;
}

bool isMessageCommand(SlashCommandType type) {
    for (int i = 0; COMMAND_TABLE[i].prefix != nullptr; ++i) {
        if (COMMAND_TABLE[i].type == type) return COMMAND_TABLE[i].isMessage;
    }
    return false;
}

bool isAdminCommand(SlashCommandType type) {
    for (int i = 0; COMMAND_TABLE[i].prefix != nullptr; ++i) {
        if (COMMAND_TABLE[i].type == type) return COMMAND_TABLE[i].isAdmin;
    }
    return false;
}

} // namespace progressive
