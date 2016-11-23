import QtQuick 2.0
import QtQuick.Controls 1.4
//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1
import Material.Styles 1.0


Item {
    id: topi
//    property int week:
//        MiddleMan.theWeek === 0 || !stack || !stack.currentItem ? 0 :
//                          (stack.currentItem.objectName === "prevWeekS" ? MiddleMan.thePrevWeek : (stack.currentItem.objectName === "nextWeekS" ? MiddleMan.theNextWeek : MiddleMan.theWeek))
    property string seasontext: MiddleMan.seasonString + " 2016 "
    property string liveorresult: MiddleMan.liveSync

    Component.onCompleted: {
         pageHelper.title = "Trading ROW"
        console.log(" proj wisth" + parent.width + " 2 " + rootLoader.width + " 3 " + themeroot.width + " 4 " + realRoot.width + " 5 " + pageHelper.width)
    }

        // spacer
    Rectangle{width: ProtoScreen.guToPx(.125); height: ProtoScreen.guToPx(1);color: "transparent"}


    Card {
        id: topcard
        width: parent.width
        height: parent.height
        anchors{
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: seasontext + "Rest-Of-Way" + " " + liveorresult

            font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
            color: themeroot.theme.primaryColor
            Layout.fillHeight: true
            Layout.fillWidth: false
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            id: cBan
        }

        SystemPalette { id: pal }


        Item {
            anchors.top: cBan.bottom
            anchors.topMargin: ProtoScreen.guToPx(.25)
            width: parent.width
            height: parent.height
            SplitView {
                orientation: Qt.Horizontal
                handleDelegate: handeldel
                width: parent.width
                height: parent.height
                SplitView {
                    id: rightr
                    orientation: Qt.Vertical
//                    handleDelegate: handeldel
//                    width: parent.width
                    height: parent.height
//                    anchors.leftMargin: 10
                    Layout.minimumWidth: parent.width * .20
                    Layout.maximumWidth: parent.width * .70

                    Card {
                        TradingFloor {}
                        Layout.maximumHeight: parent.height * .90
                        Layout.minimumHeight: parent.height * .10
                        Layout.fillHeight: true
                    }
                    Card {
                        Layout.maximumHeight: parent.height * .90
                        Layout.minimumHeight: parent.height * .10
                        Orders {
                           anchors.fill: parent
                        }
//                        Column{
//                            anchors.fill: parent

//                            TextField{
//                                id: txt
//                                text: "hello"
//                                property string color: "white"
//                                style: TextFieldStyle{
//                                    background: Rectangle {
//                                        id: rect
//                                        radius: 2
//                                        implicitWidth: 100
//                                        implicitHeight: 24
//                                        border.color: "#333"
//                                        border.width: 1
//                                        color: txt.color
//                                        Behavior on color {
//                                            SequentialAnimation {
//                                                loops: 1
//                                                ColorAnimation { from: "white"; to: "red"; duration: 1 }
//                                                ColorAnimation { from: "red"; to: "white";  duration: 3000 }
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                            Button{
//                                text: "blink"
//                                onClicked: {
//                                    txt.color = "red";
//                                    txt.color = "white";
//                                }
//                            }
//                        }
                    }
                }

                Card {
                    Layout.minimumWidth: parent.width * .30
                    Layout.maximumWidth: parent.width * .80
                    Layout.fillWidth: true

                    WkTradingTable {}
                }

                Card {
                    Layout.minimumWidth: parent.width * .10
                    Layout.maximumWidth: parent.width * .30
//                    Layout.fillWidth: true

                    Portfolio {}
                }
            }
        }
    }

    Component {
        id: handeldel

        Item {
            height: parent.height-cBan.height
            anchors.margins: 0
            Rectangle {
                border.width: 0
                id: rec
                width: styleData.hovered ? ProtoScreen.guToPx(.25) : ProtoScreen.guToPx(.1)
                color: styleData.hovered ? "black" : Qt.darker(pal.window, 1.5)
                height: parent.height
                anchors.right: rec3.left
            }
            Rectangle {
                border.width: 0
                id: rec3
                width: styleData.hovered ? ProtoScreen.guToPx(.35) : ProtoScreen.guToPx(.1)

                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                color: styleData.hovered ? "white" : Qt.darker(pal.window, 1.5)
            }
            Rectangle {
               border.width: 0
               id: rec2
               width: styleData.hovered ? ProtoScreen.guToPx(.25) : ProtoScreen.guToPx(.1)

               height: parent.height
               anchors.left: rec3.right
               color: styleData.hovered ? "black" : Qt.darker(pal.window, 1.5)
            }
        }
    }
}





