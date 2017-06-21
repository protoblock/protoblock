import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0
import QtQuick.Layouts 1.1
import Material 1.0


Card {
    id: mdview
    anchors.fill: parent

    TableView {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        id: depthvm
        highlightOnFocus:   false
        height: parent.height
        width: ProtoScreen.guToPx(10) * 4
        selectionMode: SelectionMode.NoSelection

        rowDelegate: Rectangle {
           height: ProtoScreen.guToPx(4)
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

                Material.Label {
                    id: textItem2
                    text: styleData.value
                    anchors.fill: parent
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
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
        anchors.top: depthvm.top
        anchrosHType: "center"
    }

    Component {
        id: nzdel

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
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
