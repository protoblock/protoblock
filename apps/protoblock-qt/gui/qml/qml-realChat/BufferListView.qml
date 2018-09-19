import QtQuick 2.1
import QtQuick.Controls 1.4
import Communi 3.0

import Material 1.0
import Material.ListItems 1.0 as ListItems

import ProtoblockQml.Theme 1.0
Card {
    id: background
    elevation: 6
    property alias bufferModel: listView.model
    property IrcBuffer currentBuffer
    signal closed(IrcBuffer buffer)
    height: parent.height
    width: parent.width
    Menu {
        id: menu
        MenuItem {
            text: qsTr("Close")
            shortcut: qsTr("Ctrl+W")
            enabled: !!currentBuffer
            onTriggered: closed(currentBuffer)
        }
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.topMargin: -1

        ListView {
            id: listView
            delegate: ListItems.Subtitled {
                width: parent.width
                property bool first: index === 0
                property bool current: model.buffer === currentBuffer
                elevation: 1
                backgroundColor:  first ? Colors.grey  : current ? Colors.blue : "transparent"
                text: model.title
                visible: model.title === "162.254.24.67" ? false : true
                onClicked: {
                    currentBuffer = model.buffer
                }
                onPressAndHold: {
                    menu.popup()
                }
            }
        }
    }
}
