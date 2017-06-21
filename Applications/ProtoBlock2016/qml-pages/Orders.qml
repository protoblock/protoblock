import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1
import Material 1.0

Card {
    id: orview
    anchors.fill: parent

    ListView {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height
        anchors.margins: 0
        width: parent.width
        model: MiddleMan.pGlobalOpenOrdersModel
        headerPositioning: ListView.OverlayHeader
        header: RowLayout {
            z: 2
            width: parent.width
            height: ProtoScreen.guToPx(5)

            Repeater{
                model: ["Can","Symb","Ref","Side","Price","Size"]
                Card{
                    flat: true
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: (parent.width / 6.0);
                    border.color:"black"
                    backgroundColor: Colors.blue
                    border.width:  ProtoScreen.guToPx(.2)
                    radius: 0
                    Label{
                        anchors.centerIn: parent
                        text: modelData
                        font.pixelSize: ProtoScreen.font(ProtoScreen.VERYSMALL)
                        color: "white"
                    }
                }
            }
        }

        delegate: OMSCard {
            height: ProtoScreen.guToPx(5)
            elevation: 2
            width: parent.width
            refnum:  model.refnum
            qty: model.size
            price: model.price
            side: model.isbuy ? 1 : -1
            symbolp: model.symbol
        }
    }
}
