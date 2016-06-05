import QtQuick 2.0
import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItems
Item {
    id: usersettingsCard
    Component.onCompleted:  pageHelper.title = "User Settings"
    Rectangle{
        width: parent.width
        height: parent.height
    Text{
        width: parent.width
        height: Unit.dp(160)
        wrapMode: Text.WordWrap
        text:  errorString
    }

    Column{
        anchors.fill: parent
        spacing: 2
        ListItems.Standard {
            text: "Account Settings "
            elevation: 1
        }

        ListItems.Standard {
            text: MiddleMan.playersName
            elevation: 1
        }

        ListItems.Standard {
            text: MiddleMan.secert3File
            elevation: 1
        }

        ListItems.Standard {
            text: "Public Key: " /*+ userSetttings.publicKey*/
            elevation: 1
        }

        ListItems.Standard {
            text: "Private Key: "/* + userSetttings.privateKey*/
            elevation: 1
        }

        ListItems.Standard{
            text: MiddleMan.playersStatus
            elevation: 1
        }





// MOve to System Settings
//    Rectangle {
//        id: colorPicker
//        width: parent.width
//        height: children.height
//        MenuField {
//            id: selection
//            model: ["Primary color", "Accent color", "Background color"]
//            width: Unit.dp(160)
//        }

//        Grid {
//            columns: 7
//            spacing: Unit.dp(8)

//            Repeater {
//                model: [
//                    Colors.red, Colors.pink, Colors.purple, Colors.deepPurple, Colors.indigo,
//                    Colors.blue, Colors.lightBlue, Colors.cyan, Colors.teal, Colors.green,
//                    Colors.lightGreen, Colors.lime, Colors.yellow, Colors.amber, Colors.orange,
//                    Colors.deepOrange, Colors.grey, Colors.blueGrey, Colors.brown, Colors.black,
//                    "white"
//                ]

//                Rectangle {
//                    width: Unit.dp(30)
//                    height: Unit.dp(30)
//                    radius:Unit.dp(2)
//                    color: modelData
//                    border.width: modelData === "white" ? Unit.dp(2) : 0
//                    border.color: Theme.alpha("#000", 0.26)

//                    Ink {
//                        anchors.fill: parent
//                        onPressed: {
//                            switch(selection.selectedIndex) {
//                            case 0:
//                                Colors.primaryColor = parent.color
//                                break;
//                            case 1:
//                                Colors.accentColor = parent.color
//                                break;
//                            case 2:
//                                theme.backgroundColor = parent.color
//                                break;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
}
}
}
