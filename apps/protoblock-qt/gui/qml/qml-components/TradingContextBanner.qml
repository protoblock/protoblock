import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProtoblockQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0
import QtQuick.Layouts 1.1
//import Material 1.0

import ProtoblockQml.Theme 1.0


Material.Card {
    id: thisroot

//    flat: true
    backgroundColor: "white"
    property variant inplayf: undefined
    property variant inplay: undefined


    property string lightgreen: "green"//"#c8ffc8"
    property string lightred: "red"//"#ffc8c8"
    property double recwidth: ProtoScreen.guToPx(20)
    property bool haveplayer: inplay && inplay.symbol !== ""
    property bool haveposition: inplay && inplay.myposition !== 0
    property color dcolor: Material.Theme.light.textColor

    property bool havefname: inplayf && inplayf.fantasyname !== ""
    property variant mybalance: MiddleMan.pMyFantasyNameBalance

//    property alias inplaytext: cwc.text

    property bool weeklycontract: true
    property string inplaytext: (weeklycontract ? ("Week " + tradingroot.theqmlweek ) : (tradingroot.theqmlseason + "  Season"))  + " Contract"

    property string helperTxtweekly:
                         "This contract settles at 100 times the total number of PPR fantasy points scored by "
                                    +  inplay.firstname + " " + inplay.lastname
                                     + " in Week " + tradingroot.theqmlweek
                  + ". If he scores 20 points, then the \"Writer\" (short seller) has to deliver 2000 Fantasybits to the Buyer.\n\n"
                 + "Knowing this, how much would you pay for this contract? "
                 + "If you think "+  inplay.firstname + " will score 30 points, and it costs you only 20, then "
                   +   themeroot.uname + " (you), would have a profit of 1000 Fantasybits!\n\n"
                  + "After the game, the buyer owes the seller 100 times the traded price per contract, "
                + "and the seller must \"deliver\" the total number of fantasy points scored by " + inplay.firstname + ",  to the buyer, in Fantasybits (x100),  per contract!\n\n" +
                  "If you can predict weekly busts, then you can now monetize your skills, in this first ever blockchain based, fantasy football forwards exchange!\n\n"
                  +"A good way to start, is, to place a Sell order (ask) at each players ceiling, and at the same time place a Buy order at each players floor. This is called \"passive market making\", where you are, quite literally,  getting paid for your Fantasy Football Skills!"

    property string helperTxtseason: "16 games in 16 weeks (16x16).\n\n"
                +"Season long contracts are Rest-of-Way contracts, and settle at the total PPR fantasy points " +
                " from a 16 game season. " +
                "The season runs from Week1-Week16. (Week16 is counted twice instead of using Week17).\n\n" +
                "These contract expire at the end of the season, and settle at the total PPR fantasy points scored by " + inplay.firstname + " " + inplay.lastname +
                ", for the \"Rest-of-the-Way\" this season.\n\n"
                +"The \"Writer\" (seller) must give the \"Holder\" (buyer), Fantasybits equal to the amount of PPR fantasy points scored. " +
                "It is 1 Fantasybit for each Fantasy Point, in these ROW contracts.\n\n" +
                "The idea is to try to buy below the actual settlement number, " +
                "or alternativly to \"Write\" (sell) contracts, at a price above the actual final number.\n\n" +
                "With all the known risks, how much are you willing to \"pay\" (bid) for the contract? " +
                "At what price would you be willing to " +
                " \"write\" the contract? In other words, what is your \"ask\", knowing that you get keep all the points in case of injury or bust? " +
                "While keeping in mind, the risk that you would have to pay the total points scored in the case of a breakout!"


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
        spacing: ProtoScreen.guToPx(.01)

        //name symbol
        Rectangle {
            Layout.preferredWidth: recwidth
            //Layout.minimumWidth: pname.width)
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
                text: inplaytext + "  (" + inplay.symbol + ")    "
                backgroundColor: "white"
                color: themeroot.theme.primaryColor
                helpShown: true
                helperHeader: inplay.fullname + " (" + inplay.pos + ") " + text
                helperTxt: weeklycontract ? helperTxtweekly : helperTxtseason


                width: parent.width
                height: ProtoScreen.guToPx(3)
                anchors.top: parent.top
                //anchors.horizontalCenter: parent.horizontalCenter
                anchrosHType: "center"
                elevation: 0
            }

            Material.Card {
                Layout.fillWidth: true
                id: pname
                anchors.top: cwc.bottom
                backgroundColor: "white"// (!inplay || inplay.teamid === "" ) ? "white" : TeamInfo.getPrimaryAt(inplay.teamid)//themeroot.theme.accentColor//"white"
                height: parent.height - cwc.height
                width: parent.width // teamicon.width + pname2.implicitWidth
//                anchors.horizontalCenter: parent.horizontalCenter
                elevation: 0
                RowLayout {
                    id: trow
//                    anchors.verticalCenter: parent.verticalCenter
                    anchors.fill: parent
//                    anchors.centerIn: parent
//                    width: teamicon.size + pname2.implicitWidth + symb1.implicitWidth //+ ProtoScreen.guToPx(.38)

                    Material.Icon {
                        Layout.fillWidth: false
                        id: teamicon
                        hasColor:false
                        source: "qrc:/"+ ((!inplay || inplay.teamid === "") ? "FA" :  inplay.teamid)+".PNG"
                        width: ProtoScreen.guToPx(5)
                        height: ProtoScreen.guToPx(5)
                        size: ProtoScreen.guToPx(3)
                        anchors {
//                            verticalCenter: parent.verticalCenter
    //                        left: parent.left
//                            leftMargin: ProtoScreen.guToPx(.125)
                        }
                        Layout.alignment: Qt.AlignVCenter
                    }

                    Material.Label {
                        Layout.fillWidth: true
                        //elide: Text.ElideRight
                        id: pname2
                        text: !haveplayer ? "" : inplay.fullname + " (" + inplay.pos +") "
        //                anchors.fill: parent
                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: (!inplay || inplay.teamid === "") ? themeroot.theme.primaryColor : TeamInfo.getPrimaryAt(inplay.teamid)
                        font.bold: true
                        anchors {
//                            verticalCenter: parent.verticalCenter
//                            left: teamicon.right
                            //leftMargin: ProtoScreen.guToPx(.125)
                        }
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
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
        Material.Card {
            Layout.fillWidth: true
            id: lspo
//            border.color: themeroot.theme.accentColor
//            border.width: ProtoScreen.guToPx(.125)
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
//            Layout.preferredWidth: lposgrid.implicitWidth + ProtoScreen.guToPx(.125)
//            anchors.margins: ProtoScreen.guToPx(.125)
//            anchors.centerIn: parent
            width: ProtoScreen.guToPx(18.5)

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
                    text: !inplay  ?  "" : inplay.lastprice
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignLeft

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
//                    horizontalAlignment: Text.AlignLeft

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    color: !haveposition ? dcolor : (inplay.myposition < 0) ? "red" : (inplay.myposition > 0) ? "green" : dcolor

                }


            }
        }

        //bid-ask-hi-lo
        Material.Card {
//            elevation: 0
            Layout.fillWidth: true
            id: lrec
//            border.color: "green"//"lightgrey"
//            border.width: ProtoScreen.guToPx(.125)
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
//            Layout.minimumWidth: tgrid.implicitWidth
//            Layout.preferredWidth: tgrid.implicitWidth
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

        //avg - pnl
        Material.Card {
            Layout.fillWidth: true
            id: posrec
//            border.color: themeroot.theme.accentColor
//            border.width: ProtoScreen.guToPx(.125)
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
//            Layout.preferredWidth: posgrid.implicitWidth
//            anchors.centerIn: parent
            width: ProtoScreen.guToPx(18.5)
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
                    font.family: fontfamFB
                }


            }
        }

        Material.Card {
            elevation: 0
            height: parent.height
            Layout.fillWidth: true
            id: fnameCARD
            width: ProtoScreen.guToPx(18.5)
//            Layout.preferredWidth: fbl12.width + ProtoScreen.guToPx(.5)
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
        Material.Card {
            Layout.fillWidth: true
            id: lrec2
//            border.width: 0
//            Layout.preferredWidth: recwidth * 2
            height: parent.height
//            Layout.minimumWidth: tgrid2.implicitWidth
//            Layout.preferredWidth: tgrid2.implicitWidth
//            Layout.alignment: Qt.AlignLeft
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
                    text: !mybalance ? "" : (mybalance.stake + (!tradingroot.totalopenpnl ? 0 : tradingroot.totalopenpnl)).toLocaleString() + " ƑɃ"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 2
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                    color: !inplay ? Material.Theme.light.textColor : inplay.bsdiff === 1 ? lightgreen :
//                                             inplay.bsdiff === -1 ? lightred :  Material.Theme.light.textColor

                    font.family: fontfamFB
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
                    font.family: fontfamFB
                }

                Material.Label {
                    text: !mybalance ? "" : (mybalance.stake).toLocaleString()  + " ƑɃ"
                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignLeft
                    Layout.column: 4
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    font.family: fontfamFB
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
                    font.family: fontfamFB
                }

            }
        }

     }
}
