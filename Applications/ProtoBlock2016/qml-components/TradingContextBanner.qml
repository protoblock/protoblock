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
    property variant inplay: MiddleMan.pPlayerQuoteSliceModelItem
    property double recwidth: ProtoScreen.guToPx(20)
    property bool haveplayer: inplay && inplay.playerid !== ""
    property bool haveposition: inplay && inplay.myposition !== 0
    property color dcolor: Material.Theme.light.textColor


//    Layout.fillWidth: true
//    anchors.horizontalCenter: parent.horizontalCenter
//    anchors.top: parent.top
//    width: !haveplayer ? 0 : parent.width
//    anchors.left: parent.left
    height: !haveplayer ? 0 : rl.height
//    flat: true
    elevation: !haveposition ? 0 : 3

    RowLayout {
//        enabled: inplay.playerid !== 0
        id: rl
//        width: parent.width
        height: ProtoScreen.guToPx(8)
        anchors.left: parent.left
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 4

        //name symbol
        Rectangle {
            Layout.preferredWidth: Math.max(recwidth * 2.0,pname.width)
            Layout.minimumWidth: Math.max(recwidth * 2.0,pname.width)
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
                text: " 2016 Week " + MiddleMan.theWeek + " Contract"
                color: "white"
                backgroundColor: haveplayer ? themeroot.theme.primaryColor : "transparent"

                width: pname.width
                height: ProtoScreen.guToPx(3)
                anchors.top: parent.top
                anchrosHType: "left"
                elevation: 0
            }

            Material.Card {
                Layout.fillWidth: true
                id: pname
                anchors.top: cwc.bottom
                backgroundColor:  "white"
                height: parent.height - cwc.height
                width: teamicon.size + pname2.implicitWidth + symb1.implicitWidth
                elevation: 0
                Row {
                    id: trow
                    anchors.verticalCenter: parent.verticalCenter
//                    anchors.centerIn: parent
//                    anchors.centerIn: parent
//                    width: teamicon.size + pname2.implicitWidth + symb1.implicitWidth //+ ProtoScreen.guToPx(.38)

                    Icon {
                        id: teamicon
                        hasColor:false
                        source: "qrc:/"+ inplay.teamid+".PNG"
                        width: ProtoScreen.guToPx(6)
//                        height: ProtoScreen.guToPx(6)
                        size: ProtoScreen.guToPx(6)
                        anchors {
                            verticalCenter: parent.verticalCenter
    //                        left: parent.left
    //                        leftMargin: ProtoScreen.guToPx(.125)
                        }
                    }

                    Material.Label {
                        Layout.fillWidth: true
                        id: pname2
                        text: !haveplayer ? "" : inplay.fullname + " (" + inplay.pos +") "
        //                anchors.fill: parent
        //                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        anchors {
                            verticalCenter: parent.verticalCenter
    //                        left: teamicon.right
    //                        leftMargin: ProtoScreen.guToPx(.125)
                        }
                    }

                    Material.Label {
                        Layout.fillWidth: true
                        id: symb1
                        text: !haveplayer ? "" : " Symbol: " + inplay.playerid
        //                anchors.fill: parent
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        anchors {
                            verticalCenter: parent.verticalCenter
    //                        left: pname2.right
    //                        leftMargin: ProtoScreen.guToPx(.125)
                        }
                    }
                }
            }
        }

        //last
        Rectangle {
            Layout.fillWidth: true
//            anchors.margins: 0
            Layout.fillHeight: true
            border.color: "red" //"lightgrey"
            border.width: ProtoScreen.guToPx(.125)
            Layout.preferredWidth: lr.implicitWidth
            Layout.maximumWidth: recwidth
            anchors.verticalCenter: parent.verticalCenter
//            width: lr.width
//            anchors.centerIn: parent


//            width: parent.width *.50
//            anchors.top: pname.bottom
//            anchors.left: parent.horizontalCenter
            height: parent.height
            color: inplay.vdiff !== 0 ?  "lightgrey" : "transparent"

            Row {
                anchors.left: parent.left
//                anchors.verticalCenter: parent.verticalCenter
                anchors.centerIn: parent
                id: lr
//                anchors.centerIn: parent
                width: l1.implicitWidth + l2.implicitWidth // + ProtoScreen.guToPx(.25)

                Material.Label {
                    id: l1
        //                width: parent.width * .50
        //                height: parent.height * .50
                    text: inplay.playerid === 0 ?  "" : "Last: "
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.verticalCenter: parent.verticalCenter
        //                    horizontalAlignment: Text.AlignRight
//                    anchors.leftMargin: ProtoScreen.guToPx(.125)

                }

                Material.Label{
                    id: l2
//                    id: lastval
                    text: !haveplayer ?  "" : inplay.lastprice
        //                Layout.fillHeight: true
        //                Layout.fillWidth:  false
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
        //                    color: icon.color
    //                Layout.column: 3
    //                Layout.row: 1
    //    //                    Layout.columnSpan: 2
    //                Layout.rowSpan: 2
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
//                    anchors.rightMargin: ProtoScreen.guToPx(.125)
                    color: (!haveplayer || inplay.lastprice === 0) ? "transparent" : dcolor
                }
            }
        }
