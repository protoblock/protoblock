import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import Qt.labs.settings 1.0

import Material.ListItems 1.0 as ListItems

import ProRotoQml.Theme 1.0

Pane {
    id: swappane
    property string btcadddr: MiddleMan.toBTCAddess(MiddleMan.pMyFantasyNameBalance.pk);

    ColumnLayout {
        width: parent.width / 2.0
        anchors.left: parent.left
        id: columnLayout
        //        anchors.bottomMargin: 268
        anchors.top: parent.top

        ListItems.Subtitled {
            elevation: 1
            Layout.fillWidth: false
            Layout.preferredWidth: ProtoScreen.guToPx(50)
            text: MiddleMan.pMyFantasyNameBalance.name
            valueText: "1BPw53Gf5faxAwSmXrCqHDWkhM1xK2jUX9";//btcadddr
            subText: MiddleMan.toBTCbalance(valueText);

            action:Image{
                height: parent.height
                //                width : height
                fillMode: Image.PreserveAspectFit
                source: "qrc:/btc.png"
            }
            //            iconSource:  "qrc:/btc.png"
            itemLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
            itemValueLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
            itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))


            //            itemValueLabel.font.family: fontfamFB
        }

        RowLayout {
            Button {
                id: button
                text: qsTr("Deposit")
            }

            Button {
                id: button1
                text: qsTr("Withdraw")
            }
        }

        QRCode {
            id: qrc
//            anchors.centerIn: parent
//            anchors.fill: parent
//            anchors.horizontalCenter: parent.horizontalCenter
            value: btcadddr
            level: "H"
        }

    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
