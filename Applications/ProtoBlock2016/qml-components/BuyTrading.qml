import QtQuick 2.4
import QtQuick.Layouts 1.1

import Material 1.0

import ProRotoQml.Protoblock 1.0

import ProRotoQml.Theme 1.0

//import ProRotoQml.Theme 1.0
Item {
    id: bscombo
//    property bool  weekly: false

//    property string color: "white"
//    property string backgroundColor: "black"
//    property int fontSize: ProtoScreen.font(ProtoScreen.SMALL)
//    property bool bold: false

    property int weeksleft: 15
    property bool isbuyCard: true
    property int price: (isbuyCard ? 1 : 26) * weeksleft
    property int qty: 1
    property string playerId
    property string side:  isbuyCard ? "Buy" : "Sell"
    property bool  changing: false


    property alias setprice: priceCombo.text
    property double avgpoints: 1.0

    Component.onCompleted: {
//        console.log(" bycard " + side + " price " + price + " combo " + priceCombo.currentIndex )
    }

    Card {
        id: card1
        width: parent.width
        height: parent.height
//        property alias tVal: tVal

        //Fixme do math
        ColumnLayout {
            anchors.fill: parent
            spacing: 2
//            bottom: parent.bottom
//            anchors.top: parent.top
//            anchors.right: parent.right
//            anchors.left: parent.left
//            anchors.rightMargin: ProtoScreen.guToPx(-6.125)
//            anchors.bottomMargin: ProtoScreen.guToPx(1.38)
//            anchors.leftMargin: ProtoScreen.guToPx(-6.125)
//            anchors.topMargin: ProtoScreen.guToPx(4.75)/*
//            scale: 1
//            rows: 0
//            columnSpacing: 0
//            rowSpacing: 0
//            columns: 1
//            clip: false
            Row {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                Label{
                    horizontalAlignment: Text.AlignJustify
                    verticalAlignment: Text.AlignJustify
                    text: side + " Season Contracts"
                    style: Text.Raised
                    font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                }
            }
            RowLayout{
                spacing: 1
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true

                Row {
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        id: avgPointsid
                        horizontalAlignment: Text.AlignRight
                        text: "Average Points:"
//                        font.pixelSize: ProtoScreen.font(ProtoScreen.TINY)
                        font.pixelSize: ProtoScreen.guToPx(.50)

                        Component.onCompleted: {
                            console.log(" fp " + font.pixelSize + " pp " + ProtoScreen.guToPx(.50))
                        }

                    }

                    ComboBox {
                        Layout.alignment: Qt.AlignVCenter
                        anchors.left: avgPointsid.right
                        editable: true
                        validator: IntValidator {bottom: 1; top: 40;}
    //                    focus: true
                        id: avgPoints
                        model: 40
                        currentIndex: 1

                        onActivated: {
                            priceCombo.text = index *  numweeks.currentIndex
                        }
                    }
                }

                Row {
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Label {
                        horizontalAlignment: Text.AlignRight
                        id: numweekstxt
                        text: "Weeks:"
                        font.pixelSize: ProtoScreen.font(ProtoScreen.TINY)
                    }

                    ComboBox {
                        anchors.left: numweekstxt.right
                        id: numweeks
                        editable: true
                        validator: IntValidator {bottom: 1; top: weeksleft;}
    //                    focus: true
                        currentIndex: weeksleft
                        onActivated: {
                            priceCombo.currentIndex = index *  avgPoints.currentIndex
                        }

                        model: 16
                    }
                }

            }

            RowLayout{
                spacing: 1
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true

                Row {
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Label{
                        id: priceCombotxt
                        text: side + " Price: "
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: ProtoScreen.font(ProtoScreen.TINY)

                    }
                    TextField {
                        anchors.left: priceCombotxt.right
                        id: priceCombo
                        validator: IntValidator {bottom: 1; top: 400;}
                        text: "1"
                        onAccepted: {
                            price = parseInt(text,10)
                            avgpoints = (1.0 * price) / (1.0 * (numweeks.currentIndex >0 ? numweeks.currentIndex : 15))
                            avgPoints.currentIndex = avgpoints < 1 ? 1 : avgpoints
                        }
                        font.pixelSize: ProtoScreen.font(ProtoScreen.TINY)
                    }
                }

                Row {
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Label{
                        style: "menu"
                        id: qytCombotxt
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: ProtoScreen.font(ProtoScreen.TINY)
                        text: "Qty to " + side + ":"
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    }
                    TextField {
                        anchors.left: qytCombotxt.right
                        id: qytCombo
                        validator: IntValidator {bottom: 1; top: 999;}
                        font.pixelSize: ProtoScreen.guToPx(.50)
                        //ProtoScreen.font(ProtoScreen.TINY)
                    }
                }
            }
            RowLayout{
                spacing: 1
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                Button{
                    anchors.fill: parent
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                    text:{
                        side + " NOW"
                    }

                    elevation: 3
                    backgroundColor: themeroot.theme.accentColor
                    onClicked:{ //ToDo PopUp
                        MiddleMan.doTrade(
                                    realRoot.playerInView
                                    ,isbuyCard
                                    ,priceCombo.currentIndex
                                    ,qytCombo.currentIndex
                                    )

                    }
                }
            }
            //col
        }
    }
}


