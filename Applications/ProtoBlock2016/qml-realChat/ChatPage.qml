import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import Communi 3.0

import Material 1.0
import Material.ListItems 1.0 as ListItmes


Item {
    id: page
    property IrcBuffer serverBuffer
    property alias bufferModel: bufferListView.bufferModel
    property alias currentBuffer: bufferListView.currentBuffer
    property IrcChannel currentChannel: currentBuffer ? currentBuffer.toChannel() : null

    Connections {
        target: bufferModel
        onAdded: currentBuffer = buffer
        onAboutToBeRemoved: {
            var idx = bufferModel.indexOf(buffer)
            currentBuffer = bufferModel.get(idx + 1) || bufferModel.get(Math.max(0, idx - 1))
        }
    }

    SplitView {
        anchors.fill: parent
        handleDelegate: Item { }
        BufferListView {
            id: bufferListView
            width: page.width / 6
            height: page.height - textEntry.height
            onClosed: {
                if (buffer === serverBuffer) {
                    bufferModel.quit()
                } else {
                    if (buffer.channel)
                        buffer.part(qsTr("Protoblock %1 ").arg(irc.version()))
                    bufferModel.remove(buffer)
                }
            }
        }

        Column {
            Layout.fillWidth: true
            TopicLabel {
                id: topicLabel
                width: parent.width
                visible: currentChannel
                channel: currentChannel
            }

            SplitView {
                width: parent.width
                height: parent.height - (currentChannel ? topicLabel.height : 0) - textEntry.height
                handleDelegate: Item { }
                Item {
                    id: stack
                    width: 1;
                    height: 1
                    Layout.fillWidth: true
                    Repeater {
                        anchors.fill: parent
                        model: bufferModel
                        delegate: TextBrowser {
                            anchors.fill: parent
                            buffer: model.buffer
                            visible: buffer == currentBuffer
                        }
                    }
                }

                UserListView {
                    width: page.width / 3
                    visible: currentChannel
                    channel: currentChannel
                    onQueried: currentBuffer = currentBuffer.model.add(user.name)
                }
            }

            TextEntry {
                id: textEntry
                width: parent.width
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
    }
}
