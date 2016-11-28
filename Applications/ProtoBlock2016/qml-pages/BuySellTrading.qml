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

Item {
    anchors.fill: parent
    id: pit
    property string contract
    property string symbol
    property variant inplay: MiddleMan.pPlayerQuoteSliceModelItem


    Layout.fillWidth: false
    property int depthsize: 5
    property double dihight: ProtoScreen.guToPx(4)
    property int price
    property bool isppgslider: false

//    RowLayout {
//        id: rl
//        anchors.fill: parent
//        spacing: 10
        //buy
        Material.Card {
            anchors.top: parent.top
            anchors.left: parent.left
            Layout.fillWidth: true
            id: bcard
            height: parent.height
    //            width: parent.width * .50
            anchors.margins: ProtoScreen.guToPx(.125)
    //        backgroundColor: (inplay.ldiff !== 0 ) ?  "lightgrey" : "white"
            Layout.preferredWidth: bcardgrid.implicitWidth
            GridLayout {
                id: bcardgrid
                anchors.centerIn: parent
                columns: 3
                rows: 3

                Material.Label {
                    text: "Buy Price:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                SpinBox {
                    id: buyspin
                    decimals: 0
                    stepSize: 1.0
                    maximumValue: 40
                    minimumValue:  1
                    value: 1
                    Layout.column: 2
                    Layout.row: 1
    //                anchors.centerIn: parent
                    onValueChanged: {

                    }
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                }

                Material.Label {
                    text: "expected fantasy points"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.Tiny)
                }

                Material.Label {
                    text: "Buy Qty:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)

                }

                SpinBox {
                    id: buyqty
                    decimals: 0
                    stepSize: 1.0
                    maximumValue: 100
                    minimumValue:  1
                    value: 1
                    Layout.column: 2
                    Layout.row: 2
    //                anchors.centerIn: parent
                    onValueChanged: {

                    }
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                }

                Material.Label {
                    text: "number of contracts"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.Tiny)
                }

                Button {
                    focus: true
                    id: buybut
                    text: "Buy"
                    backgroundColor: "green"
                    onClicked: {
                    }
                    Layout.column: 1
                    Layout.row: 3
                    Layout.columnSpan: 3
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                }
            }
        }

        //sell
        Material.Card {
            Layout.fillWidth: true
            anchors.left: bcard.right
            anchors.top: parent.top
            id: scard
            height: parent.height
    //            width: parent.width * .50
            anchors.margins: ProtoScreen.guToPx(.125)
    //        backgroundColor: (inplay.ldiff !== 0 ) ?  "lightgrey" : "white"
            Layout.preferredWidth: scardgrid.implicitWidth
            GridLayout {
                id: scardgrid
                anchors.centerIn: parent
                columns: 3
                rows: 3

                Material.Label {
                    text: "Sell Price:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                SpinBox {
                    id: sellspin
                    decimals: 0
                    stepSize: 1.0
                    maximumValue: 40
                    minimumValue:  1
                    value: 40
                    Layout.column: 2
                    Layout.row: 1
    //                anchors.centerIn: parent
                    onValueChanged: {

                    }
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true


                }

                Material.Label {
                    text: "expected fantasy points"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.Tiny)
                }

                Material.Label {
                    text: "Sell Qty:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)

                }

                SpinBox {
                    id: sellqty
                    decimals: 0
                    stepSize: 1.0
                    maximumValue: 100
                    minimumValue:  1
                    value: 1
                    Layout.column: 2
                    Layout.row: 2
    //                anchors.centerIn: parent
                    onValueChanged: {

                    }
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                }

                Material.Label {
                    text: "number of contracts"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.Tiny)
                }

                Button {
                    focus: true
                    id: sellbut
                    text: "Sell"
                    backgroundColor: "red"

                    Layout.column: 1
                    Layout.row: 3
                    Layout.columnSpan: 3
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true

                    onClicked : {
                        focus = true;
                        console.log(" price " + sellspin.value )
                        myTradeDialog.price = sellspin.value
                        myTradeDialog.qty = sellqty.value
                        myTradeDialog.side = "Sell"
                        myTradeDialog.player = inplay.fullname + " (" + inplay.position +") "
                                + "Symbol(" + inplay.symbol +")"
                        myTradeDialog.show()
                    }
                }
            }
        }

