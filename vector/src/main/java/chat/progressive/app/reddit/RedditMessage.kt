package chat.progressive.app.reddit

import chat.progressive.app.protocol.IProtocolMessage
import chat.progressive.app.protocol.ProtocolContentType
import chat.progressive.app.protocol.ProtocolType

data class RedditMessage(
    override val id: String,
    override val chatId: String,
    override val senderId: String,
    override val senderName: String,
    override val text: String,
    override val timestamp: Long = System.currentTimeMillis(),
    override val isOutgoing: Boolean = false,
    override val contentType: ProtocolContentType = ProtocolContentType.TEXT,
    override val protocolType: ProtocolType = ProtocolType.REDDIT,
    val url: String = "",
    val score: Int = 0,
    val commentCount: Int = 0,
    val depth: Int = 0
) : IProtocolMessage
