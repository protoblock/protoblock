import QtQuick 2.4
import QtQuick.Layouts 1.1

import QtQuick.Controls 1.4
import Material 1.0
import ProRotoQml.Protoblock 1.0
import QtQuick.Controls.Styles 1.3 as ControlStyles


//import ProRotoQml.Theme 1.0
Item {
//    id: name
//    property bool  weekly: false

//    property string color: "white"
//    property string backgroundColor: "black"
//    property int fontSize: ProtoScreen.font(ProtoScreen.SMALL)
//    property bool bold: false

    property int numweeks: 15
    property bool isbuyCard: true
    property int price: (isbuyCard ? 1 : 26) * numweeks
    property int qty: 1
    property string playerId
    property string side:  isbuyCard ? "Buy" : "Sell"
    property bool  changing: false


    property alias setprice: priceCombo.currentIndex
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
        GridLayout{
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: -49
            anchors.bottomMargin: 11
            anchors.leftMargin: -49
            anchors.topMargin: -38
            scale: 1
            rows: 0
            columnSpacing: 0
            rowSpacing: 0
            columns: 1
            clip: false
            RowLayout{
                spacing: 6
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                Label{
                    anchors.centerIn: parent
                    text: side
                    style: Text.Raised
                    font.pointSize: 10
                    Layout.fillWidth: true
                }
            }

            RowLayout{
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                Label {
                    text: qsTr("Average Points")
                    Layout.fillWidth: true
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

                ComboBox {
////                    style: ControlStyles.ComboBoxStyle{
////                        font{
////                            family: "Roboto"
////                            bold: bold
////                            pixelSize: fontSize
////                        }

//                    }
                    editable: true
                    id: avgPoints
                    Layout.fillWidth: true
                    model: 40
                    currentIndex: 0
                    onCurrentIndexChanged: {
                        if ( currentIndex > 0 ) {
                            avgpoints = currentIndex
                            console.log(side + " avgPoints " + currentIndex)
//                            var points = avgPoints.currentText
//                            var numberOfWeeks = comboBox2.currentText
//                            yearVal.text = (avgpoints * numberOfWeeks)
//                            weeklyAvg.text = avgPoints.currentText
//                            howManyWeeks.text = comboBox2.currentText

                            if ( !changing ) {
                                changing = true;
                                priceCombo.currentIndex = currentIndex *  comboBox2.currentIndex
                                changing = false;
                            }
                        }
                    }
                }
                Label {
                    id: label2
                    text: qsTr("Weeks")
                    Layout.fillWidth: true
                }
                ComboBox {
                    id: comboBox2
                    Layout.fillWidth: true
                    model: 16
                    currentIndex: numweeks
                    onCurrentIndexChanged: {
                        if ( currentIndex > 0 ) {
                            console.log(side + " comboBox2 " + currentIndex)
//                            var points = avgpoints
//                            var numberOfWeeks = comboBox2.currentText
//                            yearVal.text = (avgpoints * numberOfWeeks)

//                            weeklyAvg.text = avgpoints
//                            howManyWeeks.text = comboBox2.currentText

                            if ( !changing ) {
                                changing = true
                                priceCombo.currentIndex = currentIndex *  avgPoints.currentIndex
                                changing = false;
                            }
                        }
                    }
                }
            }



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


            RowLayout{
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Label{
                    text: side + " Price"
                    horizontalAlignment: Text.AlignRight
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: false

                }
                ComboBox{
                    id: priceCombo
                    Layout.fillWidth: true
                    model: 400
                    currentIndex: price
                    onCurrentIndexChanged: {
                        console.log(side + " priceCombo " + currentIndex)

                        if ( !changing ) {
                            changing = true
                            avgpoints = (1.0 * currentIndex) / (1.0 * (comboBox2.currentIndex >0 ? comboBox2.currentIndex : 15))
                            avgPoints.currentIndex = avgpoints < 1 ? 1 : avgpoints
                            console.log(" agvpoints " + avgpoints + " int " + avgPoints.currentIndex)
                            changing = false
                        }
                    }
                }
                Label{
                    text: "Qty to " + side
                    horizontalAlignment: Text.AlignRight
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    transformOrigin: Item.Center
                    Layout.fillWidth: true
                }
                ComboBox{
                    id: qytCombo
                    Layout.fillWidth: true
                    model: 999
                    currentIndex: qty
                }
            }

            RowLayout{
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: false
                Layout.fillWidth: true
                Button{
                    text:{
                        side + " NOW"
                    }
                    Layout.fillWidth: true;
                    Layout.fillHeight: false
                    elevation: 3
                    backgroundColor: themeroot.theme.accentColor
                    onClicked:{
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
