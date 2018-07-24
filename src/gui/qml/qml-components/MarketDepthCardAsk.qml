import QtQuick 2.4
import QtQuick.Layouts 1.1
import Material 1.0
import  ProRotoQml.Theme 1.0


Item {
    id: marketDepthCardAsk
    property int buySize
    property int buy
    property int sellSize
    property int sell
    property string infoColor: "green"
    property int elevation : 0
    property int maximumLineCount: 2
    property int numgames: 16
    property int maxnumgames: 16
    property int numppg: 20
    property int maxppg: 40
    property int colums: 3
//    property bool isppg: false
    height: maximumLineCount == 2 ? ProtoScreen.guToPx(8)  : ProtoScreen.guToPx(10)

    Card{
        elevation: marketDepthCardAsk.elevation
        anchors.fill: parent
        border.color: "black"
        RowLayout {
            id: rowLayout1
            anchors.fill: parent
//            Rectangle{
//                id: info
//                Layout.fillHeight: true
//                Layout.fillWidth: false
//                Layout.preferredWidth:   2
//                color: infoColor
//            }
            spacing: 0
            Label {
                id: label3
                text: sell > 0 ? sell.toString() : ""
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 5.0) * 2.0
                horizontalAlignment: Text.AlignHCenter
                color: label7.text != "" ? label7.color :Theme.light.textColor
//                       label6.text != "" ? label6.color : Theme.light.textColor
            }

            Label {
                id: label4
                text: sellSize >0 ? sellSize.toString() : ""
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 5.0) * 2.0
                width: (parent.width / 5.0) * 2.0

                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label7
                text: { (sellSize * numgames * sell) === 0 ? "" :
                       Math.round(sell / numgames).toString()
                }
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 5.0)
                width: (parent.width / 5.0)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color:  ((sellSize * numgames * sell) === 0 ? 0 :
                        Math.round(sell / numgames) ) < numppg
                        ? "green" : Theme.light.textColor

                font.pixelSize: ProtoScreen.font(ProtoScreen.TINY)

            }





        }
    }
}
