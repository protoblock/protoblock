import QtQuick 2.0
import QtQuick.Controls 1.4
//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1
//import Material.Styles 1.0
//import QtQuick.Controls.Styles 1.4
//import "../qml-components/TradingContextBanner.qml"
//import "../qml-components/*.qml"


Item {
//    property int week:
//        MiddleMan.theWeek === 0 || !stack || !stack.currentItem ? 0 :
//                          (stack.currentItem.objectName === "prevWeekS" ? MiddleMan.thePrevWeek : (stack.currentItem.objectName === "nextWeekS" ? MiddleMan.theNextWeek : MiddleMan.theWeek))
//    property string seasontext: MiddleMan.seasonString + " 2016 "
    property string seasontext: " 2017 Season Trading - " // MiddleMan.seasonString + " 2017 Season Trading "
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
            color: liveorresult === "Live" ? "green" : "red"//themeroot.theme.primaryColor
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
                        Orders {
                           anchors.centerIn: parent
                           anchors.fill: parent
                        }
                    }
                }

                Card {
                    id: popcard
                    Layout.minimumWidth: parent.width * 0
                    Layout.maximumWidth: parent.width * .25
                    Layout.fillWidth: false
                    height: parent.height * .80
                    enabled: wwww.ss
                    visible: wwww.ss

                    Card {
                        anchors.top: parent.top
                        height: parent.height
                        ListSymbolSearch {
                            id: lss
                            property alias ss: wwww.ss
                        }
                        //ROWTradingTable {}
                    }
                    onEnabledChanged: {
                    }
                }

                Card {
                    Layout.minimumWidth: parent.width * .50
                    Layout.maximumWidth: parent.width
                    Layout.fillWidth: true
                    height: parent.height
//                    backgroundColor: "grey"

                    Card {
                        id: wkt
                        height: parent.height - bcard.height
                        width: parent.width
//                        backgroundColor: wwww.ss ? "grey" : "transparent"

                        WkTradingTable {
                            id: wwww
                        }

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
            }
        }
    }


//                    backgroundColor: "transparent"
/*
                    ColumnLayout {
                        height: parent.height
                        anchors.top: parent.top

                        RowLayout {
    //                            width: parent.width
                            Layout.preferredHeight: ProtoScreen.guToPx(6)
                            spacing: 0
                            IconButton {
    //                                anchors.left: parent.left
    //                                anchors.top: parent.top
                                Layout.fillHeight: true
                                Layout.fillWidth: false
                                Layout.preferredWidth: ProtoScreen.guToPx(6)
                                height: parent.height
                                onClicked: {
                                    wwww.ss = false
                                }

                                size: ProtoScreen.guToPx(2.5)

                                action: Action {
                                    iconName: "awesome/minus"
                                    hoverAnimation: true
                                }
                            }

                            Rectangle {
                                id: rec
                                color: "green"
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                anchors.margins: ProtoScreen.guToPx(.25)
                                height: ProtoScreen.guToPx(6)
                               // width: parent.width - ProtoScreen.guToPx(6)
                                TextField {
                                    focus: true
                                    anchors.leftMargin: ProtoScreen.guToPx(.5)
                                    anchors.fill: parent

    //                                    id: symbsearch
                                    width: parent.width / 1.07
                                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                    placeholderText: "search symbol..."
                                    inputMethodHints: Qt.ImhNoPredictiveText;
                                    onTextChanged: {
                                        console.log("text " + displayText)
                                    }
                                    activeFocusOnPress: true
                                    onHoveredChanged: {
                                        console.log(" text hover" + activeFocus)
                                    }

                                    color: "black"
                                    enabled: true

                                }

                                //MouseArea { anchors.fill: parent; onClicked: { scope.focus = true } }
                            }

                        }
*//*
                        ListView {
                            focus: true
                            anchors.top: parent.top
                           // height: parent.height
    //                        model: MiddleMan.pPlayerSymbolsModel
                            delegate: Card {
                                anchors.fill: parent
                                Label {
                                    text: modelData
                                    Layout.fillHeight: true
                                    verticalAlignment: Text.AlignVCenter
                                    Layout.fillWidth: false
    //                                Layout.preferredWidth: (parent.width / 4) - 2
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                Label {
                                    text: "pid"
                                    Layout.fillHeight: true
                                    verticalAlignment: Text.AlignVCenter
                                    Layout.fillWidth: false
    //                                Layout.preferredWidth: (parent.width / 4) - 2
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                            header: TextField {
                                anchors.leftMargin: ProtoScreen.guToPx(.5)
                                width: parent.width
                                //height: parent.height
                                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                placeholderText: "search..."
                                inputMethodHints: Qt.ImhNoPredictiveText;
                                onTextChanged: {
                                }
                                enabled: true//false//styleData.column === 1
                                visible: true//false//styleData.column === 1
                                activeFocusOnPress: true
                            }
                            model: 10//MiddleMan.pPlayerSymbolsModel
                        }
//                    }
                        */

                //}


//                    LeaderProjView {
//                        visible: parent.visible
//                    }
//                }


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





