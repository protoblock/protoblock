import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import Material.ListItems 1.0 as ListItems
import QtQuick.Controls 1.4

//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
//import ProRotoQml.Models 1.0
import QtQuick.Controls.Styles 1.4
import Material.Styles 1.0

import Material 1.0 as Material


Item {
    Component.onCompleted: {
         pageHelper.title = "Trading"
        console.log(" on completed  Trading ")
    }

    property variant inplayf: MiddleMan.pTradingPositionsModel
    property variant mybalance: MiddleMan.pMyFantasyNameBalance

    Banner {
        backgroundColor: "white"
        color: themeroot.theme.primaryColor
        anchrosHType: "center"
        text: "2017 Season Trading"
        id: mainTitle
        width: parent.width
        elevation: 0
    }

    SwipeView {
        id: swipeView
        anchors.top: mainTitle.bottom
        height: parent.height - mainTitle.height
        width: parent.width
        currentIndex: 0

        ColumnLayout {
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width
            spacing: 0
            Material.Card {
                z: 2
                flat: true
                Layout.preferredHeight: ProtoScreen.guToPx(8)
                Layout.fillWidth: true
                Layout.fillHeight: false
                backgroundColor:  "white"
                id: lrec2
                border.width: 0
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                width: parent.width
                GridLayout {
                    id: tgrid2
                    anchors.centerIn: parent
                    columns: 4
                    rows: 2
                    Material.Label {
                        text: "Net Bal:"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight

                        Layout.alignment: Qt.AlignRight
                        Layout.column: 1
                        Layout.row: 1
                        font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                    }

                    Material.Label {
                        text: "Open PnL:"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight

                        Layout.alignment: Qt.AlignRight
                        Layout.column: 1
                        Layout.row: 2
                        font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                    }

                    Material.Label {
                        text: "Settled Bal:"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight

                        Layout.alignment: Qt.AlignRight
                        Layout.column: 3
                        Layout.row: 1
                        font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                    }

                    Material.Label {
                        text: "Leaderboard:"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight

                        Layout.alignment: Qt.AlignRight
                        Layout.column: 3
                        Layout.row: 2
                        font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                    }

                    Material.Label {
                        text: !mybalance ? "" : (mybalance.stake + (!inplayf ? 0 : inplayf.totalopenpnl)).toLocaleString() + " ƑɃ"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft

                        Layout.alignment: Qt.AlignLeft
                        Layout.column: 2
                        Layout.row: 1
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        font.bold: true
                    }

                    Material.Label {
                        text: !inplayf ? 0 : ((inplayf.totalopenpnl > 0 ? "+" : "") + inplayf.totalopenpnl).toLocaleString() + " ƑɃ"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft

                        Layout.alignment: Qt.AlignLeft
                        Layout.column: 2
                        Layout.row: 2
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        color: (!inplayf || inplayf.totalopenpnl === 0) ? Material.Theme.light.textColor : inplayf.totalopenpnl > 0 ? "green" : "red"
                    }

                    Material.Label {
                        text: !mybalance ? "" : (mybalance.stake).toLocaleString()  + " ƑɃ"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft

                        Layout.alignment: Qt.AlignLeft
                        Layout.column: 4
                        Layout.row: 1
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    }

                    Material.Label {
                        textFormat: Text.RichText
                        text: !mybalance ? "" : (mybalance.bits).toLocaleString()  + " ƑɃ<sup>Skill</<sup>"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft

                        Layout.alignment: Qt.AlignLeft
                        Layout.column: 4
                        Layout.row: 2
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    }

                }
            }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: parent.height - lrec2.height
                Layout.preferredWidth: parent.width
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter

                id: playersListView
                model: MiddleMan.pPlayerQuoteSliceModel

                headerPositioning: ListView.OverlayHeader

                header: Item {
                    width: parent.width
                    height: ProtoScreen.guToPx(4)
                    z: 2
//                    property real totalw: 1.0/9.0
//                    property var widths: [1.5*totalw,4.0*totalw,1.75*totalw,1.75*totalw]
                    property real totalw: 1.0/10.0
                    property var widths: [1.25*totalw,3.8*totalw,1.70*totalw,1.75*totalw,1.5*totalw]


                    Rectangle {
                        color: themeroot.theme.primaryColor
                        anchors.fill: parent
                        RowLayout {
                            id: r0
                            anchors.fill: parent
                            Layout.fillWidth: true
                            Layout.fillHeight: true
        //                    anchors.margins: ProtoScreen.guToPx(.1)
                            //pos team
                            Material.Label {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.preferredWidth: (parent.width * widths[0])
                                Layout.alignment: Qt.AlignCenter
                                text: "Pos"
                                font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                color: "white"
                                font.bold: true
                            }
                            Material.Label {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.preferredWidth: (parent.width * widths[1])
                                Layout.alignment: Qt.AlignCenter
                                text: "Player Name"
                                font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                color: "white"
                                font.bold: true

                            }
                            Material.Label {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.preferredWidth: (parent.width * widths[2])
                                Layout.alignment: Qt.AlignCenter
                                text: "Symbol"
                                font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                color: "white"
                                font.bold: true

                            }

                            Material.Label {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.preferredWidth: (parent.width * widths[3])
                                Layout.alignment: Qt.AlignCenter
                                text: "Holdings"
                                font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                color: "white"
                                font.bold: true

                            }
                            Material.Label {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.preferredWidth: (parent.width * widths[4])
                                Layout.alignment: Qt.AlignCenter
                                text: "Price"
                                font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                color: "white"
                                font.bold: true

                            }
                        }
                    }
                }
                delegate: Item {
                    width: parent.width
                    height: ProtoScreen.guToPx(8)
                    property real totalw: 1.0/10.0
                    property var widths: [1.25*totalw,3.8*totalw,1.70*totalw,1.75*totalw,1.5*totalw]
                    RowLayout {
                        id: rl
                        anchors.fill: parent
                        Layout.fillWidth: true
                        Layout.fillHeight: true
    //                    anchors.margins: ProtoScreen.guToPx(.1)
                        //pos team
                        ColumnLayout {
                            height: parent.height
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.preferredWidth: (parent.width * widths[0])
                            Layout.alignment: Qt.AlignCenter
                            Material.Label {
                                text: pos
                                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                verticalAlignment: Text.AlignBottom
                                horizontalAlignment: Text.AlignHCenter
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                            }

                            Material.Label {
                                text: teamid
                                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                verticalAlignment: Text.AlignTop
                                horizontalAlignment: Text.AlignHCenter
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                ColorAnimation on color { to: TeamInfo.getPrimaryAt(teamid); duration: 10000 }
                                font.bold: true
                            }
                        }

                        //last first
                        ColumnLayout {
                            height: parent.height
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignCenter
                            Layout.preferredWidth: (parent.width * widths[1])
                            Material.Label {
                                text: lastname + ","
                                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                verticalAlignment: Text.AlignBottom
                                horizontalAlignment: Text.AlignLeft
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                            }

                            Material.Label {
                                text: firstname
                                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                verticalAlignment: Text.AlignTop
                                horizontalAlignment: Text.AlignLeft
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                            }
                        }

                        //symbol
//                        Material.Card {
//                            backgroundColor: "transparent"
                            Material.Label {
                                height: parent.height / 2.0
                                Layout.fillWidth: true
                                Layout.fillHeight: false
                                Layout.preferredWidth: (parent.width * widths[2])
                                Layout.alignment: Qt.AlignCenter

                                anchors.centerIn: parent
                                text: symbol
                                font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                                verticalAlignment: Text.AlignHCenter
                                horizontalAlignment: Text.AlignLeft
                            }
//                        }


                        //pos pnl
                        ColumnLayout {
                            height: parent.height
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.preferredWidth: (parent.width * widths[3])
                            Layout.alignment: Qt.AlignCenter

                            Material.Label {
                                text: !myposition ? "" : myposition === 0 ? "" :
                                      (myposition + " @ " + parseFloat(myavg).toFixed(2))
                                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                verticalAlignment: Text.AlignBottom
                                horizontalAlignment: Text.AlignLeft
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                            }

                            Material.Label {
                                text: !mypnl ? "" : mypnl === 0 ? "" : (mypnl > 0 ? "+" : "") + (mypnl + " ƑɃ")
                                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                verticalAlignment: Text.AlignTop
                                horizontalAlignment: Text.AlignLeft
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                color: mypnl === "" ? "transparant" : mypnl < 0 ? "red" : "green"
                            }
                        }

                        //last change
                        ColumnLayout {
                            height: parent.height
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.preferredWidth: (parent.width * widths[4])
                            Layout.alignment: Qt.AlignCenter

                            Material.Label {
                                text: lastprice + ((updown < 0) ? " ↓" : (model.updown > 0) ? " ↑" : "");
                                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                verticalAlignment: Text.AlignBottom
                                horizontalAlignment: Text.AlignLeft
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                            }

                            Material.Label {
                                text: change === 0 ? "unch" : (change > 0 ? "+" : "") +change
                                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                verticalAlignment: Text.AlignTop
                                horizontalAlignment: Text.AlignLeft
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                color: (!change || change === 0) ? Material.Theme.light.textColor
                                                                 : change < 0 ? "red" : "green"
                            }
                        }
                    }

                    Rectangle{anchors.bottom: rl.top; width: parent.width; color: "light grey" ;height: ProtoScreen.guToPx(.1)}
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            MiddleMan.startDepth(model.playerid)
    //                        rootLoader.source = "qrc:/TradingFloor.qml"
                              swipeView.currentIndex = 1
                        }
                    }
                }

            }
        }

        GridLayout {
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width
//            spacing: 0
            columns: height > width ?  1 : 2
            rows: height > width ?  2 : 1
            flow: (parent.height > parent.width) ? GridLayout.TopToBottom : GridLayout.LeftToRight


            Material.Card {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.width
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                id: md
                MarketDepthTable {
                    Layout.fillWidth: true
                    width: parent.width
                }
            }

            Material.Card {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.width
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                backgroundColor: "#f5f5f5"
                TabView {
                    anchors.fill: parent
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Tab {
                        title: "Orders"
                        Orders {}
                    }
                    Tab {
                        title: "Buy"
                        BuyTab {}
                    }
                    Tab {
                        title: "Sell"
                        BuyTab {
                            isbuy: false
                        }
                    }

                    style: TabViewStyle {
                        frameOverlap: ProtoScreen.guToPx(.25)
                        tabsAlignment: Qt.AlignHCenter
                        tabOverlap: ProtoScreen.guToPx(.25)
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
                            implicitHeight: ProtoScreen.guToPx(4)
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
    }

    PageIndicator {
        id: indicator
        count: swipeView.count
        currentIndex: swipeView.currentIndex

        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        interactive: true
    }

}


