import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Private 1.0


Item {
    id: omscard
    property int side
    property int qty
    property int price
    property double wid: parent.width / 7.0
    property string symbolp
    property string refnum
    property int elevation : 0
    property int maximumLineCount: 2
    height: maximumLineCount == 2 ? ProtoScreen.guToPx(8)  : ProtoScreen.guToPx(10)

    Material.Card{
        elevation: omscard.elevation
        anchors.fill: parent
        border.color: "black"

        RowLayout {
            id: rowLayout1
            anchors.fill: parent
            spacing: 0

            Material.IconButton{
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: wid
                color: "red"
                onClicked: {
                    MiddleMan.doCancel(refnum)
                }
                action: Material.Action {
                    iconName: "awesome/times"
                }
            }

            Material.Label {
                id: label1
                text: symbol
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: wid * 2.0
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            }


            Material.Label {
                id: label3
                text: side > 0 ? "buy" : side < 0 ? "sell" : "?"
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: wid
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)

            }

            Material.Label {
                id: label5
                text:  qty.toString()
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: wid
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)

            }

            Material.Label {
                id: label4
                text: price.toString()
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: wid
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)

            }


            Material.Label {
                id: label2
                text: refnum
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: wid
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            }


        }
    }
}
