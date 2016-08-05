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
    height: maximumLineCount == 2 ? ProtoScreen.guToPx(8)  : ProtoScreen.guToPx(10)

    Card{
        elevation: marketDepthCard.elevation
        anchors.fill: parent
        border.color: "black"
        RowLayout {
            id: rowLayout1
            anchors.fill: parent
            Rectangle{
                id: info
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth:   2
                color: infoColor
            }
            Label {
                id: label1
                text: buySize > 0 ? buySize.toString() : ""
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 4) - 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label2
                text: buy > 0 ? buy.toString() : ""
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 4) - 2
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label3
                text: sell > 0 ? sell.toString() : ""
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 4) - 2
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label4
                text: sellSize >0 ? sellSize.toString() : ""
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width / 4) - 2
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
