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


Material.Card {
    id: thisroot
    property string lightgreen: "green"//"#c8ffc8"
    property string lightred: "red"//"#ffc8c8"
    property variant inplay: MiddleMan.pPlayerQuoteSliceModelItem
    property double recwidth: ProtoScreen.guToPx(20)
    property bool haveplayer: inplay && inplay.symbol !== ""
    property bool haveposition: inplay && inplay.myposition !== 0
    property color dcolor: Material.Theme.light.textColor

    property variant inplayf: MiddleMan.pTradingPositionsModel
    property bool havefname: inplayf && inplayf.fantasyname !== ""
    property variant mybalance: MiddleMan.pMyFantasyNameBalance
//    Layout.fillWidth: true

//    Layout.fillWidth: true
//    anchors.horizontalCenter: parent.horizontalCenter
//    anchors.top: parent.top
//    width: !haveplayer ? 0 : parent.width
//    anchors.left: parent.left
    height: rl.height //!haveplayer ? 0 : rl.height
//    flat: true
    elevation: !haveposition ? 2 : 6
//    width: parent.width
//    border.color: !haveposition ? "transparent" : themeroot.theme.accentColor
//    border.width: !haveposition ? 0 : ProtoScreen.guToPx(.25)
    RowLayout {
//        enabled: inplay.playerid !== 0
        id: rl
        width: parent.width
        height: ProtoScreen.guToPx(8)
//        anchors.left: parent.left
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 10

        //name symbol
        Rectangle {
            Layout.preferredWidth: Math.max(recwidth * 2.0,pname.width)
            Layout.minimumWidth: Math.max(recwidth * 2.0,pname.width)
            Layout.preferredHeight: parent.height
            Layout.fillWidth: false
            border.width: 0
//            anchors.centerIn: parent
            Banner {
                Layout.fillWidth: false
                id: cwc
                fontSize: ProtoScreen.font(ProtoScreen.SMALL)
                bold: true
                anchors.left: parent.left
                text: " 2017 Season Contract (" + inplay.symbol + ")"
                backgroundColor: "white"
                color: themeroot.theme.primaryColor
                helpShown: true
                helperHeader: inplay.fullname + " (" + inplay.position + ") Season Contract"
                helperTxt: "16 games 16 weeks. Season long contracts settle at the total points from a 16 game season. " +
                            "the season is from week1-week16. (week 16 is counted twice instead of using week 17)." +
                            "These expire at the total fantasy points scored by " + inplay.fullname +
                            ". The \"Writer\" (seller) must give the \"Buyer\", fantasy bits equal to the amount of fantsy points scored " +
                            "It is 1 Fantasy Bit per Fantasy Point in these season long contracts." +
                            "The game is to try to buy below the actual settlement number," +
                            "or alternativly to write (sell) at a price above the actual final number. " +
                            "With all the known risks, how much are you willing to \"pay\" for the contract? " +
                            "For how much would you be willing to " +
                            " write, or sell, the contract, knowing that you keep all the points in cae of injury," +
                            " but have to pay up in case of a breakout?"

                width: parent.width
                height: ProtoScreen.guToPx(3)
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchrosHType: "center"
                elevation: 0
            }

            Material.Card {
                Layout.fillWidth: true
                id: pname
                anchors.top: cwc.bottom
                backgroundColor:  "white"
                height: parent.height - cwc.height
                width: parent.width // teamicon.width + pname2.implicitWidth
//                anchors.horizontalCenter: parent.horizontalCenter
                elevation: 0
                RowLayout {
                    id: trow
//                    anchors.verticalCenter: parent.verticalCenter
                    anchors.centerIn: parent
//                    anchors.centerIn: parent
//                    width: teamicon.size + pname2.implicitWidth + symb1.implicitWidth //+ ProtoScreen.guToPx(.38)

                    Icon {
                        Layout.fillWidth: false
                        id: teamicon
                        hasColor:false
                        source: "qrc:/"+ inplay.teamid+".PNG"
//                        width: ProtoScreen.guToPx(6)
//                        height: ProtoScreen.guToPx(6)
//                        size: ProtoScreen.guToPx(3)
                        anchors {
                            verticalCenter: parent.verticalCenter
    //                        left: parent.left
                            leftMargin: ProtoScreen.guToPx(.125)
                        }
                    }

                    Material.Label {
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        id: pname2
                        text: !haveplayer ? "" : inplay.fullname + " (" + inplay.pos +") "
        //                anchors.fill: parent
                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        anchors {
                            verticalCenter: parent.verticalCenter
    //                        left: teamicon.right
                            leftMargin: ProtoScreen.guToPx(.125)
                        }
                    }

//                    Material.Label {
//                        Layout.fillWidth: true
//                        elide: Text.ElideRight
//                        id: symb1
//                        text: !haveplayer ? "" : " - " + inplay.symbol
//        //                anchors.fill: parent
//                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                        verticalAlignment: Text.AlignVCenter
//                        horizontalAlignment: Text.AlignHCenter

//                        anchors {
//                            verticalCenter: parent.verticalCenter
//    //                        left: pname2.right
//    //                        leftMargin: ProtoScreen.guToPx(.125)
//                        }
//                    }
                }
            }
        }


        //last my pos - pnl
        Card {
            Layout.fillWidth: true
            id: lspo
//            border.color: themeroot.theme.accentColor
//            border.width: ProtoScreen.guToPx(.125)
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
            Layout.preferredWidth: lposgrid.implicitWidth
            anchors.margins: ProtoScreen.guToPx(.125)
//            anchors.centerIn: parent

//            width: parent.width *.50
//            anchors.top: pname.bottom
//            anchors.right: parent.horizontalCenter
//            height: ProtoScreen.guToPx(15)
            backgroundColor: (inplay.ldiff !== 0 ) ?  "lightgrey" : "white"
//            elevation: 3
            GridLayout {
                id: lposgrid
    //            width: parent.width

                anchors.centerIn: parent
                columns: 2
                rows: 2
    //            columnSpacing: ProtoScreen.guToPx(2)

                Material.Label {
                    text: "Last:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                    color: !haveposition ? dcolor : themeroot.theme.accentColor
                }

                Material.Label {
                    text: "Pos:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: !haveplayer ?  "" : inplay.lastprice
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    font.bold: true
                }

                Material.Label {
                    text: !haveposition ? "" : (inplay.myposition < 0) ? "Short(" + Math.abs(inplay.myposition) + ")" :
                                                                                 "Long(" + inplay.myposition + ")"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: !haveposition ? dcolor : (inplay.myposition < 0) ? "red" : (inplay.myposition > 0) ? "green" : dcolor

                }


            }
        }

        //last
//        Rectangle {
//            Layout.fillWidth: true
////            anchors.margins: 0
//            Layout.fillHeight: true
////            border.color: "red" //"lightgrey"
//            border.width: ProtoScreen.guToPx(.125)
//            Layout.preferredWidth: l2.implicitWidth
////            Layout.maximumWidth: recwidth
//            anchors.verticalCenter: parent.verticalCenter
////            width: lr.width
////            anchors.centerIn: parent


////            width: parent.width *.50
////            anchors.top: pname.bottom
////            anchors.left: parent.horizontalCenter
//            height: parent.height
//            color: inplay.vdiff !== 0 ?  "lightgrey" : "transparent"

////            Row {
////                anchors.left: parent.left
//////                anchors.verticalCenter: parent.verticalCenter
////                anchors.centerIn: parent
////                id: lr
//////                anchors.centerIn: parent
////                width: l1.implicitWidth + l2.implicitWidth // + ProtoScreen.guToPx(.25)

////                Material.Label {
////                    id: l1
////        //                width: parent.width * .50
////        //                height: parent.height * .50
////                    text: inplay.playerid === 0 ?  "" : "Last: "
////                    verticalAlignment: Text.AlignVCenter
////                    horizontalAlignment: Text.AlignHCenter
////                    anchors.verticalCenter: parent.verticalCenter
////        //                    horizontalAlignment: Text.AlignRight
//////                    anchors.leftMargin: ProtoScreen.guToPx(.125)
////                    elide: Text.ElideRight

////                }

//                Material.Label{
//                    id: l2
////                    id: lastval
//                    text: !haveplayer ?  "" : inplay.lastprice
//        //                Layout.fillHeight: true
//        //                Layout.fillWidth:  false
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//        //                    color: icon.color
//    //                Layout.column: 3
//    //                Layout.row: 1
//    //    //                    Layout.columnSpan: 2
//    //                Layout.rowSpan: 2
//                    anchors.centerIn: parent
//                    font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
////                    anchors.rightMargin: ProtoScreen.guToPx(.125)
//                    color: (!haveplayer || inplay.lastprice === 0) ? "transparent" : dcolor
//                }
////            }
//        }
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
        Card {
            elevation: 0
            Layout.fillWidth: true
            id: lrec
//            border.color: "green"//"lightgrey"
//            border.width: ProtoScreen.guToPx(.125)
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
            Layout.minimumWidth: tgrid.implicitWidth
            Layout.preferredWidth: tgrid.implicitWidth
//            anchors.centerIn: parent

//            width: parent.width *.50
//            anchors.top: pname.bottom
//            anchors.right: parent.horizontalCenter
//            height: ProtoScreen.guToPx(15)
            backgroundColor: (inplay.hdiff !== 0 || inplay.ldiff !== 0 || inplay.bsdiff !== 0 || inplay.asdiff !== 0) ?  "lightgrey" : "white"
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
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: !inplay ? Material.Theme.light.textColor : inplay.adiff === 1 ? lightgreen :
                                              inplay.adiff === -1 ? lightred : Material.Theme.light.textColor

                }

                Material.Label {
                    text: "Vol:"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

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
                    text: "Chng:"
                    verticalAlignment: Text.AlignVCenter
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
                    verticalAlignment: Text.AlignVCenter
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
                    text: "Hi:"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: "Lo:"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: inplay.hi
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 4
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: inplay.lo
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 4
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }
            }
        }