//    }

    /*
    RowLayout {
        id: rl
        anchors.fill: parent
        Layout.fillWidth: false
        Layout.fillHeight: true
        spacing: 10

        //buy
        Material.Card {
            Layout.fillWidth: true
            id: bcard
            height: parent.height
//            width: parent.width * .50
            anchors.margins: ProtoScreen.guToPx(.125)
    //        backgroundColor: (inplay.ldiff !== 0 ) ?  "lightgrey" : "white"
            Layout.preferredWidth: bcardgrid.implicitWidth
            GridLayout {
                id: bcardgrid
                anchors.centerIn: parent
                columns: 3
                rows: 3

                Material.Label {
                    text: "Buy Price:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                SpinBox {
                    id: buyspin
                    decimals: 0
                    stepSize: 1.0
                    maximumValue: 40
                    minimumValue:  1
                    value: 1
                    Layout.column: 2
                    Layout.row: 1
                    anchors.centerIn: parent
                    onValueChanged: {

                    }
                }

                Material.Label {
                    text: "expected fantasy points"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.Tiny)
                }

                Material.Label {
                    text: "Buy Qty:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                SpinBox {
                    id: buyqty
                    decimals: 0
                    stepSize: 1.0
                    maximumValue: 100
                    minimumValue:  1
                    value: 1
                    Layout.column: 2
                    Layout.row: 2
                    anchors.centerIn: parent
                    onValueChanged: {

                    }
                }

                Material.Label {
                    text: "number of contracts"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.Tiny)
                }

                Button {
                    focus: true
                    id: buybut
                    text: "Buy"
                    backgroundColor: "green"
                    onClicked: {
                    }
                    Layout.column: 1
                    Layout.row: 3
                    Layout.rowSpan: 3
                    anchors.fill: parent
                }
            }
        }

        Material.Card {
            Layout.fillWidth: true
            id: scard
            height: parent.height
//            width: parent.width * .50
            anchors.margins: ProtoScreen.guToPx(.125)
    //        backgroundColor: (inplay.ldiff !== 0 ) ?  "lightgrey" : "white"
            GridLayout {
                id: scardgrid
                anchors.centerIn: parent
                columns: 3
                rows: 3

                Material.Label {
                    text: "Sell Price:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                SpinBox {
                    id: sellspin
                    decimals: 0
                    stepSize: 1.0
                    maximumValue: 40
                    minimumValue:  1
                    value: 1
                    Layout.column: 2
                    Layout.row: 1
                    anchors.centerIn: parent
                    onValueChanged: {

                    }
                }

                Material.Label {
                    text: "expected fantasy points"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.Tiny)
                }

                Material.Label {
                    text: "Buy Qty:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                SpinBox {
                    id: sellqty
                    decimals: 0
                    stepSize: 1.0
                    maximumValue: 100
                    minimumValue:  1
                    value: 1
                    Layout.column: 2
                    Layout.row: 2
                    anchors.centerIn: parent
                    onValueChanged: {

                    }
                }

                Material.Label {
                    text: "number of contracts"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.Tiny)
                }

                Button {
                    focus: true
                    id: sellbut
                    text: "Sell"
                    backgroundColor: "red"
                    onClicked: {
                    }
                    Layout.column: 1
                    Layout.row: 3
                    Layout.rowSpan: 3
                    anchors.fill: parent
                }
            }
        }
    }
*/

    //    Card {
//        id: topcard
//        width: parent.width
//        height: parent.height
//        elevation: 0
//        anchors.centerIn: parent
//        anchors{
//            top: parent.top
//            topMargin:ProtoScreen.guToPx(.5)
//            horizontalCenter: parent.horizontalCenter
//            margins: ProtoScreen.guToPx(.125)
//        }

//        // spacer
//        Rectangle{width: ProtoScreen.guToPx(.125); height: ProtoScreen.guToPx(1);color: "transparent"}

