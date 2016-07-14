import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import Communi 3.0

import Material 1.0
import Material.ListItems 1.0 as ListItmes

import ProRotoQml.Theme 1.0
Item {
    id: page
    property IrcBuffer serverBuffer
    property alias bufferModel: bufferListView.bufferModel
    property alias currentBuffer: bufferListView.currentBuffer
    property IrcChannel currentChannel: currentBuffer ? currentBuffer.toChannel() : null
    width: rootLoader.width
    height: rootLoader.height


    UserListView {
        id: usersList
        width: page.width / 3
        height: rootLoader.height
        visible:  currentChannel
        anchors.right: parent.right
        channel: currentChannel
        onQueried: currentBuffer = currentBuffer.model.add(user.name)
    }

    Repeater {
        id: messageArea
        height: page.height - topicLabel.height
        width: page.width - (bufferListView.width  - (usersList.width * 1.07))
        model: bufferModel
        delegate: TextBrowser {
            height: (page.height / 1.3)  - ( topicLabel.height)
            width: page.width - bufferListView.width  -  (usersList.width* 1.07)
            anchors{
                right: usersList.left
                top: parent.top
                topMargin: topicLabel.height * 1.07
            }
            buffer: model.buffer
            visible: buffer == currentBuffer
        }
    }



    BufferListView {
        id: bufferListView
        width: rootLoader.width / 6
        height: rootLoader.height
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
    TopicLabel {
        id: topicLabel
        width: rootLoader.width - ( bufferListView.width  + (usersList.width * 1.07))
        anchors.left: bufferListView.right
        anchors.leftMargin: ProtoScreen.guToPx(1.25)
        visible: currentChannel
        channel: currentChannel
    }

}