//        //my positions
//        Rectangle {
//            Layout.fillWidth: true
////            anchors.margins: 0
//            border.color: "orange" //"lightgrey"
//            border.width: 0//ProtoScreen.guToPx(.125)
////            Layout.preferredWidth: recwidth
//            Layout.preferredWidth: pr.width
////            anchors.centerIn: parent


////            width: parent.width *.50
////            anchors.top: pname.bottom
////            anchors.left: parent.horizontalCenter
//            height: parent.height
////            color: inplay.vdiff !== 0 ?  "lightgrey" : "transparent"

//            Column {
////                anchors.verticalCenter: parent.verticalCenter
//                id: pr
//                anchors.centerIn: parent
//                height: parent.height

////                width: p1.implicitWidth + p2.implicitWidth
////                anchors.leftMargin: ProtoScreen.guToPx(.125)
////                anchors.rightMargin: ProtoScreen.guToPx(.125)

//                Material.Label {
//                    id: p1
//                    text: !haveplayer ? "" : "My Position"
//                    verticalAlignment: Text.AlignVCenter
//                    Layout.alignment: Qt.AlignCenter
////                    anchors.top: parent.top
//                    anchors.horizontalCenter: parent.horizontalCenter
//        //                    horizontalAlignment: Text.AlignRight

