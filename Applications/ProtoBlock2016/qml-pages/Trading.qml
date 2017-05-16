import QtQuick 2.0
import QtQuick.Controls 1.4
//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1
import Material.Styles 1.0
import QtQuick.Controls.Styles 1.4


Item {
    id: topi
//    property int week:
//        MiddleMan.theWeek === 0 || !stack || !stack.currentItem ? 0 :
//                          (stack.currentItem.objectName === "prevWeekS" ? MiddleMan.thePrevWeek : (stack.currentItem.objectName === "nextWeekS" ? MiddleMan.theNextWeek : MiddleMan.theWeek))
//    property string seasontext: MiddleMan.seasonString + " 2016 "
    property string seasontext: MiddleMan.seasonString + " 2016 - Week " + MiddleMan.theWeek
    property string liveorresult: MiddleMan.liveSync

    Component.onCompleted: {
         pageHelper.title = "Trading"
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
            text: seasontext + " " + liveorresult

            font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
            color: themeroot.theme.primaryColor
            Layout.fillHeight: true
            Layout.fillWidth: false
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            id: cBan
        }

        SystemPalette { id: pal }


        TradingContextBanner {
            id: tcbx
            anchors.top: cBan.bottom
            Layout.preferredWidth: parent.width * .50
//            anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            width: parent.width
            anchors.leftMargin: ProtoScreen.guToPx(.25)
        }
//        RowLayout {
//            id: tcb
//            width: parent.width
//            anchors.top: cBan.bottom
//            height: ProtoScreen.guToPx(8)

//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            spacing: 1

//            TradingContextBanner {
//                id: tcbx
//                anchors.top: tcb.top
//                Layout.preferredWidth: parent.width * .50
//    //            anchors.horizontalCenter: parent.horizontalCenter
//                anchors.left: parent.left
////                width: parent.width
//                anchors.leftMargin: ProtoScreen.guToPx(.25)
//            }

//            FantasyNameContextBanner {
//                id: fcbx
//                anchors.top: tcb.top
//                Layout.preferredWidth: parent.width * .50

//    //            anchors.horizontalCenter: parent.horizontalCenter
//    //            width: parent.width * .50
//                anchors.right: parent.right
//                anchors.rightMargin: ProtoScreen.guToPx(.25)
//            }
//        }

        Item {
            id: itema
            anchors.top: tcbx.bottom
            anchors.topMargin: ProtoScreen.guToPx(.25)
            width: parent.width
            height: parent.height - tcbx.height - cBan.height
            SplitView {
                orientation: Qt.Horizontal
                handleDelegate: handeldel
                width: parent.width
                height: parent.height
//                SplitView {
//                    id: rightr
//                    orientation: Qt.Vertical
////                    handleDelegate: handeldel
////                    width: parent.width
//                    height: parent.h`eight
////                    anchors.leftMargin: 10
//                    Layout.minimumWidth: parent.width * .20
//                    Layout.maximumWidth: parent.width * .70

//                    Card {
//                        MarketDepthTable {}
//                        Layout.maximumHeight: parent.height * .90
//                        Layout.minimumHeight: parent.height * .10
//                        Layout.fillHeight: true
//                    }
//                    Card {
//                        Layout.maximumHeight: parent.height * .90
//                        Layout.minimumHeight: parent.height * .10
//                        Orders {
//                           anchors.fill: parent
//                        }
////                        Column{
////                            anchors.fill: parent

////                            TextField{
////                                id: txt
////                                text: "hello"
////                                property string color: "white"
////                                style: TextFieldStyle{
////                                    background: Rectangle {
////                                        id: rect
////                                        radius: 2
////                                        implicitWidth: 100
////                                        implicitHeight: 24
////                                        border.color: "#333"
////                                        border.width: 1
////                                        color: txt.color
////                                        Behavior on color {
////                                            SequentialAnimation {
////                                                loops: 1
////                                                ColorAnimation { from: "white"; to: "red"; duration: 1 }
////                                                ColorAnimation { from: "red"; to: "white";  duration: 3000 }
////                                            }
////                                        }
////                                    }
////                                }
////                            }
////                            Button{
////                                text: "blink"
////                                onClicked: {
////                                    txt.color = "red";
////                                    txt.color = "white";
////                                }
////                            }
////                        }
//                    }
//                }


                SplitView {
                    Layout.minimumWidth: parent.width * 0
                    Layout.maximumWidth: parent.width * .50
                    orientation: Qt.Vertical
                    height: parent.height
                    width: ProtoScreen.guToPx(40)

                    Card {
                        MarketDepthTable {}
                        Layout.maximumHeight: parent.height
                        Layout.minimumHeight: parent.height * 0
                        Layout.fillHeight: true

                    }

                    Card {
                        Layout.maximumHeight: parent.height
                        Layout.minimumHeight: parent.height * 0
                        backgroundColor: "#f5f5f5"
                        TabView {
                            anchors.fill: parent
                            Tab {
                                title: "Orders"
                                Orders {
                                   anchors.centerIn: parent
                                   anchors.fill: parent
                                }
                            }
                            Tab {
                                title: "Buy"
                                BuyTab {
                                   anchors.fill: parent
                                   anchors.centerIn: parent
                                }
                            }
                            Tab {
                                title: "Sell"
                                BuyTab {
                                   anchors.fill: parent
                                   anchors.centerIn: parent
                                }
                            }

                            style: TabViewStyle {
//                                frameOverlap: ProtoScreen.guToPx(.25)
                                tabsAlignment: Qt.AlignHCenter
//                                tabOverlap: ProtoScreen.guToPx(-1)
                                tab: Rectangle  {
                                    color: {
                                        if ( styleData.selected ) return "#f5f5f5"
                                        else if (styleData.title === "Buy")
                                            return "green"
                                        else if ( styleData.title === "Sell")
                                            return "red"
                                        else return themeroot.theme.primaryColor
                                    }
                                    implicitWidth: ProtoScreen.guToPx(10)
                                    implicitHeight: ProtoScreen.guToPx(3)
                                    radius: 2
                                    Label {
                                        id: text
                                        anchors.centerIn: parent
                                        anchors.fill: parent
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)

                                        text: styleData.title
                                        color: styleData.selected ? "black" : "#f5f5f5"
                                    }
                                }
                                frame: Rectangle { color: "#f5f5f5" }
                            }
                        }
                    }

                }
                Card {
                    Layout.minimumWidth: parent.width * .50
                    Layout.maximumWidth: parent.width
                    Layout.fillWidth: true
                    height: parent.height
//                    backgroundColor: "grey"

                    Card {
                        height: parent.height - bcard.height
                        width: parent.width
                        id: wkt
//                        backgroundColor: "orange"

                        WkTradingTable {}
                    }

                    Card {
                        anchors.top: wkt.bottom
//                        backgroundColor: "yellow"
                        id: bcard
                        height: ProtoScreen.guToPx(16)
                        width: parent.width
//                        height: parent.height * .40
//                        width: parent.width
//                        anchors.bottom: parent.bottom

                        BuySellTrading {
                            id: bt
                            anchors.fill: parent
                        }
                    }

                }

//                Card {
//                    Layout.minimumWidth: parent.width * .10
//                    Layout.maximumWidth: parent.width * .30
////                    Layout.fillWidth: true

//                    TradingFloor {}
//                }

//                Card {
//                    Layout.minimumWidth: parent.width * .10
//                    Layout.maximumWidth: parent.width * .30
////                    Layout.fillWidth: true

//                    Portfolio {}
//                }
            }
        }
    }

    Component {
        id: handeldel

        Item {
            height: itema.height
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





