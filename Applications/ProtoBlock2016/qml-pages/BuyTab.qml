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

    Material.Card {
    //            anchors.top: parent.top
    //            anchors.left: parent.left
    //            Layout.alignment: Qt.AlignCenter
        Layout.fillWidth: true
        anchors.centerIn: parent
    //            Layout.fillHeight: true

//        id: bcard
    //            height: parent.height
        height: ProtoScreen.guToPx(14)


    //            width: parent.width * .50
        anchors.margins: ProtoScreen.guToPx(.125)
    //        backgroundColor: (inplay.ldiff !== 0 ) ?  "lightgrey" : "white"
        Layout.preferredWidth: bcardgrid.width + ProtoScreen.guToPx(3)
        GridLayout {
            id: bcardgrid
            anchors.centerIn: parent
            columns: 3
            rows: 3
    //                anchors.margins: ProtoScreen.guToPx(3)

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

            Material.Button {
    //                    focus: true
                enabled: MiddleMan.liveSync === "Live"
                elevation: 4
                id: buybut
                text: "Buy"
                backgroundColor: "green"
                Layout.column: 1
                Layout.row: 3
                Layout.columnSpan: 3
                Layout.alignment: Qt.AlignCenter
                Layout.fillWidth: true
                onClicked : {
    //                        focus = true;
                    console.log(" price " + buyspin.value )
                    myTradeDialog.price = buyspin.value
                    myTradeDialog.qty = buyqty.value
                    myTradeDialog.side = "Buy"
                    myTradeDialog.player = inplay.fullname + " (" + inplay.pos +") "
                            + "Symbol(" + inplay.symbol +")"
                    myTradeDialog.show()
                }

            }
        }
    }

}
