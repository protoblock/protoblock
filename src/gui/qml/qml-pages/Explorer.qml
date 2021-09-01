import QtQml.Models 2.2
import QtQml 2.2
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Material 1.0
import ProRotoQml.Theme 1.0

Item {
    Component.onCompleted: {
        pageHelper.title = "Explorer";
        MiddleMan.set_blocknum_string_num(blocknum.value)
    }
    id: explorer
    anchors.fill: parent
    ColumnLayout {
        spacing: ProtoScreen.guToPx(2)
//        anchors.top: parent.top
//
        anchors.left: parent.left
        anchors.margins: ProtoScreen.guToPx(2)
//        height: parent.height
        anchors.fill: parent
        Card {
            Layout.preferredWidth: ProtoScreen.guToPx(80)
            Layout.preferredHeight: ProtoScreen.guToPx(10)

            GridLayout {
//                anchors.fill: parent
                anchors.margins: ProtoScreen.guToPx(1)
                anchors.centerIn: parent
                columns: 2
                rows: 1
                Label {
                    id: lblblock
                    text: "Block:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                }

                SpinBox {
                    focus: true
                    id: blocknum
                    enabled: MiddleMan.liveSync === "Live"
                    decimals: 0
                    stepSize: 1.0
                    maximumValue: MiddleMan.height
                    minimumValue: 0
                    value: MiddleMan.height
                    Layout.column: 2
                    Layout.row: 1
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: false
                    Layout.preferredWidth: ProtoScreen.guToPx(9)
                    onValueChanged: {
                        MiddleMan.set_blocknum_string_num(blocknum.value)
                    }
                }
            }
        }

        TextArea {
            Layout.preferredWidth: ProtoScreen.guToPx(80)
            Layout.preferredHeight: contentHeight
            Layout.fillHeight: true

//            anchors.horizontalCenter: parent.horizontalCenter
            text: MiddleMan.blocknum_string
        }
    }
}