//        Item {
//            id: row3
//            anchors.fill: parent
////            width: boundingRect.width * .85
////            height: parent.height - boundingRect.height - bandepth.height - boundquote.height - cwc.height
////            anchors.top: parent.bottom
////            anchors.topMargin: ProtoScreen.guToPx(1)
////            anchors.horizontalCenter: boundingRect.horizontalCenter
////                anchors.rightMargin: width * .10

//            Item {
//                height: ProtoScreen.guToPx(14)
//                id: bcardrow;
//                width: parent.width
////                    height: parent.height
//                anchors.top: parent.top
//                anchors.horizontalCenter: parent.horizontalCenter

//                BuyTrading {
//                    symbol: symbol
//                    id: buycard
////                        width: parent.width
////                        height: parent.height
//                    calcprice: inplay.lastprice
////                        onCalculate:

//                    Component.onCompleted: {
//                        buycard.calculate.connect(doCalc)
//                    }
//                }
//            }


//            Item {
//                anchors.top: bcardrow.bottom
//                id: row1
//                width: (parent.width / 3.0) * 2.0
//                height: ProtoScreen.guToPx(14)//parent.height / 2
////                        anchors.top: bcardrow.right
//                anchors.margins: ProtoScreen.guToPx(.5)
//                anchors.horizontalCenter: parent.horizontalCenter


//                Rectangle {
//                    color: "transparent"
//                    width: parent.width / 2.0
////                            height: parent.height - ProtoScreen.guToPx(2)
////                            radius: ProtoScreen.guToPx(1)
////                            anchors.right: parent.right
//                    height: parent.height - ProtoScreen.guToPx(2)
//                    radius: ProtoScreen.guToPx(1)
//                    anchors.right: parent.horizontalCenter


//                    Button {
////                            anchors.fill: parent
//                        width: parent.width / 2.0 // ProtoScreen.guToPx(4)
//                        height: width / 2.0
////                            anchors.rightMargin: ProtoScreen.guToPx(1)
////                            anchors.right: parent.right
////                            anchors.verticalCenter: parent.verticalCenter
//                        anchors.centerIn: parent
////                            anchors.margins: ProtoScreen.guToPx(.25)
//                        text: "BUY"
//                        backgroundColor:  Colors.green
//                        textColor: "white"
//                        onClicked : {
//                            focus = true;
//                            console.log(" price " + buycard.calcprice )
//                            myTradeDialog.price = buycard.calcprice
//                            myTradeDialog.qty = buycard.qty
//                            myTradeDialog.side = "Buy"
//                            myTradeDialog.player = inplay.fullname + " (" + inplay.position +") "
//                                    + "Symbol(" + inplay.symbol +")"
//                            myTradeDialog.show()
////                                    MiddleMan.doTrade(
////                                            inplay.symbol
////                                            ,true
////                                         ,buycard.calcprice
////                                         ,buycard.qty
////                                            )


//                        }
//                    }
//                }

////                        Text {
////                            id: ttt
////                            text: "a"
////                            Component.onCompleted: {
////                                console.log(" implic width " + ttt.implicitWidth)
////                                console.log(" implic height " + ttt.implicitHeight)

////                            }
////                        }

//                Rectangle {
//                    color: "transparent"
//                    width: parent.width / 2
//                    height: parent.height - ProtoScreen.guToPx(2)
//                    radius: ProtoScreen.guToPx(1)
//                    anchors.left: parent.horizontalCenter
//                    Button {
//                        width: parent.width / 2 // ProtoScreen.guToPx(4)
//                        height: width / 2.0
////                            anchors.leftMargin: ProtoScreen.guToPx(1)
////                            anchors.left: parent.left
////                            anchors.verticalCenter: parent.verticalCenter
//                        anchors.centerIn: parent

////                            anchors.margins: ProtoScreen.guToPx(.25)
//                        text: "SELL"
//                        backgroundColor:  Colors.red
//                        textColor: "white"
//                        onClicked : {
//                            focus = true;
//                            myTradeDialog.price = buycard.calcprice
//                            myTradeDialog.qty = buycard.qty
//                            myTradeDialog.side = "Sell"
//                            myTradeDialog.player = inplay.fullname + " (" + inplay.position +") "
//                                    + "Symbol(" + inplay.symbol +")"
//                            myTradeDialog.show()
//                        }
//                     }

