import QtQuick 2.0
import QtQuick.Controls 1.4
//import QtQuick.Controls 2.2 as Controls

//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.3

Item {
    id: trcomp
    width: parent.width
    height: parent.height

    property variant quoteitem: undefined
    property alias  tradepos: tcb.inplayf
    property alias depthmodel: mdt.depthmm
//    property alias  leftwidth: tcb.recwidth
    property alias leftwidth: depths.width;
    property alias globalorders: orders.globalorders;
    property alias quoteproxymodel: wwww.quotemodel
    property bool isweekly

    TradingContextBanner {
        id: tcb
        inplay: quoteitem
        anchors.top: parent.top
        Layout.preferredWidth: parent.width
        anchors.left: parent.left
        width: parent.width
        anchors.leftMargin: ProtoScreen.guToPx(.25)
        recwidth: leftwidth
        weeklycontract: isweekly
//        inplayf: tpos
    }

    Item {
        id: itema
        anchors.top: tcb.bottom
        anchors.topMargin: ProtoScreen.guToPx(.25)
        width: parent.width
        height: parent.height - tcb.height

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
                    id: depths
                    Layout.maximumHeight: parent.height * .70
                    Layout.minimumHeight: parent.height * .30
                    backgroundColor: "#f5f5f5"
                    Layout.fillHeight: true
                    //Layout.preferredHeight: ProtoScreen.guToPx(80)

                    MarketDepthTable {
                        id: mdt
                        inplay: quoteitem
                    }
                }

                Card {
                    Layout.maximumHeight: parent.height * .70
                    Layout.minimumHeight: parent.height * .30
                    //MarketDepthTable {}
                    Orders {
                        id: orders
                        mysymbol: !quoteitem ? "" : quoteitem.symbol
//                        isweekly: isweekly
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
                    ListSymbolSearch  {
                        id: lss
                        wk: isweekly
                        property alias ss: wwww.ss
                    }
                    //ROWTradingTable {}
                }
            }

            Card {
                Layout.minimumWidth: parent.width * .50
                Layout.maximumWidth: parent.width
                Layout.fillWidth: true
                height: parent.height

                Card {
                    id: wkt
                    height: parent.height - ProtoScreen.guToPx(16)
                    width: parent.width

                    WkTradingTable {
                        id: wwww
                        anchors.fill: parent
                    }
                }

                Card {
                    anchors.bottom: parent.bottom
                    id: bcard
                    height: ProtoScreen.guToPx(16)
                    width: parent.width

                    BuySellTrading {
                        id: bt
                        anchors.fill: parent
                        inplay: quoteitem
                        isweekly: trcomp.isweekly
                    }
                }

            }
        }
    }

    SystemPalette { id: pal }
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





