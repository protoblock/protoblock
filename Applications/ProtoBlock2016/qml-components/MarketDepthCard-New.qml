import QtQuick 2.4
import QtQuick.Layouts 1.1
import Material 1.0
import  ProRotoQml.Theme 1.0


Item {
    id: marketDepthCard
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
        elevation: marketDepthCard.elevation
        anchors.fill: parent
        border.color: "red"
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
            spacing: 1
//            Label {
//                id: label0
//                text: { (buySize * numppg * buy) === 0 ? "" :
//                       Math.round(buy / numppg).toString()
//                }
//                Layout.fillHeight: true
//                Layout.fillWidth: false
//                Layout.preferredWidth: (parent.width / colums) - 2
//                verticalAlignment: Text.AlignVCenter
//                horizontalAlignment: Text.AlignHCenter
//                color:  ((buySize * numppg * buy) === 0 ? 0 :
//                        Math.round(buy / numppg) ) > numgames
//                        ? "red" : Theme.light.textColor
//            }

            Label {
                id: label01
                text: { (buySize * numgames * buy) === 0 ? "" :
                       Math.round(buy / numgames).toString()
                }
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / colums) - 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color:  ((buySize * numgames * buy) === 0 ? 0 :
                        Math.round(buy / numgames) ) > numppg
                        ? "red" : Theme.light.textColor
            }


            Label {
                id: label1
                text: buySize > 0 ? buySize.toString() : ""
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / colums) - 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label2
                text: buy > 0 ? buy.toString() : ""
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / colums) - 2
                horizontalAlignment: Text.AlignHCenter
                color: label01.text != "" ? label01.color :
                       label0.text != "" ? label0.color : Theme.light.textColor
            }
        }
    }
}
