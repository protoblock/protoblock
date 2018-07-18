import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1


import Torrent.TorrentModel 1.0
import Torrent.RateController 1.0


import ProRotoQml.Utils 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItem
import Material.Extras 1.0

ApplicationWindow {
    id: root

    property var torrentsComponents: ["All", "Downloading", "Seeding", "Paused"]
    property var settingsComponents: ["Speed settings"]
    property var sections: [torrentsComponents, settingsComponents]
    property var sectionTitles: [ "Torrents", "Settings" ]

    property string selectedComponent: sections[0][0]

    visible: true
//    clientSideDecorations: false

    theme {
        primaryColor:       Palette.colors["blue"]["500"]
        primaryDarkColor:   Palette.colors["blue"]["700"]
        accentColor:        Palette.colors["teal"]["500"]
        tabHighlightColor:  "white"
    }

    property var speed2Name: function( bytesPerSecond ){
        if ( bytesPerSecond < 1000 )
            return bytesPerSecond + " B";
        if ( bytesPerSecond < 1024000 )
            return Math.round( bytesPerSecond / 1024 * 100 ) / 100 + " Kb"
        if ( bytesPerSecond < 1048576000 )
            return Math.round( bytesPerSecond / 1048576 * 100 ) / 100 + " Mb"
        else
            return Math.round( bytesPerSecond / 1073741824 * 100 ) / 100 + " Gb"
    }

    initialPage: TabbedPage {
        id: page

        title: "Torrent Client"

        actionBar.maxActionCount: navDrawer.enabled ? 3 : 4
        actions: [
            Action {
                iconName: "av/play_arrow"
                name: "Play"
                onTriggered: {
                    for ( var i = 0; i < torrentModel.count; ++i ){
                        var res = torrentModel.data( i, TorrentModel.TorState );
                        if ( res == TorrentModel.Paused )
                            torrentModel.setPause( i, false )
                    }
                }
            },

            Action {
                iconName: "av/pause"
                name: "Pause"
                onTriggered: {
                    for ( var i = 0; i < torrentModel.count; ++i ){
                        var res = torrentModel.data( i, TorrentModel.TorState );
                        if ( res === TorrentModel.Downloading || res === TorrentModel.Seeding )
                            torrentModel.setPause( i, true )
                    }
                }
            },

            Action {
                iconName: "image/color_lens"
                name: "Color"
                onTriggered: colorPicker.show()
            },

            Action{
                id : addTorrentAction
                name: "New Torrent"
                iconName: "action/note_add"
                onTriggered: addTorrentDialog.open()
            },

            Action {
                iconName: "action/language"
                name: "Language"
            },

            Action {
                iconName: "action/settings"
                name: "Settings"
            }
        ]
        backAction: navDrawer.action

        tabs: navDrawer.enabled ? [] : sectionTitles

        NavigationDrawer {
            id: navDrawer

            enabled: true

            Flickable {
                anchors.fill: parent

                contentHeight: Math.max(content.implicitHeight, height)

                Column {
                    id: content
                    anchors.fill: parent

                    Repeater {
                        model: sections

                        delegate: Column {
                            width: parent.width

                            ListItem.Subheader {
                                text: sectionTitles[index]
                            }

                            Repeater {
                                model: modelData
                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == root.selectedComponent
                                    onClicked: {
                                        root.selectedComponent = modelData
                                        navDrawer.close()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }



        Repeater{
            id: tabView
            anchors.fill: parent
//            currentIndex: page.selectedTab
            model: sections

            delegate: Item {
                width: tabView.width
                height: tabView.height
                clip: true

                property string selectedComponent: modelData[0]

                Sidebar {
                    id: sidebar

                    expanded: !navDrawer.enabled

                    Column {
                        width: parent.width

                        Repeater {
                            model: modelData
                            delegate: ListItem.Standard {
                                text: modelData
                                selected: modelData == selectedComponent
                                onClicked: selectedComponent = modelData
                            }
                        }
                    }
                }
                Flickable {
                    id: flickable
                    anchors {
                        left:   sidebar.right
                        right:  parent.right
                        top:    parent.top
                        bottom: parent.bottom
                    }
                    clip: true
                    contentHeight: Math.max(pageLoader.implicitHeight + 40, height)
                    Loader {
                        id: pageLoader
                        anchors.fill: parent

                        // selectedComponent will always be valid, as it defaults to the first component
                        source: {
                            if (navDrawer.enabled) {
                                return root.selectedComponent === "Speed settings"
                                       ? "SpeedLimit.qml" : "TorrentListView.qml"
                            } else {
                                return selectedComponent === "Speed settings"
                                       ? "SpeedLimit.qml" : "TorrentListView.qml"
                            }
                        }
                    }
                }
                Scrollbar {
                    flickableItem: flickable
                }
            }

        }

        AddTorrentDialog{
            id : addTorrentDialog
            onAccepted: torrentModel.addTorrent( torrentFileUrl, destinationFolder )
        }

//        ColorPickerDialog {
//            id: colorPicker
//        }

    }
}