/*
        Rectangle {
            border.color: "lightgrey"
            border.width: ProtoScreen.guToPx(.125)
            width: ProtoScreen.guToPx(20)

//            width: parent.width *.50
//            anchors.top: pname.bottom
//            anchors.left: parent.horizontalCenter
            height: lrec.height
            color: inplay.vdiff !== 0 ?  "lightgrey" : "transparent"

            GridLayout {
                anchors.centerIn: parent
                columns: 3
                rows: 3

                Material.Label {
    //                width: parent.width * .50
    //                height: parent.height * .50
                    text: "Last:"
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight
                    Layout.column: 1
                    Layout.row: 1
                    Layout.columnSpan: 2
                    Layout.rowSpan: 2
                }

                Material.Label {
    //                width: parent.width * .50
    //                height: parent.height * .50
                    text: "Change:"
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight
                    Layout.column: 1
                    Layout.row: 3
                    Layout.columnSpan: 2
//                    Layout.rowSpan: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label{
                    id: lastval
                    text: inplay.lastprice === 0 ?  "" : inplay.lastprice
    //                Layout.fillHeight: true
    //                Layout.fillWidth:  false
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignLeft
//                    color: icon.color
                    Layout.column: 3
                    Layout.row: 1
//                    Layout.columnSpan: 2
                    Layout.rowSpan: 2
                }

                Material.Label{
                    id: change
                    text: ((inplay.change > 0) ? "+" : "" ) + inplay.change.toString() + " "
    //                Layout.fillHeight: true
    //                Layout.fillWidth:  false
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignLeft
    //                color: icon.color
                    Layout.column: 3
                    Layout.row: 3
//                    Layout.columnSpan: 2
//                    Layout.rowSpan: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: { inplay.change < 0 ? Colors.red :
                             inplay.change > 0 ? Colors.green : Colors.black
                    }
                }
            }
        }
*/
        //bid-ask-hi-lo
        Rectangle {
            Layout.fillWidth: true
            id: lrec
            border.color: "green"//"lightgrey"
            border.width: ProtoScreen.guToPx(.125)
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
            Layout.minimumWidth: tgrid.implicitWidth
            Layout.preferredWidth: tgrid.implicitWidth
//            anchors.centerIn: parent

//            width: parent.width *.50
//            anchors.top: pname.bottom
//            anchors.right: parent.horizontalCenter
//            height: ProtoScreen.guToPx(15)
            color: (inplay.hdiff !== 0 || inplay.ldiff !== 0 || inplay.bsdiff !== 0 || inplay.asdiff !== 0) ?  "lightgrey" : "transparent"
            GridLayout {
                id: tgrid
    //            width: parent.width

                anchors.centerIn: parent
                columns: 6
                rows: 2
    //            columnSpacing: ProtoScreen.guToPx(2)

                Material.Label {
                    text: "Bid:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: "Ask:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: inplay.bid === 0 ? "" : inplay.bid
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: !inplay ? Material.Theme.light.textColor : inplay.bsdiff === 1 ? lightgreen :
                                             inplay.bsdiff === -1 ? lightred :  Material.Theme.light.textColor

                }

                Material.Label {
                    text: inplay.ask === 0 ? "" : inplay.ask
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: !inplay ? Material.Theme.light.textColor : inplay.adiff === 1 ? lightgreen :
                                              inplay.adiff === -1 ? lightred : Material.Theme.light.textColor

                }

                Material.Label {
                    text: "Volume:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 5
                    Layout.row: 1
//                    Layout.columnSpan: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: inplay.volume
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

//                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 6
                    Layout.row: 1
//                    Layout.columnSpan: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
    //                width: parent.width * .50
    //                height: parent.height * .50
                    text: "Change:"
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight
                    Layout.column: 5
                    Layout.row: 2
//                    Layout.columnSpan: 2
//                    Layout.rowSpan: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label{
                    id: change
                    text: ((inplay.change > 0) ? "+" : "" ) + inplay.change.toString() + " "
    //                Layout.fillHeight: true
    //                Layout.fillWidth:  false
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignLeft
    //                color: icon.color
                    Layout.column: 6
                    Layout.row: 2
//                    Layout.columnSpan: 2
//                    Layout.rowSpan: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: { inplay.change < 0 ? Colors.red :
                             inplay.change > 0 ? Colors.green : Colors.black
                    }
                }

                Material.Label {
                    text: "High:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: "Low:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: inplay.hi
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 4
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: inplay.lo
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 4
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }
            }
        }

        //my positions
        Rectangle {
            Layout.fillWidth: true
//            anchors.margins: 0
            border.color: "orange" //"lightgrey"
            border.width: ProtoScreen.guToPx(.125)
//            Layout.preferredWidth: recwidth
            Layout.preferredWidth: pr.implicitWidth
//            anchors.centerIn: parent


//            width: parent.width *.50
//            anchors.top: pname.bottom
//            anchors.left: parent.horizontalCenter
            height: parent.height
//            color: inplay.vdiff !== 0 ?  "lightgrey" : "transparent"

            Row {
//                anchors.verticalCenter: parent.verticalCenter
                id: pr
                anchors.centerIn: parent
                width: p1.implicitWidth + p2.implicitWidth
//                anchors.leftMargin: ProtoScreen.guToPx(.125)
//                anchors.rightMargin: ProtoScreen.guToPx(.125)

                Material.Label {
                    id: p1
                    text: !haveplayer ? "" : "My Position: "
                    verticalAlignment: Text.AlignVCenter
                    Layout.alignment: Qt.AlignCenter
                    anchors.verticalCenter: parent.verticalCenter
        //                    horizontalAlignment: Text.AlignRight

                }

                Material.Label{
                    id: p2
//                    id: lastval
                    text: !haveposition ? "" : (inplay.myposition < 0) ? "Short(" + Math.abs(inplay.myposition) + ")" :
                                                                                 "Long(" + inplay.myposition + ")"
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
//                    font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
                    color: !haveposition ? dcolor : (inplay.myposition < 0) ? "red" : (inplay.myposition > 0) ? "green" : dcolor

                }
            }
        }

        //avg - pnl
        Rectangle {
            Layout.fillWidth: true
            id: posrec
            border.color: themeroot.theme.accentColor
            border.width: ProtoScreen.guToPx(.125)
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
            Layout.preferredWidth: posgrid.implicitWidth
//            anchors.centerIn: parent

//            width: parent.width *.50
//            anchors.top: pname.bottom
//            anchors.right: parent.horizontalCenter
//            height: ProtoScreen.guToPx(15)
            color: (inplay.hdiff !== 0 || inplay.ldiff !== 0 || inplay.bsdiff !== 0 || inplay.asdiff !== 0) ?  "lightgrey" : "transparent"
            GridLayout {
                id: posgrid
    //            width: parent.width

                anchors.centerIn: parent
                columns: 2
                rows: 2
    //            columnSpacing: ProtoScreen.guToPx(2)

                Material.Label {
                    text: "Average Price:"


                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                    color: !haveposition ? dcolor : themeroot.theme.accentColor
                }

                Material.Label {
                    text: "Profit / Loss:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: (!inplay || !haveposition) ? "" : inplay.myavg
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)

                }

                Material.Label {
                    text: !inplay ? "" : (inplay.mypnl > 0 ? "+" : "") + inplay.mypnl + " ƑɃ"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: !inplay ? dcolor : (inplay.mypnl < 0) ? "red" : (inplay.mypnl > 0) ? "green" : Material.Theme.light.textColor

                }


            }
        }

     }
}
