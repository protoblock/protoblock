import QtQuick 2.4

import ProRotoQml.Torrent 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItem
import Material.Extras 1.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Theme 1.0

Item{
    width:  ProtoScreen.guToPx(25)
    height:  ProtoScreen.guToPx(12.5)

    Button {
        id: ad
        text: "add torrent"
        width: parent.width
        anchors {
            bottom: parent.bottom
        }
        onClicked: addTorrentDialog.open()
    }
    ListView{
        id : listView
        width: parent.width
        height: parent.height - ad.height
        anchors.top: ad.bottom
        spacing:  ProtoScreen.guToPx(.5)
        clip: true
        model: torModel
        delegate: TorrentDelegate {
            id: torrentDelegate
        }

        Scrollbar{
            flickableItem: listView
        }
    }
}
