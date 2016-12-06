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
    id: mdview
    anchors.fill: parent

    property string lightgreen: "green"//"#c8ffc8"
    property string lightred: "red"//"#ffc8c8"
     property variant inplay: MiddleMan.pPlayerQuoteSliceModelItem
//    Rectangle {
//        id: middlebar
//        anchors.horizontalCenter: parent.horizontalCenter
//        height: parent.height
//        color: "grey"
//        width: ProtoScreen.guToPx(2)
//        border.color: "black"
//        Layout.fillHeight: true
//        Layout.fillWidth: true
//    }
/*
    Item {
        Layout.fillWidth: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        id: topi
        width: depthvm.width
        height: cwc.height + pname.height + lrec.height

        Banner {
            id: cwc
            fontSize: ProtoScreen.font(ProtoScreen.SMALL)
            bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            text: " 2016 Week " + MiddleMan.theWeek + " Contract"
            color: "white"
            backgroundColor: themeroot.theme.primaryColor

            width: parent.width
            height: ProtoScreen.guToPx(3)
//            anchors.bottomMargin:ProtoScreen.guToPx(1)
//            anchors.topMargin:ProtoScreen.guToPx(.5)
            anchors.top: parent.top
            anchrosHType: "center"
        }

        Material.Card {
            id: pname
            anchors.top: cwc.bottom
            backgroundColor:  "white"
            width: parent.width
            height: teamicon.height

            Icon {
                id: teamicon
                hasColor:false
                source: "qrc:/"+ inplay.teamid+".PNG"
                width: ProtoScreen.guToPx(6)
                height: width
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    margins: ProtoScreen.guToPx(.25)
                }
            }
            Material.Label {
                id: pname2
                text: inplay.fullname + " (" + inplay.pos +") "
//                anchors.fill: parent
//                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                anchors {
                    verticalCenter: parent.verticalCenter
                    left: teamicon.right
                    margins: ProtoScreen.guToPx(.25)
                }
            }

            Material.Label {
                id: symb1
                text: " - " + inplay.playerid
//                anchors.fill: parent
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                anchors {
                    verticalCenter: parent.verticalCenter
                    left: pname2.right
                    margins: ProtoScreen.guToPx(.25)
                }
            }


        }

        Rectangle {
            id: lrec
            border.color: "lightgrey"
            border.width: ProtoScreen.guToPx(.125)
            width: parent.width *.50
            anchors.top: pname.bottom
            anchors.right: parent.horizontalCenter
            height: ProtoScreen.guToPx(15)
            color: (inplay.hdiff !== 0 || inplay.ldiff !== 0 || inplay.bsdiff !== 0 || inplay.asdiff !== 0) ?  "lightgrey" : "transparent"
            GridLayout {
                id: tgrid
    //            width: parent.width

                anchors.centerIn: parent
                columns: 4
                rows: 3
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
                    text: "Volume:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 3
                    Layout.columnSpan: 2
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
                    text: inplay.volume
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 3
                    Layout.columnSpan: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: "Hi:"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 3
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                Material.Label {
                    text: "Lo:"
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
        Rectangle {
            border.color: "lightgrey"
            border.width: ProtoScreen.guToPx(.125)
            width: parent.width *.50
            anchors.top: pname.bottom
            anchors.left: parent.horizontalCenter
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
     }
*/
        //        Item {
//            anchors.top: pname.bottom
//            id: qban
//            height: ProtoScreen.guToPx(10)
//            width: parent.width
//            Material.Card {
//                Layout.fillWidth: true
//                Layout.preferredWidth: parent.width * .50

//            }
//            Material.Card {
//                Layout.fillWidth: true
//                Layout.preferredWidth: parent.width * .50
//                Row {
//                    spacing: 1
//                    anchors.horizontalCenter: parent.horizontalCenter
//                    Column {
//                        anchors.verticalCenter: parent.verticalCenter
//                        Material.Label {
//                            width: parent.width * .50
//                            height: parent.height * .50
//                            text: "Last:" + inplay.lastprice
//                            verticalAlignment: Text.AlignVCenter
//                            horizontalAlignment: Text.AlignRight
//                        }

//                        Material.Label {
//                            width: parent.width * .50
//                            height: parent.height * .50
//                            text: "Change:"
//                            verticalAlignment: Text.AlignVCenter
//                            horizontalAlignment: Text.AlignRight
//                        }
//                    }

//                    Column {
//                        anchors.verticalCenter: parent.verticalCenter
//                        Row {
//                            Label{
//                                id: lastval
//                                text: inplay.lastprice === 0 ?  "" : inplay.lastprice
//                                Layout.fillHeight: true
//                                Layout.fillWidth:  false
//                                verticalAlignment: Text.AlignVCenter
//                                horizontalAlignment: Text.AlignLeft
//                                color: icon.color
//                            }
//                        }

//                        Row {

//                            Label{
//                                id: change
//                                text: ((inplay.change > 0) ? "+" : "" ) + inplay.change.toString() + " "
//                                Layout.fillHeight: true
//                                Layout.fillWidth:  false
//                                verticalAlignment: Text.AlignVCenter
//                                horizontalAlignment: Text.AlignLeft
//                                color: icon.color
//                            }

