import QtQuick 2.0
import QtQuick.Controls 1.4
//import QtQuick.Controls 2.2 as Controls

//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.3
//import Material.Styles 1.0
//import QtQuick.Controls.Styles 1.4
//import "../qml-components/TradingContextBanner.qml"
//import "../qml-components/*.qml"


Item {
//    property int week:
//        MiddleMan.theWeek === 0 || !stack || !stack.currentItem ? 0 :
//                          (stack.currentItem.objectName === "prevWeekS" ? MiddleMan.thePrevWeek :
//    (stack.currentItem.objectName === "nextWeekS" ? MiddleMan.theNextWeek : MiddleMan.theWeek))
//    property string seasontext: MiddleMan.seasonString + " 2016 "
    property string seasontext: "NFL Season " + MiddleMan.theSeason + " - Week " + MiddleMan.theWeek // MiddleMan.seasonString + " 2017 Season Trading "
    property string liveorresult: MiddleMan.liveSync
    property variant inplay: MiddleMan.pPlayerQuoteSliceModelItem

    property string wkorrow: "WEEK-" + MiddleMan.theWeek

    Component.onCompleted: {
        pageHelper.title = "Trading"
        console.log("trading completed")
     }

    id: tradingroot

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

        Rectangle {
//            height: cBan.height - ProtoScreen.guToPx(1)
            width: stack.lwidth
            anchors.left: parent.left
            anchors.margins: ProtoScreen.guToPx(1)
            color: "gray" // Theme.light.textColor//themeroot.theme.secondaryColor
//            radius: 2
            anchors.verticalCenter: cBan.verticalCenter
            visible: liveorresult === "Live"
            Layout.fillHeight: true
            Layout.fillWidth: false

            Label {
                anchors.centerIn: parent
                text: wkorrow + " FANTASY FUTURES";
                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                color: "green" //themeroot.theme.primaryColor //liveorresult === "Live" ?  :
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                id: futlabel
                font.bold: true
            }
        }


        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: seasontext + " " + liveorresult

            font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
            color: themeroot.theme.primaryColor //liveorresult === "Live" ?  :
            Layout.fillHeight: true
            Layout.fillWidth: false
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            id: cBan
        }

        Label {
            text: "TRADE: "
            color: "green"
            anchors.right: barrec.left
            anchors.margins: ProtoScreen.guToPx(.25)
            font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
            Layout.fillHeight: true
            Layout.fillWidth: false
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: barrec.verticalCenter
            font.bold: true
            id: tradeLbl
        }

        Rectangle {
            id: barrec
            height: cBan.height //- ProtoScreen.guToPx(1)
            width: ProtoScreen.guToPx(20)
            anchors.right: parent.right
            anchors.rightMargin: ProtoScreen.guToPx(2)
//            anchors.left: parent.left
            color: themeroot.theme.primaryColor // Theme.light.textColor//themeroot.theme.secondaryColor
//            radius: ProtoScreen.guToPx(.5)
            anchors.verticalCenter: cBan.verticalCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            TabBar {
                id: bar
                tabs: ["wk" + MiddleMan.theWeek ,"row"]
                centered: false
//                width: parent.width / 2.0
                darkBackground: true
                anchors.centerIn: parent
                anchors.fill: parent
//                anchors.margins: ProtoScreen.guToPx(.125)
                onSelectedIndexChanged: {
                    wkorrow = (tabs[selectedIndex] === "row") ? ("REST-OF-WAY") : ("WEEK-" + MiddleMan.theWeek)
                }
                Layout.fillHeight: false
                Layout.fillWidth: false
            }
        }

        SystemPalette { id: pal }

        StackLayout {
            currentIndex: bar.selectedIndex

            anchors.top: cBan.bottom
            anchors.topMargin: ProtoScreen.guToPx(.25)
            width: parent.width
            height: parent.height
            id: stack
            property real lwidth: (currentIndex === 0) ? tcbx.recwidth : tcbxW.recwidth

//                anchors.fill: parent

            Component.onCompleted: {
//               stack.push({item: rowTrading, properties:{objectName:"rowTrading"}})
//               stack.push({item: wkTrading, properties:{objectName:"wkTrading"}})

               console.log( " stack " + width)
            }

            Item {
                id: rowTrading
                width: parent.width
                height: parent.height

                TradingContextBanner {
                    id: tcbx
                    inplay: tradingroot.inplay
                    anchors.top: parent.top
                    Layout.preferredWidth: parent.width
                    anchors.left: parent.left
                    width: parent.width
                    anchors.leftMargin: ProtoScreen.guToPx(.25)
                    recwidth: dsplit.width
                }

                Item {
                    id: itema
                    anchors.top: tcbx.bottom
                    anchors.topMargin: ProtoScreen.guToPx(.25)
                    width: parent.width
                    height: parent.height - tcbx.height - cBan.height

                    SplitView {
                        anchors.fill: parent
                        orientation: Qt.Horizontal
                        handleDelegate: handeldel
        //                width: parent.width
        //                height: parent.height

                        SplitView {
                            id: dsplit
                            Layout.minimumWidth: parent.width * .10
                            Layout.maximumWidth: parent.width * .50
                            orientation: Qt.Vertical
                            height: parent.height
        //                    Layout.preferredWidth: ProtoScreen.guToPx(40)

                            Card {
                                Layout.maximumHeight: parent.height * .70
                                Layout.minimumHeight: parent.height * .30
                                backgroundColor: "#f5f5f5"
                                Layout.fillHeight: true
                                //Layout.preferredHeight: ProtoScreen.guToPx(80)

                                MarketDepthTable {}
        //                       Orders {
        //                          anchors.centerIn: parent
        //                          anchors.fill: parent
        //                       }
                            }

                            Card {
                                Layout.maximumHeight: parent.height * .70
                                Layout.minimumHeight: parent.height * .30
                                //MarketDepthTable {}
                                Orders {
                                    mysymbol: !inplay ? "" : inplay.symbol
                                }

                            }

                        }

                        Card {
                            id: popcard
                            Layout.minimumWidth: parent.width * 0
                            Layout.maximumWidth: parent.width * .25
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

                            Card {
                                id: wkt
                                height: parent.height - bcard.height
                                width: parent.width

                                WkTradingTable {
                                    id: wwww
                                }
                            }

                            Card {
                                anchors.top: wkt.bottom
                                id: bcard
                                height: ProtoScreen.guToPx(16)
                                width: parent.width

                                BuySellTrading {
                                    id: bt
                                    anchors.fill: parent
                                }
                            }

                        }
                    }
                }
            }

            Item {
                id: wkTrading
                width: parent.width
                height: parent.height

                TradingContextBanner {
                    id: tcbxW
                    inplay: tradingroot.inplay
                    anchors.top: parent.top
                    Layout.preferredWidth: parent.width
                    anchors.left: parent.left
                    width: parent.width
                    anchors.leftMargin: ProtoScreen.guToPx(.25)
                    recwidth: dsplitW.width
                }

                Item {
                    id: itemaW
                    anchors.top: tcbxW.bottom
                    anchors.topMargin: ProtoScreen.guToPx(.25)
                    width: parent.width
                    height: parent.height - tcbxW.height

                    SplitView {
                        anchors.fill: parent
                        orientation: Qt.Horizontal
                        handleDelegate: handeldel
        //                width: parent.width
        //                height: parent.height

                        SplitView {
                            id: dsplitW
                            Layout.minimumWidth: parent.width * .10
                            Layout.maximumWidth: parent.width * .50
                            orientation: Qt.Vertical
                            height: parent.height
        //                    Layout.preferredWidth: ProtoScreen.guToPx(40)

                            Card {
                                Layout.maximumHeight: parent.height * .70
                                Layout.minimumHeight: parent.height * .30
                                backgroundColor: "#f5f5f5"
                                Layout.fillHeight: true
                                //Layout.preferredHeight: ProtoScreen.guToPx(80)

                                MarketDepthTable {}
        //                       Orders {
        //                          anchors.centerIn: parent
        //                          anchors.fill: parent
        //                       }
                            }

                            Card {
                                Layout.maximumHeight: parent.height * .70
                                Layout.minimumHeight: parent.height * .30
                                //MarketDepthTable {}
                                Orders {
                                    mysymbol: !inplay ? "" : inplay.symbol
                                }

                            }

                        }

                        Card {
                            id: popcardW
                            Layout.minimumWidth: parent.width * 0
                            Layout.maximumWidth: parent.width * .25
                            height: parent.height * .80
                            enabled: wwwwW.ss
                            visible: wwwwW.ss

                            Card {
                                anchors.top: parent.top
                                height: parent.height
                                ListSymbolSearch {
                                    id: lssW
                                    property alias ss: wwwwW.ss
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

                            Card {
                                id: wktW
                                height: parent.height - bcardW.height
                                width: parent.width

                                WkTradingTable {
                                    id: wwwwW
                                }
                            }

                            Card {
                                anchors.top: wktW.bottom
                                id: bcardW
                                height: ProtoScreen.guToPx(16)
                                width: parent.width

                                BuySellTrading {
                                    id: btW
                                    anchors.fill: parent
                                }
                            }

                        }
                    }
                }

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





