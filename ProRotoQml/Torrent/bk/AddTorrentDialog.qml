import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import Qt.labs.settings 1.0

import Material 1.0 as Material
import Material.Extras 1.0

Material.Dialog{
    title: "Add new torrent"
    width: Screen.width /  2
    height: Screen.height /  2
    property alias torrentFileUrl       : torrentFileDialog.fileUrl
    property alias destinationFolder    : destFolderDialog.folder

    Column{
        width: parent.width
        height: parent.height
        Material.TextField {
            text: torrentFileDialog.fileUrl
            placeholderText: "Torrent file"
            readOnly: true
            width: parent.width
            MouseArea{
                anchors.fill: parent
                onClicked: torrentFileDialog.open()
            }

            anchors.horizontalCenter: parent.horizontalCenter
        }

        Material.TextField {
            text: destFolderDialog.folder
            placeholderText: "Destination folder"
            width: parent.width
            readOnly: true

            MouseArea{
                anchors.fill: parent
                onClicked: destFolderDialog.open()
            }

            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Settings{
        category: "LastFolders"
        property alias torrentFile    : torrentFileDialog.folder
        property alias destination    : destFolderDialog.folder
    }

    FileDialog{
        id : destFolderDialog

        title: "Please choose a destination folder"

        selectFolder: true
        selectExisting: false
        selectMultiple: false
    }

    FileDialog{
        id : torrentFileDialog

        title: "Please choose a file"
        nameFilters: [ "Torrent files (*.torrent)", "All files (*)" ]
    }

}