//                 }

//            }

////                    BuyTrading {
////                        id: sellCard
////                        width: parent.width / 2
////                        height: parent.height
////                        isbuyCard: false

////                    }
//        }

//    }
    Dialog {
        height: ProtoScreen.guToPx(34)
        minimumHeight: ProtoScreen.guToPx(8)
        width: ProtoScreen.guToPx(77)
        minimumWidth: ProtoScreen.guToPx(16)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        property string side: "none"
        property int price
        property int qty
        property string player
        id: myTradeDialog
        positiveButtonText: side + " Now"
        title: "Confirm Trade - 2016 Season Rest-of-The-way"
        text: "Protoblock Player: " + realRoot.uname
        dialogContent: Column {
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
//            anchors.centerIn: parent
            spacing: ProtoScreen.guToPx(.5)


            Text{
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                text:  myTradeDialog.side + " " + myTradeDialog.qty.toString() +
                       " contract(s) at price " + myTradeDialog.price.toString()
                font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            }

            Text{
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                text:  "of " + myTradeDialog.player
                font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            }

        }

        onAccepted: {
            if ( realRoot.uname === "" ) {
                rootLoader.source = "qrc:/Account.qml"
                pageHelper.selectedTabIndex = 3;
            }
            else MiddleMan.doTrade(
                    inplay.symbol
                    ,(side == "Buy") ? true : false
                 ,myTradeDialog.price
                 ,myTradeDialog.qty
                    )
        }

    }
}
//            Card {
//                id: buySell
//                width: boundingRect.width
//                anchors.top: row3.bottom
////                Layout.fillHeight: true
////                Layout.fillWidth: true
//                height: ProtoScreen.guToPx(12)
//                anchors.topMargin: ProtoScreen.guToPx(.25)
//                anchors {
//                    horizontalCenter: boundingRect.horizontalCenter
////                    margins: ProtoScreen.guToPx(.25)
//                }

//                elevation: 0
//                Row {
////                    spacing: 2
//                    id: row1
//                    width: (parent.width / 3) * 2
//                    height: ProtoScreen.guToPx(4)
//                    anchors.centerIn: parent
//                    anchors.fill: parent
//                    anchors.margins: ProtoScreen.guToPx(.5)
////                    anchors.horizontalCenter: parent.horizontalCenter

//                    Rectangle {
//                        color: "transparent"
//                        width: parent.width / 2
//                        height: parent.height - ProtoScreen.guToPx(2)
//                        radius: ProtoScreen.guToPx(1)
//                        anchors.verticalCenter: parent.verticalCenter

//                        Button {
////                            anchors.fill: parent
//                            width: parent.width / 2 // ProtoScreen.guToPx(4)
//                            height: width / 2.7
////                            anchors.rightMargin: ProtoScreen.guToPx(1)
////                            anchors.right: parent.right
////                            anchors.verticalCenter: parent.verticalCenter
//                            anchors.centerIn: parent
////                            anchors.margins: ProtoScreen.guToPx(.25)
//                            text: "BUY"
//                            backgroundColor:  Colors.green
//                            textColor: "white"
//                            onClicked : {
//                                console.log(" price " + pint.txtN )
//                                 MiddleMan.doTrade(
//                                        inplay.symbol
//                                        ,true
//                                        ,pint.txtN
//                                        ,qint.txtN
//                                        )
//                            }
//                        }
//                    }

//                    Rectangle {
//                        color: "transparent"
//                        width: parent.width / 2
//                        height: parent.height - ProtoScreen.guToPx(2)
//                        radius: ProtoScreen.guToPx(1)
//                        anchors.verticalCenter: parent.verticalCenter
//                        Button {
//                            width: parent.width / 2 // ProtoScreen.guToPx(4)
//                            height: width / 2.7
////                            anchors.leftMargin: ProtoScreen.guToPx(1)
////                            anchors.left: parent.left
////                            anchors.verticalCenter: parent.verticalCenter
//                            anchors.centerIn: parent