//                            Icon{
//                            id: icon
//    //                            anchors.left: change.right
//                            Layout.fillWidth:  false
//                            Layout.fillHeight: true
//                            hasColor:true
//                            color: { inplay.change < 0 ? Colors.red :
//                                     inplay.change > 0 ? Colors.green : Colors.black
//                            }
//                            source: {
//                                if (inplay.change < 0 ){
//                                    "qrc:/icons/ic_trending_down.png"

//                                    ////ic_trending_down.png"

//                                }
//                                else
//                                {
//                                    if (inplay.change === 0 )
//                                    {
//                                        "qrc:/icons/ic_trending_flat.png"
//                                    }
//                                    else
//                                    {
//                                        "qrc:/icons/ic_trending_up.png"
//                                    }
//                                }
//                            }
//                        }
//                        }
//                    }

//                }
//            }
//        }

//        ListItems.Subtitled {
//            id: pname
//            anchors.top: cwc.bottom
////        anchors.centerIn: parent
////        Layout.fillWidth: false
////        clip: false
////        elevation:  2
////                anchors.horizontalCenter: cwc.horizontalCenter
////                anchors.horizontalCenter: topcard.horizontalCenter
//            backgroundColor:  "white"
//            width: parent.width
//            text: {
//                text: inplay.fullname + " (" + inplay.pos +")"
//            }

//            action: Icon{
//                id: teamicon
//                hasColor:false
//                source: "qrc:/"+ inplay.teamid+".PNG"
//                width: ProtoScreen.guToPx(6)
//                height: width
//            }
//        }
//    }




    TableView {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        id: depthvm
        highlightOnFocus:   false
        height: parent.height
//            implicitWidth: parent.width

        width: ProtoScreen.guToPx(10) * 4
        selectionMode: SelectionMode.NoSelection

        rowDelegate: Rectangle {
           height: ProtoScreen.guToPx(5)
           color: styleData.alternate?"#f5f5f5":"transparent"
        }

        headerDelegate:  Rectangle {
            width: parent.width
            height: ProtoScreen.guToPx(8)
            implicitWidth: textItem2.implicitWidth
            color: "transparent"
            Material.Card {
                width: parent.width
                height: parent.height * .50
                backgroundColor: themeroot.theme.primaryColor
                anchors.bottom: parent.bottom
                radius: 1
//                    border: false
                Material.Label {
                    id: textItem2
                    text: styleData.value
                    anchors.fill: parent
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                    elide: Text.ElideRight
//                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }
            }
        }

        frameVisible: false

        TableViewColumn {
            role: "bidsize"
            title: "Bid Size"
            horizontalAlignment : Text.AlignHCenter
            width: ProtoScreen.guToPx(10)
            delegate: nzdel
            resizable: true
            movable: false
        }

        TableViewColumn{
            role: "bid"
            title: "Bid Price"
            horizontalAlignment : Text.AlignHCenter
            width: ProtoScreen.guToPx(10)
            delegate: nzdel
            resizable: true
            movable: false
        }

//        TableViewColumn{
//            width: ProtoScreen.guToPx(2)
//            resizable: false
//            movable: false
//            delegate: Rectangle {
////                id: middlebar
////                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.fill: parent
//                color: "grey"
//                width: ProtoScreen.guToPx(2)
////                border.: "black"
//                Layout.fillHeight: true
//                Layout.fillWidth: true
//            }
//        }

        TableViewColumn{
            role: "ask"
            title: "Ask Price"
            horizontalAlignment : Text.AlignHCenter
            width: ProtoScreen.guToPx(10)
            delegate: nzdel
            resizable: true
            movable: false
        }

        TableViewColumn {
            role: "asksize"
            title: "Ask Size"
            horizontalAlignment : Text.AlignHCenter
            width: ProtoScreen.guToPx(10)
            delegate: nzdel
            resizable: true
            movable: false
        }

    }

    Banner {
        id: dhead
        Layout.fillWidth: false
        fontSize: ProtoScreen.font(ProtoScreen.SMALL)
        bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        text: " Market Depth"
        color: themeroot.theme.primaryColor
        backgroundColor: "white"

        width: depthvm.width
        height: ProtoScreen.guToPx(4)
//            anchors.bottomMargin:ProtoScreen.guToPx(1)
//            anchors.topMargin:ProtoScreen.guToPx(.5)
        anchors.top: depthvm.top
        anchrosHType: "center"
    }

    Component {
        id: nzdel

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
//            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: (styleData.value < 1) ? "" : styleData.value;
            font.bold: false;
        }
    }

    Connections {
        target: MiddleMan
        onPPlayerQuoteSliceModelItemChanged: {
            console.log(" onPPlayerQuoteSliceModelItemChanged ");
            depthvm.model = MiddleMan.pPlayerQuoteSliceModelItem.pDepthMarketModel
            console.log("after floor  onPPlayerQuoteSliceModelItemChanged ");
        }
    }
}