//                }

//                Material.Label{
//                    id: p2
////                    id: lastval
//                    text: !haveposition ? "" : (inplay.myposition < 0) ? "Short(" + Math.abs(inplay.myposition) + ")" :
//                                                                                 "Long(" + inplay.myposition + ")"
////                    verticalAlignment: Text.AlignVCenter
////                    anchors.top: p.verticalCenter
////                    font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
//                     anchors.horizontalCenter: parent.horizontalCenter
//                    color: !haveposition ? dcolor : (inplay.myposition < 0) ? "red" : (inplay.myposition > 0) ? "green" : dcolor

//                }
//            }
//        }

        //avg - pnl
        Card {
            Layout.fillWidth: true
            id: posrec
//            border.color: themeroot.theme.accentColor
//            border.width: ProtoScreen.guToPx(.125)
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
            Layout.preferredWidth: posgrid.implicitWidth
//            anchors.centerIn: parent

//            width: parent.width *.50
//            anchors.top: pname.bottom
//            anchors.right: parent.horizontalCenter
//            height: ProtoScreen.guToPx(15)
            backgroundColor: (inplay.hdiff !== 0 || inplay.ldiff !== 0 || inplay.bsdiff !== 0 || inplay.asdiff !== 0) ?  "lightgrey" : "white"
            GridLayout {
                id: posgrid
    //            width: parent.width

                anchors.centerIn: parent
                columns: 2
                rows: 2
    //            columnSpacing: ProtoScreen.guToPx(2)

                Material.Label {
                    text: "Avg Price:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                    color: !haveposition ? dcolor : themeroot.theme.accentColor
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
                    text: (!inplay || !haveposition) ? "" : parseFloat(inplay.myavg).toFixed(2);
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)

                }

                Material.Label {
                    text: !inplay ? "" : (inplay.mypnl > 0 ? "+" : "") + inplay.mypnl + " ƑɃ"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: !inplay ? dcolor : (inplay.mypnl < 0) ? "red" : (inplay.mypnl > 0) ? "green" : Material.Theme.light.textColor

                }


            }
        }

        //fname
