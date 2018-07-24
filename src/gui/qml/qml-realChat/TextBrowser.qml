import QtQuick 2.1
import QtQuick.Controls 1.4
import Communi 3.0

import ProRotoQml.Theme 1.0
Item {
    id: background
    property IrcBuffer buffer
    height: parent.height
    width: parent.width

    TextArea {
        id: textArea
        height: parent.height
        width: parent.width
        readOnly: true
        textFormat: Qt.RichText
        frameVisible: false
        TextEntry {
            id: textEntry
            width: textArea.width / 1.07
            focus: false
            anchors{
                horizontalCenter: textArea.horizontalCenter
                top:  textArea.bottom
                bottomMargin: ProtoScreen.guToPx(4)
            }
            buffer: currentBuffer
            enabled: currentBuffer
            onMessageSent: currentBuffer.receiveMessage(message)
            Connections {
                target: page
                onCurrentBufferChanged: {
                    if (page.visible && currentBuffer)
                        textEntry.forceActiveFocus()
                }
            }
        }
    }
    MessageFormatter {
        id: formatter
    }

    Connections {
        target: buffer
        onMessageReceived: {
            var line = formatter.formatMessage(message)
            if (line)
                textArea.append(line)
        }
    }
}