//                MaterialComboBox{
//                    id: avgPoints
//                    width: Layout.width
//                    model: 40
//                    currentIndex: 0
//                    onCurrentIndexChanged: {
//                        if ( currentIndex > 0 ) {
//                            avgpoints = currentIndex
//                            console.log(side + " avgPoints " + currentIndex)
////                            var points = avgPoints.currentText
////                            var numberOfWeeks = comboBox2.currentText
////                            yearVal.text = (avgpoints * numberOfWeeks)
////                            weeklyAvg.text = avgPoints.currentText
////                            howManyWeeks.text = comboBox2.currentText

//                            if ( !changing ) {
//                                changing = true;
//                                priceCombo.currentIndex = currentIndex *  comboBox2.currentIndex
//                                changing = false;
//                            }
//                        }
//                    }
//                }
            //                    onCurrentIndexChanged: {
            //                        if ( currentIndex > 0 ) {
            //                            avgpoints = currentIndex
            //                            console.log(side + " avgPoints " + currentIndex)

            //                            if ( !changing ) {
            //                                changing = true;
            //                                priceCombo.currentIndex = currentIndex *  comboBox2.currentIndex
            //                                changing = false;
            //                            }
            //                        }
            //                    }
//                            }




                //                    onCurrentIndexChanged: {
//                        if ( currentIndex > 0 ) {
//                            console.log(side + " comboBox2 " + currentIndex)
//                            if ( !changing ) {
//                                changing = true
//                                priceCombo.currentIndex = currentIndex *  avgPoints.currentIndex
//                                changing = false;
//                            }
//                        }
//                    }
//                }



//            RowLayout{
//                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
//                Layout.fillHeight: true
//                Layout.fillWidth: true
//                Label{
//                    id:tVal
//                    text: "Yearly Value"
//                    Layout.fillWidth: true
//                }
//                Label{
//                    id: yearVal
//                    text:  "0"
//                    Layout.fillWidth: true
//                }
//            }
//            RowLayout{
//                Layout.fillHeight: true
//                Layout.fillWidth: true
//                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

//                Label{
//                    text:  "Average Weeks "
//                    Layout.fillWidth: true
//                }
//                Label{
//                    id: howManyWeeks
//                    text: "0"
//                    horizontalAlignment: Text.AlignHCenter
//                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
//                    Layout.fillWidth: true
//                }
//                Label{
//                    text: "Weekly Avg"
//                    Layout.fillWidth: true
//                }
//                Label{
//                    id: weeklyAvg
//                    text: "0"
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    Layout.fillWidth: true
//                }
//            }


