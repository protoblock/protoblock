import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4 as Controls

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

    property int weeksleft: 16
    property bool isbuyCard: true
    property int price: (isbuyCard ? 1 : 26) * weeksleft
    property int qty: 1
    property string playerId
    property string side:  isbuyCard ? "Buy" : "Sell"
    property bool  changing: false

    property int avgppg: Math.round(calcprice / numgames)
    property int numgames: 16
    property int calcprice
//    property int qty: 1
//    property alias setprice: priceCombo.text
    property double avgpoints: 1.0

//    Component.onCompleted: {
////        console.log(" bycard " + side + " price " + price + " combo " + priceCombo.currentIndex )
//    }
    width: parent.width
    height: parent.height
//    CardWithContent {
//        id: card1
//        width: parent.width
//        height: parent.height
////        property alias tVal: tVal
////        bannerColor: "white"
//        bannerText: "Trade Calculator"
////        bannerbottom: true
//        object:

//    }
//}

    RowLayout {
        anchors.fill: parent
        width: parent.width
        height: parent.height
        ColumnLayout {

            width: parent.width * .40
            height: parent.height
            Label {
                id: btban
        //                Layout.preferredWidth:
                text: "Trading Calculator"
                style: "subheading"
//                anchors.top: parent.top
                height: calcutt.height
                verticalAlignment: Text.AlignTop

            }
            RowLayout {
                id: rl1
                anchors.top: parent.verticalCenter
                height: parent.height - btban.height
                Layout.fillWidth: true
                width: parent.width
                Layout.fillHeight: false
                spacing: ProtoScreen.guToPx(3)
                ColumnLayout {
                    height: parent.height
                    width: parent.width / 2.0
                    Layout.fillHeight: true
                    Layout.fillWidth: true


                    Controls.ComboBox {
            //                    anchors.fill: parent
            //                    Layout.alignment: Qt.AlignVCenter
            //                    anchors.left: avgPointsid.right
            //                    editable: true
            //                    anchors.top: avgl.top
            //                    anchors.topMargin: avgl.height
                        inputMethodHints : Qt.ImhDigitsOnly
                        validator: IntValidator {bottom: 1; top: 40;}
                        editable: true
                        id: avgPoints
                        model: 40
                        currentIndex: avgppg
                        activeFocusOnPress: true
//                        focus: true
                        onActivated: {
                            avgppg = index
                            console.log("onActivated avgPoints " + index)
                            pricecombo.currentIndex = Math.round(index * numgames)
                        }


                        onAccepted: {
                            avgppg = currentIndex
                            console.log("accepted avgPoints " + currentIndex)
                            pricecombo.currentIndex = Math.round(currentIndex * numgames)
                        }

                        onFocusChanged: {
                            if ( !focus ) {
                                if ( editText != "" && currentIndex.toString() != editText ) {
                                    currentIndex = parseInt(editText)
                                    avgppg = currentIndex
                                    pricecombo.currentIndex = Math.round(currentIndex * numgames)
                                }
                            }
                           console.log("onFocusChanged avgPoints " + currentIndex)
                        }
                    }
                    Label {
                        id: avgl
            //                Layout.preferredWidth:
                        text: "Avg PPG"
                        style: "caption"
                    }
                }

                ColumnLayout {
                    height: parent.height
                    width: parent.width / 2.0
                    Layout.fillHeight: true
                    Layout.fillWidth: true


                    Controls.ComboBox {
            //                    Layout.alignment: Qt.AlignVCenter
            //                    anchors.left: avgPointsid.right
            //                    editable: true
            //                    anchors.fill: parent
            //                    id: numgames
                        inputMethodHints : Qt.ImhDigitsOnly
                        editable: true
                        id: numgamescb
                        validator: IntValidator {bottom: 1; top: 16;}
        //                displayText: currentIndex+1
                        model: 17
                        currentIndex: 16
                        activeFocusOnPress: true
//                        focus: true
                        onAccepted: {
                            numgames = currentIndex
                            pricecombo.currentIndex = Math.round(currentIndex * avgppg)
                        }

                        onActivated: {
                            numgames = index
                            console.log("onActivated numg " + index)
                            pricecombo.currentIndex = Math.round(index * numgames)
                        }

                        onFocusChanged: {
                            if ( !focus ) {
                                if ( editText != "" && currentIndex !== parseInt(editText) ) {
                                    currentIndex = parseInt(editText)
                                    numgames = currentIndex
                                    pricecombo.currentIndex = Math.round(currentIndex * numgames)
                                }
                            }
                           console.log("onFocusChanged numg " + currentIndex)
                        }
                    }
                    Label {
                        id: ngl
            //                Layout.preferredWidth:
                        text: "# Weeks"
                        style: "caption"

                    }
                }
            }
        }

        ColumnLayout {
//            anchors.fill: parent
            width: parent.width * .20
            height: parent.height

            Button {
                focus: true
                id: calcutt
        //                Layout.preferredWidth:
                text: "Calc"
                backgroundColor: "grey"
                onClicked: {
                    focus = true;
                    console.log(" numgame" + numgames + " avgppg " + avgppg + " price " + calcprice)
                }
            }

            ColumnLayout {
                height: parent.height
                width: parent.width
                Layout.fillHeight: true
                Layout.fillWidth: true
                anchors.top: parent.verticalCenter
                Controls.ComboBox {
        //                    anchors.fill: parent
        //                    Layout.alignment: Qt.AlignVCenter
        //                    anchors.left: avgPointsid.right
        //                    editable: true
        //                    anchors.top: avgl.top
        //                    anchors.topMargin: avgl.height
                    inputMethodHints : Qt.ImhDigitsOnly
                    validator: IntValidator {bottom: 1; top: 400;}
                    model: 400
                    editable: true
                    id: pricecombo
                    currentIndex: calcprice

                    activeFocusOnPress: true
                    focus: true
                    onAccepted: {
                        calcprice = currentIndex
                        avgPoints.currentIndex = Math.round(currentIndex / numgames)
                        console.log("pricecombo acc" + currentIndex)
                    }

                    onActivated: {
                        console.log("pricecombo act " + index)
                        calcprice = index
                        avgPoints.currentIndex = Math.round(index / numgames)
                    }

                    onFocusChanged: {
                        if ( !focus ) {
                            if ( editText != "" && currentIndex !==  parseInt(editText) ) {
                                currentIndex = parseInt(editText)
                                calcprice = currentIndex
                                avgPoints.currentIndex = Math.round(calcprice / numgames)
                            }
                        }
                       console.log("pricecombo onFocusChanged " + currentIndex + " edittext" + editText + " cp " + calcprice)
                    }

                    onCurrentIndexChanged: {
                        console.log("pricecombo currindex " + currentIndex)

                    }

                }
                Label {
                    id: prl
        //                Layout.preferredWidth:
                    text: "Price "
                    style: "caption"
                }
            }
        }
        ColumnLayout {

            width: parent.width * .40
            height: parent.height
            Label {
                id: totall
        //                Layout.preferredWidth:
                text: "Total"
                style: "Total"
                height: calcutt.height
            }
            RowLayout {
                spacing: ProtoScreen.guToPx(3)

                height: parent.height - totall.height
                width: parent.width
                Layout.fillHeight: true
//                anchors.verticalCenter: parent.verticalCenter
                anchors.top: parent.verticalCenter
                ColumnLayout {
                    height: parent.height
                    width: parent.width * .35
                    Layout.fillHeight: true
                    Layout.fillWidth: true


                    Controls.ComboBox {
            //                    anchors.fill: parent
            //                    Layout.alignment: Qt.AlignVCenter
            //                    anchors.left: avgPointsid.right
            //                    editable: true
            //                    anchors.top: avgl.top
            //                    anchors.topMargin: avgl.height
                        inputMethodHints : Qt.ImhDigitsOnly
                        validator: IntValidator {bottom: 1; top: 1000;}
                        editable: true
                        id: qtyCB
                        model: 1000
                        currentIndex: 1
                        activeFocusOnPress: true
//                        focus: true
                        onActivated: {
                            qty = index
                            console.log("onActivated qty " + index)
                            pricecombo.currentIndex = Math.round(index * numgames)
                        }


                        onAccepted: {
                            qty = currentIndex
                            console.log("accepted qtyCB " + currentIndex)
                        }

                        onFocusChanged: {
                            if ( !focus ) {
                                if ( editText != "" &&  currentIndex !== parseInt(editText) ) {
                                    currentIndex = parseInt(editText)
                                    qty = currentIndex
                                }
                            }
                           console.log("onFocusChanged qtyCB " + currentIndex)
                        }
                    }
                    Label {
                        id: qtyl
            //                Layout.preferredWidth:
                        text: "* Qty"
                        style: "caption"
                    }
                }

                ColumnLayout {
                    height: parent.height
                    width: parent.width * .65
                    Layout.fillHeight: true
                    Layout.fillWidth: true


//                    Rectangle {
//                       id: rectot
//                       border.color: "grey"
//                       color: "light grey"

//                       anchors.verticalCenter: qtyCB.verticalCenter;
                       Label {
                          height: qtyCB.height
                          width: qtyCB.width * 2.0
//                            anchors.fill: parent
                            id: totalval
                            text: Math.round(pricecombo.currentIndex * qtyCB.currentIndex) + " ƑɃ"
                            color: "darkred"
                       }
//                    }
                    Label {
                        id: totallb
            //                Layout.preferredWidth:
                        text: "= Total Value"
                        style: "caption"

                    }
                }
            }
        }

    }

}