////                            anchors.margins: ProtoScreen.guToPx(.25)
//                            text: "SELL"
//                            backgroundColor:  Colors.red
//                            textColor: "white"
//                            onClicked : {
//                                console.log(" price " + pint.txtN )
//                                 MiddleMan.doTrade(
//                                        inplay.symbol
////                                        pid.txtN
//                                        ,false
//                                        ,pint.txtN
//                                        ,qint.txtN
//                                        )

//                            }
//                         }

//                     }

//                }
//            }


//            Card {
//                id: inputs
//                width: buySell.width
//                anchors.top: buySell.bottom
////                Layout.fillHeight: true
////                Layout.fillWidth: true
//                height: ProtoScreen.guToPx(8)
//                anchors.topMargin: ProtoScreen.guToPx(.25)
//                anchors {
//                    horizontalCenter: buySell.horizontalCenter
////                    margins: ProtoScreen.guToPx(.25)
//                }
//                elevation: 0

////                Row {
////                    width: (pint.width + qint.width) * 1.20
////                    height: ProtoScreen.guToPx(4)
////                    anchors.centerIn: parent
////                    anchors.fill: parent
////                    anchors.margins: ProtoScreen.guToPx(.5)
////                    anchors.horizontalCenter: parent.horizontalCenter

////                    id: row2

//                    IntHelper {
//                        id: pint
////                            width: parent.width / 2
//                        labelTxt: "Price"
//                        lo: 1
//                        hi: 400
//                        anchors.verticalCenter: parent.verticalCenter
//                        anchors.right: parent.horizontalCenter
//                        anchors.rightMargin:ProtoScreen.guToPx(1)
////                        helpShown: true

//                    }
//                    IntHelper {
//                        id: qint
////                            width: parent.width / 2
//                        labelTxt: "Qty"
//                        lo: 1
//                        hi: 100
//                        anchors.verticalCenter: parent.verticalCenter
//                        anchors.left: pint.right
//                        anchors.leftMargin: ProtoScreen.guToPx(2)
////                        helpShown: true
////                        txtN: gamesslider.value * ppgslider.value
//                    }

////                    IntHelper {
////                        id: pid
////                        labelTxt: "oid"
////                        lo: 1
////                        hi: 10000
////                        anchors.verticalCenter: parent.verticalCenter
////                        anchors.right: pint.left
////                        anchors.rightMargin: ProtoScreen.guToPx(2)
//////                        txtN: inplay.playerid
////                        onChanged: {
////                            MiddleMan.doCancel(txtN);
////                        }
////                    }


//            }

//                Row {
//                    id: row2
//                    anchors.top: row1.bottom
//                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
//                    Layout.fillHeight: true
//                    Layout.fillWidth: true

//                    Label{
//                        style: "menu"
//                        id: qytCombotxt
//                        horizontalAlignment: Text.AlignRight
//                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
//                        text: "Qty: "
//                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
//                    }
//                    TextField {
//                        anchors.left: qytCombotxt.right
//                        id: qytCombo
//                        validator: IntValidator {bottom: 1; top: 999;}
//                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
//                    }
//                }

//                Row {
//                    anchors.top: row2.bottom
//                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
//                    Layout.fillHeight: true
//                    Layout.fillWidth: true

//                    Label{
//                        id: priceCombotxt
//                        text: " Price: "
//                        horizontalAlignment: Text.AlignRight
//                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)

//                    }
//                    TextField {
//                        anchors.left: priceCombotxt.right
//                        id: priceCombo
//                        validator: IntValidator {bottom: 1; top: 400;}
//                        text: "1"
////                        onAccepted: {
////                            price = parseInt(text,10)
////                            avgpoints = (1.0 * price) / (1.0 * (numweeks.currentIndex >0 ? numweeks.currentIndex : 15))
////                            avgPoints.currentIndex = avgpoints < 1 ? 1 : avgpoints
////                        }
//                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
//                    }
//                }

//            }




//    Connections {
//        target: MiddleMan
//        onPPlayerQuoteSliceModelItemChanged: {
//            console.log(" onPPlayerQuoteSliceModelItemChanged ");
//            depthvm.model = MiddleMan.pPlayerQuoteSliceModelItem.pDepthMarketModel
//            console.log("after floor  onPPlayerQuoteSliceModelItemChanged ");
//        }
//    }