//        Rectangle {
//            enabled: false
//            Layout.preferredWidth: cwc2.width//Math.max(fname.width,cwc2.width)
//            Layout.minimumWidth: cwc2.width//Math.max(fname.width,cwc2.width)
//            Layout.preferredHeight: parent.height
//            Layout.fillWidth: true
//            border.width: 0
//            Layout.alignment: Qt.AlignRight
////            anchors.centerIn: parent
//            Card {
//                Layout.fillWidth: true
//                id: cwc2
//                anchors.left: parent.left
////                width: ProtoScreen.guToPx(28)
//                backgroundColor: havefname ? themeroot.theme.accentColor : "white"

//                width: Math.max(fname.width,cwc2b.width)
//                height: ProtoScreen.guToPx(3)
//                anchors.top: parent.top
//                elevation: 0
//                Material.Label {
//                    id: cwc2b
//                    text: "Protoblock Trader Account"
//    //                anchors.fill: parent
//    //                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter

//                    anchors {
//                        verticalCenter: parent.verticalCenter
////                        centerIn: parent
//    //                        left: teamicon.right
//    //                        leftMargin: ProtoScreen.guToPx(.125)
//                    }
//                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                }

//            }

//            Material.Card {
//                anchors.top: cwc2.bottom
//                height: parent.height - cwc2.height
//                Layout.fillWidth: true
//                id: fname
//                width: fbl1.implicitWidth
////                flat: true
//                elevation: 0
//                Material.Label {
//                    id: fbl1
//                    text: !havefname ? "" : inplayf.fantasyname
//    //                anchors.fill: parent
//    //                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter

//                    anchors {
////                        verticalCenter: parent.verticalCenter
//                        centerIn: parent
//    //                        left: teamicon.right
//    //                        leftMargin: ProtoScreen.guToPx(.125)
//                    }
//                }
//            }
//        }

        Material.Card {
            elevation: 0
            height: parent.height
            Layout.fillWidth: false
            id: fnameCARD
            Layout.preferredWidth: fbl12.width + ProtoScreen.guToPx(.5)
            anchors.rightMargin: ProtoScreen.guToPx(.25)
            anchors.leftMargin: ProtoScreen.guToPx(.25)
//            width: fbl1.implicitWidth
//           elevation: 0
            Material.Label {
                id: fbl12
                text: !havefname ? "" : inplayf.fantasyname
            //                anchors.fill: parent
                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight

                anchors {
//                    verticalCenter: parent.verticalCenter
//                    right: parent.right
//                    rightMargin: ProtoScreen.guToPx(.25)
                    centerIn: parent
            //                        left: teamicon.right
            //                        leftMargin: ProtoScreen.guToPx(.125)
                }
            }
        }

        //balances
        Card {
            Layout.fillWidth: true
            id: lrec2
            border.width: 0
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
            Layout.minimumWidth: tgrid2.implicitWidth
            Layout.preferredWidth: tgrid2.implicitWidth
            Layout.alignment: Qt.AlignLeft
//            anchors.centerIn: parent

//            width: parent.width *.50
//            anchors.top: pname.bottom
//            anchors.right: parent.horizontalCenter
//            height: ProtoScreen.guToPx(15)
//            color: (inplay.hdiff !== 0 || inplay.ldiff !== 0 || inplay.bsdiff !== 0 || inplay.asdiff !== 0) ?  "lightgrey" : "transparent"
            GridLayout {
                id: tgrid2
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
                    text: !mybalance ? "" : (mybalance.stake + (!inplayf ? 0 : inplayf.totalopenpnl)).toLocaleString() + " ƑɃ"
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
                    text: !inplayf ? 0 : ((inplayf.totalopenpnl > 0 ? "+" : "") + inplayf.totalopenpnl).toLocaleString() + " ƑɃ"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: (!inplayf || inplayf.totalopenpnl === 0) ? Material.Theme.light.textColor : inplayf.totalopenpnl > 0 ? "green" : "red"
                }

                Material.Label {
                    text: !mybalance ? "" : (mybalance.stake).toLocaleString()  + " ƑɃ"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignLeft
                    Layout.column: 4
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    textFormat: Text.RichText
                    text: !mybalance ? "" : (mybalance.bits).toLocaleString()  + " ƑɃ<sup>Skill</<sup>"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignLeft
                    Layout.column: 4
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

            }
        }

     }
}
