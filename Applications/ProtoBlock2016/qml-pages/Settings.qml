import QtQuick 2.0
//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0

//import ProRotoQml.Torrent 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItems
Item {
    id: usersettingsCard
    Component.onCompleted:  pageHelper.title = "System Settings"
    property var themes: ["Material"]

    Flickable{
        width: parent.width
        height: parent.height
        contentHeight: parent.height  * 1.8
        contentWidth: parent.width
        interactive: true
        boundsBehavior: Flickable.StopAtBounds

        Card {
            id: colorPicker
            width: parent.width / 1.07
            height: ProtoScreen.guToPx(12.25) + (selection.height * 2) + selectionBanner.height
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top // ProtoScreen.guToPx(4)
            anchors.topMargin: ProtoScreen.guToPx(4)
            Banner{
                id: selectionBanner
                text: "Change Colors"
                height:  ProtoScreen.guToPx(6)
                width: parent.width
                backgroundColor: themeroot.theme.primaryColor
            }

            MenuField {
                id: selection
                model: ["Primary color", "Accent color", "Background color"]
                width: ProtoScreen.guToPx(20)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: selectionBanner.bottom
                anchors.topMargin: ProtoScreen.guToPx(4)
            }

            Grid {
                columns: 7
                spacing: ProtoScreen.guToPx(1)
                anchors.top: selection.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                Repeater {
                    model: [
                        Colors.red, Colors.pink, Colors.purple, Colors.deepPurple, Colors.indigo,
                        Colors.blue, Colors.lightBlue, Colors.cyan, Colors.teal, Colors.green,
                        Colors.lightGreen, Colors.lime, Colors.yellow, Colors.amber, Colors.orange,
                        Colors.deepOrange, Colors.grey, Colors.blueGrey, Colors.brown, Colors.black,
                        "white"
                    ]

                    Rectangle {
                        width: ProtoScreen.guToPx(4)
                        height: ProtoScreen.guToPx(4)
                        radius:5
                        color: modelData
                        border.width: modelData === "white" ? 5 : 0
                        border.color: Theme.alpha("#000", 0.26)

                        Ink {
                            anchors.fill: parent
                            onPressed: {
                                switch(selection.selectedIndex) {
                                case 0:
                                    themeroot.theme.primaryColor = parent.color
                                    break;
                                case 1:
                                    themeroot.theme.accentColor = parent.color
                                    break;
                                case 2:
                                    themeroot.theme.backgroundColor = parent.color
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        Card{
            id: themPicker
            width: parent.width / 1.07
            height: colorPicker.height
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: colorPicker.bottom
            anchors.topMargin: ProtoScreen.guToPx(4)
            Banner{
                id: themeBanner
                width: parent.width
                height: ProtoScreen.guToPx(6)
                text: "Theme Picker"
                backgroundColor: themeroot.theme.primaryColor

            }
            Row{
                width: (parent.width / 5) * 3
                spacing: 5
                height: themPicker.height - themeBanner.height
                anchors.top: themeBanner.bottom
                anchors.topMargin: ProtoScreen.guToPx(1.5)
                anchors.horizontalCenter: parent.horizontalCenter
                Repeater {
                    id: themeMenu
                    model: ["Material"]
                    delegate:
                        ThemeButton{
                        width: parent.width / 3.2
                        height:  parent.height / 2
                        highlighted: modelData === realRoot.theme ? true : false
                        img: "qrc:/logoOnly.png"
                        name: modelData
                        onClicked: {
                            if(modelData !== realRoot.theme ){
                                realRoot.theme =  modelData
                            }else {
                                console.log( "Current themeroot Object is theme")
                            }
                        }
                    }
                }
            }
        }


        //        Card{
        //            id: torrentCard
        //            width: parent.width / 1.07
        //            height: colorPicker.height
        //            anchors.horizontalCenter: parent.horizontalCenter
        //            anchors.top: themPicker.bottom
        //            anchors.topMargin: ProtoScreen.guToPx(4)
        //            Banner{
        //                id: torrentSettingsBanner
        //                width: parent.width
        //                height: ProtoScreen.guToPx(4)
        //                text: "Torrent Settings"
        //            }
        //            SpeedLimit{
        //                width: parent.width / 1.07
        //                height: torrentCard.height - torrentSettingsBanner.height
        //                anchors.top: torrentSettingsBanner.bottom
        //                anchors.topMargin: torrentSettingsBanner.height + 5
        //            }
        //        }

        //        TorrentListView{
        //            id: torrentHelper
        //            width: parent.width / 1.07
        //            height: colorPicker.height
        //            anchors.horizontalCenter: parent.horizontalCenter
        //            anchors.top: torrentCard.bottom
        //            anchors.topMargin: ProtoScreen.guToPx(4)
        //        }
        //    }
    }
}
