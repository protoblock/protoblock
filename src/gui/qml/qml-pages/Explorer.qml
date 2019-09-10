import QtQuick 2.4
import QtQuick.Controls 1.4
//import ProRotoQml.Protoblock 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItems
import QtQuick.Layouts 1.1
import ProRotoQml.Theme 1.0

Item {
    Component.onCompleted: {
        pageHelper.title = "Explorer";
        MiddleMan.set_blocknum_string_num(blocknum.value)
    }
    id: explorer
    anchors.fill: parent
    ColumnLayout {
        width: Math.min(parent.width * .90,  ProtoScreen.guToPx(100 ))
        height: parent.height
//        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: ProtoScreen.guToPx(3)
        spacing: ProtoScreen.guToPx(3)

//        spacing: ProtoScreen.guToPx(2)
//        anchors.top: parent.top
//
//        anchors.left: parent.left
//        anchors.margins: ProtoScreen.guToPx(2)
//        height: parent.height
//        anchors.fill: parent
        Card {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height * .10

            GridLayout {
//                anchors.fill: parent
                Layout.fillWidth: true
                width: parent.width / 4
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
//                    Layout.preferredWidth: parent.width * .10
                    onValueChanged: {
                        MiddleMan.set_blocknum_string_num(blocknum.value)
                    }
                    font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                }

//                Button {
//                    enabled: MiddleMan.liveSync === "Live"
//                    elevation: 2
//                    id: sendbut
//                    text: "explore"
//                    Layout.column: 2
//                    Layout.row: 2
//                    Layout.alignment: Qt.AlignLeft
//                    Layout.fillWidth: false
//                    Layout.fillHeight: false
//                    onClicked : {
//                        MiddleMan.set_blocknum_string_num(blocknum.value)
//                        console.log(MiddleMan.blocknum_string)
//                    }
//                    Layout.preferredHeight: ProtoScreen.guToPx(3)
//                    Layout.preferredWidth: ProtoScreen.guToPx(9)

//                }
            }
        }

        TextArea{
            Layout.preferredWidth: parent.width
//            Layout.preferredHeight: contentHeight
            Layout.preferredHeight: parent.height * .85
            Layout.fillHeight: true

            textMargin: ProtoScreen.guToPx(3)

//            anchors.horizontalCenter: parent.horizontalCenter
            text: MiddleMan.blocknum_string
        }
    }
}
