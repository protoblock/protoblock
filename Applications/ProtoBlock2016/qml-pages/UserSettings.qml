import QtQuick 2.0
import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItems
Item {
    id: usersettingsCard
    Component.onCompleted:  pageHelper.title = "User Settings"
    // move to System Settings
    Card {
        id: colorPicker
        width: parent.width / 1.07
        height: Unit.dp(90) + (selection.height * 2) + selectionBanner.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 32
        Banner{
            id: selectionBanner
            text: "Change Colors"
            height: 48
            width: parent.width
        }

        MenuField {
            id: selection
            model: ["Primary color", "Accent color", "Background color"]
            width: Unit.dp(160)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: selectionBanner.bottom
            anchors.topMargin: 32
        }

        Grid {
            columns: 7
            spacing: Unit.dp(8)
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
                    width: Unit.dp(30)
                    height: Unit.dp(30)
                    radius:Unit.dp(2)
                    color: modelData
                    border.width: modelData === "white" ? Unit.dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)

                    Ink {
                        anchors.fill: parent
                        onPressed: {
                            switch(selection.selectedIndex) {
                            case 0:
                                root.theme.primaryColor = parent.color
                                break;
                            case 1:
                                root.theme.accentColor = parent.color
                                break;
                            case 2:
                                root.theme.backgroundColor = parent.color
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
        anchors.topMargin: 32
        Banner{
            id: themeBanner
            width: parent.width
            height: 48
            text: "Theme Picker"
        }
        MenuField {
            id: themeMenu
            model: ["Material", "Classic", "Pinky"]
            width: parent.width / 1.07
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: themeBanner.bottom
            anchors.topMargin: 32
            onSelectedTextChanged: {
                console.log(selectedText)
                if(selectedText !== realRoot.theme ){
                    realRoot.theme =  selectedText
                }else {
                console.log( "Current Root Object is theme")

                }

          }
        }
    }
}
