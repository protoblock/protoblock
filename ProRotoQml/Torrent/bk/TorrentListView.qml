import QtQuick 2.4

import Torrent.TorrentModel 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItem
import Material.Extras 1.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Theme 1.0
ListView{
    id : listView
    width: parent.width
    height: parent.height
    spacing:  4
    clip: true
    model: torrentModel
    delegate: TorrentDelegate {
        id: torrentDelegate
    }

    Scrollbar{
        flickableItem: listView
    }

    ActionButton {
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 32
        }

        iconName: "qrc:/icons/bottom.png"
        onClicked: addTorrentDialog.open()
    }

}
