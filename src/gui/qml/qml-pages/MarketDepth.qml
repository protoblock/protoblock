import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0
import QtQuick.Layouts 1.1

import ProRotoQml.Theme 1.0


Item {
    id: mdview
    anchors.fill: parent
    Item {
        width: parent.width
        height: parent.height

        TableView {
            id: depthvm
            highlightOnFocus:   false
            height: parent.height - ProtoScreen.guToPx(5)
            implicitWidth: parent.width

            selectionMode: SelectionMode.NoSelection

            rowDelegate: Rectangle {
               height: ProtoScreen.guToPx(5)
               color: styleData.alternate?"#f5f5f5":"transparent"
            }

            headerDelegate:  Rectangle {
                width: parent.width
                height: ProtoScreen.guToPx(8)
                implicitWidth: textItem2.implicitWidth
                color: "white"
                Material.Card {
                    width: parent.width
                    height: parent.height * .50
                    backgroundColor: themeroot.theme.primaryColor
                    anchors.bottom: parent.bottom
                    radius: 1
//                    border: false
                    Material.Label {
                        id: textItem2
                        text: " " + styleData.value + " "
                        anchors.fill: parent
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        elide: Text.ElideRight
                        wrapMode: Text.WordWrap
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
                width: ProtoScreen.guToPx(15)
            }

            TableViewColumn{
                role: "bid"
                title: "Bid Price"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(15)
            }

            TableViewColumn{
                role: "ask"
                title: "Ask Price"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(15)
            }

            TableViewColumn {
                role: "asksize"
                title: "Ask Size"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(15)
            }
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
