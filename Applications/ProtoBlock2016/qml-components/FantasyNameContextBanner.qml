import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0
import QtQuick.Layouts 1.1
import Material 1.0

import ProRotoQml.Theme 1.0


Card {
    id: thisroot
    property string lightgreen: "green"//"#c8ffc8"
    property string lightred: "red"//"#ffc8c8"
    property variant inplay: MiddleMan.pTradingPositionsModel
    property double recwidth: ProtoScreen.guToPx(20)
    property bool haveplayer: inplay && inplay.fantasyname !== ""
    property color dcolor: Material.Theme.light.textColor

    property variant mybalance: MiddleMan.pMyFantasyNameBalance

    Layout.fillWidth: true
//    Layout.fillWidth: true
//    anchors.horizontalCenter: parent.horizontalCenter
//    anchors.top: parent.top
//    width: !haveplayer ? 0 : parent.width
//    anchors.left: parent.left
    height: !haveplayer ? 0 : rl.height
//    flat: true
    elevation: 0
//    width: parent.width

    RowLayout {
//        enabled: inplay.playerid !== 0
        id: rl
        width: parent.width
        height: ProtoScreen.guToPx(8)
//        anchors.right: parent.right
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 4

        layoutDirection: Qt.RightToLeft

        //fantasyname
        Rectangle {
            Layout.preferredWidth: Math.max(pname2.width,cwc.width)
            Layout.minimumWidth: Math.max(pname2.width,cwc.width)
            Layout.preferredHeight: parent.height
            Layout.fillWidth: true
            border.width: 0
//            anchors.centerIn: parent
            Banner {
                Layout.fillWidth: true
                id: cwc
                fontSize: ProtoScreen.font(ProtoScreen.SMALL)
                bold: true
                anchors.left: parent.left
                text: "Protoblock Trader Account"
                width: ProtoScreen.guToPx(30)
                color: dcolor
                backgroundColor: haveplayer ? themeroot.theme.accentColor : "transparent"

//                width: pname2.width
                height: ProtoScreen.guToPx(3)
                anchors.top: parent.top
                anchrosHType: "left"
                elevation: 0
            }

            Material.Card {
                anchors.top: cwc.bottom
                height: parent.height - cwc.height
                Layout.fillWidth: true
                id: pname2
                width: fbl1.implicitWidth
//                flat: true
                elevation: 0
                Material.Label {
                    id: fbl1
                    text: !haveplayer ? "" : inplay.fantasyname
    //                anchors.fill: parent
    //                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    anchors {
//                        verticalCenter: parent.verticalCenter
                        centerIn: parent
    //                        left: teamicon.right
    //                        leftMargin: ProtoScreen.guToPx(.125)
                    }
                }
            }
        }

        //balances
        Card {
            Layout.fillWidth: true
            id: lrec
            border.width: 0
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
            Layout.minimumWidth: tgrid.implicitWidth
            Layout.preferredWidth: tgrid.implicitWidth
            Layout.alignment: Qt.AlignLeft
//            anchors.centerIn: parent

//            width: parent.width *.50
//            anchors.top: pname.bottom
//            anchors.right: parent.horizontalCenter
//            height: ProtoScreen.guToPx(15)
//            color: (inplay.hdiff !== 0 || inplay.ldiff !== 0 || inplay.bsdiff !== 0 || inplay.asdiff !== 0) ?  "lightgrey" : "transparent"
            GridLayout {
                id: tgrid
    //            width: parent.width

                anchors.centerIn: parent
                columns: 4
                rows: 2
    //            columnSpacing: ProtoScreen.guToPx(2)

                Material.Label {
                    text: "Net Bal:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: "Open PnL:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: "Settled Bal:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: "Leaderboard:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: !mybalance ? "" : (mybalance.stake + (!inplay ? 0 : inplay.totalopenpnl)).toLocaleString() + " ƑɃ"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                    color: !inplay ? Material.Theme.light.textColor : inplay.bsdiff === 1 ? lightgreen :
//                                             inplay.bsdiff === -1 ? lightred :  Material.Theme.light.textColor

                }

                Material.Label {
                    text: !inplay ? 0 : ((inplay.totalopenpnl > 0 ? "+" : "") + inplay.totalopenpnl).toLocaleString() + " ƑɃ"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: (!inplay || inplay.totalopenpnl === 0) ? Material.Theme.light.textColor : inplay.totalopenpnl > 0 ? "green" : "red"
                }

                Material.Label {
                    text: !mybalance ? "" : (mybalance.stake).toLocaleString()  + " ƑɃ"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 4
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    textFormat: Text.RichText
                    text: !mybalance ? "" : (mybalance.bits).toLocaleString()  + " ƑɃ<sup>Skill</<sup>"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 4
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

            }
        }

     }
}
