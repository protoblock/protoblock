import QtQuick 2.4
import Communi 3.1
import Material 1.0
import ProRotoQml.Theme 1.0


TextField {
    id: textField
    focus: true
    height: ProtoScreen.guToPx(11)
    placeholderText: qsTr("Type To Chat")

    property alias buffer: completer.buffer
    signal messageSent(IrcMessage message)
    Keys.onTabPressed: completer.complete(text, cursorPosition)
    IrcCompleter {
        id: completer
        onCompleted: {
            textField.text = text
            textField.cursorPosition = cursor
        }
        parser: IrcCommandParser {
            id: parser
            tolerant: true
            triggers: ["/"]
            channels: buffer ? buffer.model.channels : []
            target: buffer ? buffer.title : ""

            Component.onCompleted: {
                parser.addCommand(IrcCommand.Join, "JOIN <#channel> (<key>)")
                parser.addCommand(IrcCommand.CtcpAction, "ME [target] <message...>")
                parser.addCommand(IrcCommand.Nick, "NICK <nick>")
                parser.addCommand(IrcCommand.Part, "PART (<#channel>) (<message...>)")
            }
        }
    }
    onAccepted: {
        var cmd = parser.parse(text)
        if (cmd) {
            buffer.connection.sendCommand(cmd)
            if (cmd.type === IrcCommand.Message
                    || cmd.type === IrcCommand.CtcpAction
                    || cmd.type === IrcCommand.Notice) {
                var msg = cmd.toMessage(buffer.connection.nickName, buffer.connection)
                textField.messageSent(msg)
            }
            textField.text = ""
        }
    }
}
