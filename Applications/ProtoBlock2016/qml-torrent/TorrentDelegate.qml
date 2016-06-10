import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import ProRotoQml.Torrent 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItem
import Material.Extras 1.0

ListItem.Subtitled {
    id: torrentDelegate
    elevation: 5
    anchors.left: parent.left
    anchors.right: parent.right
    // some reason having issues getting the name back along with other things like files and what not
    text: model.TorName
    content : Item{
        id: torSpeed
        RowLayout{
            Icon{
                color: TorState == TorrentModel.Downloading
                       ? theme.accentColor : Palette.colors['grey']['500']
                size: 15
                name: "qrc:/icons/download.png"
            }
            Label{
                Layout.fillWidth: true
                Layout.minimumWidth: 48
                text: speed2Name( TorDownSpeed )
            }
            Icon{
                color: TorState == TorrentModel.Downloading ||
                       TorState == TorrentModel.Seeding
                       ? theme.accentColor : Palette.colors['grey']['500']
                size:  15
                name: "qrc:/icons/upload.png"
            }
            Label{
                text: speed2Name( TorUpSpeed ) + " \t Peers" +  TorSeedCount + "/"+ TorPeerCount
            }
        }
    }

    secondaryItem: Item{
        id: progressLoader
        anchors{
            right: parent.right
            verticalCenter: parent.verticalCenter
        }

        height: torrentDelegate.height - 16
        width: height

        Icon{
            name: "qrc:/icons/done.png"
            size: parent.height
            visible: progressCircle.value == progressCircle.maximumValue
        }

        ProgressCircle {
            id: progressCircle
            anchors.fill: parent
            indeterminate: TorState != TorrentModel.Downloading &&
                           TorState != TorrentModel.Seeding &&
                           TorState != TorrentModel.Preparing &&
                           TorState != TorrentModel.Paused

            minimumValue: 0
            maximumValue: 100
            visible: progressCircle.value < progressCircle.maximumValue
            color: TorState == TorrentModel.Preparing ? Palette.colors['yellow']['700']
                                                      : Theme.primaryColor
            value: TorProgress < 0 ? 0 : TorProgress
            Behavior on value{
                NumberAnimation { duration: 100 }
            }

            Label {
                color: TorState == TorrentModel.Preparing ? Palette.colors['yellow']['900']
                                                          : Theme.primaryColor
                style: 'subheading'
                anchors.centerIn: parent
                text: TorProgress + "%"
            }
        }
    }

    action: Icon {
        id: playPauseIcon
        anchors.centerIn: parent
        name: TorState == TorrentModel.Paused ? "qrc:/icons/play.png" : "qrc:/icons/pause.png"
        size: torrentDelegate.height - 32

        MouseArea{
            id: playPauseArea
            anchors.fill: parent
            onClicked: {
                if ( TorState == TorrentModel.Downloading || TorState == TorrentModel.Seeding )
                    torModel.setPause( index, true )
                else if ( TorState == TorrentModel.Paused )
                    torModel.setPause( index, false )
            }
        }
    }
}

