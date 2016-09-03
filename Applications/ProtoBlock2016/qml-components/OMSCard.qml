import QtQuick 2.4
import QtQuick.Layouts 1.1
import Material 1.0
import  ProRotoQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0


Item {
    id: omscard
    property int side
    property int qty
    property int price
    property string symbol
    property string refnum
    property int elevation : 0
    property int maximumLineCount: 2
    height: maximumLineCount == 2 ? ProtoScreen.guToPx(8)  : ProtoScreen.guToPx(10)

    Card{
        elevation: omscard.elevation
        anchors.fill: parent
        border.color: "black"

        RowLayout {
            id: rowLayout1
            anchors.fill: parent


            IconButton{
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 6) - 2
//                hasColor: true
                color: "red"
                iconSource: "qrc:/icons/navigation_close.png"
                onClicked: {
                    MiddleMan.doCancel(refnum)
                }
            }

//            Rectangle{
//                id: info
//                Layout.fillHeight: true
//                Layout.fillWidth: false
//                Layout.preferredWidth:   2
//                color: infoColor
//            }
            Label {
                id: label1
                text: symbol
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 6) - 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label2
                text: refnum
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 6) - 2
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label3
                text: side > 0 ? "buy" : side < 0 ? "sell" : "?"
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 6) - 2
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label4
                text: price.toString()
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 6) - 2
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label5
                text:  qty.toString()
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 6) - 2
                horizontalAlignment: Text.AlignHCenter
            }

        }
    }
}
