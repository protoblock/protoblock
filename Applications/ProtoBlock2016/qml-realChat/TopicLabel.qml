import QtQuick 2.4
import Communi 3.0
import Material 1.0
Card {
    id: background
    elevation: 0
    width: page.width - bufferListView.width  -  (usersList.width* 1.07)

    property IrcChannel channel
    height: Math.max(20, label.implicitHeight + 4)
    IrcTextFormat {
        id: textFormat
    }
    Label {
        id: label
        anchors.margins: 2
        anchors.leftMargin: 4
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        wrapMode: Text.Wrap
        text: channel && channel.topic ? textFormat.toHtml(channel.topic) : "-"
    }
}
