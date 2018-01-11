import QtQuick 2.4
import QtQuick.Controls 1.4
//import ProRotoQml.Protoblock 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItems
import QtQuick.Layouts 1.1
import ProRotoQml.Theme 1.0

Item {
    Component.onCompleted: { pageHelper.title = "Wallet"; }

    property var stakebal: !MiddleMan.pMyFantasyNameBalance ?  "0" : (MiddleMan.pMyFantasyNameBalance.stake).toLocaleString();
    property var skillbal: !MiddleMan.pMyFantasyNameBalance ?  "0" : (MiddleMan.pMyFantasyNameBalance.bits).toLocaleString();

    ColumnLayout {
        spacing: ProtoScreen.guToPx(2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: ProtoScreen.guToPx(2)
        ListItems.Subtitled {
            Layout.preferredWidth: ProtoScreen.guToPx(60)
            id: thelist
            elevation: 1
            text: MiddleMan.pMyFantasyNameBalance.name
            subText: MiddleMan.pMyFantasyNameBalance.pk
            action:Image{
                height: parent.height
                width : height
                fillMode: Image.PreserveAspectFit
                source: "qrc:/icons/action_account_circle.png"
            }
            itemLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
            itemValueLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
            itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))

            valueText: "Skill(" + skillbal + " ƑɃ) Stake(" + stakebal +" ƑɃ)"
            itemValueLabel.font.family: fontfamFB
        }

        Card {
            id: sendcard
            Layout.preferredWidth: ProtoScreen.guToPx(60)
            Layout.preferredHeight: ProtoScreen.guToPx(12)
//            height: ProtoScreen.guToPx(30)
//            width: ProtoScreen.guToPx(100)
//            anchors.margins: ProtoScreen.guToPx(1)
            GridLayout {
                id: scardgrid
                anchors.fill: parent
                anchors.margins: ProtoScreen.guToPx(1)
                columns: 6
                rows: 3
                Label {
                    text: "Send To:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 1
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    Layout.fillWidth: true
                }

                TextField {
                    id: nameto
                    placeholderText: "to name"
                    inputMethodHints: Qt.ImhNoPredictiveText;
                    Layout.column: 2
                    Layout.row: 1
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.columnSpan: 5
                    Layout.fillHeight: true
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    implicitWidth: ProtoScreen.guToPx(40)
                }


                Label {
                    text: "Amount:"

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter
                    Layout.column: 1
                    Layout.row: 2
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                }

                SpinBox {
                    id: sendqty
                    decimals: 0
                    stepSize: 1.0
                    maximumValue: MiddleMan.pMyFantasyNameBalance.bits
                    minimumValue:  0
                    value: 0
                    Layout.column: 2
                    Layout.row: 2
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: false
                    Layout.preferredWidth: ProtoScreen.guToPx(9)
                }


                Button {
                    enabled: MiddleMan.liveSync === "Live" && sendqty.value > 0 && nameto.text !== ""
                    elevation: 2
                    id: sendbut
                    text: "send"
//                    backgroundColor: themeroot.theme.accentColor
                    Layout.column: 2
                    Layout.row: 3
//                    Layout.columnSpan: 1
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    onClicked : {
                        mytransferdialog.amount = sendqty.value
                        mytransferdialog.to = nameto.text
                        mytransferdialog.show()
                    }
                    Layout.preferredHeight: ProtoScreen.guToPx(3)
                    Layout.preferredWidth: ProtoScreen.guToPx(9)

                }
            }
        }
    }

    Dialog {
        id: mytransferdialog
        height: ProtoScreen.guToPx(34)
        minimumHeight: ProtoScreen.guToPx(8)
        width: ProtoScreen.guToPx(66)
        minimumWidth: ProtoScreen.guToPx(16)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        property string to: "to"
        property int amount
        positiveButtonText: "Send Now"
        title: "Confirm Transfer "
        text: "Send From: " + realRoot.uname
        dialogContent: Column {
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: ProtoScreen.guToPx(.5)

            Text{
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                text:  "Transfer: " + (mytransferdialog.amount).toLocaleString() + " Fantasybits"
                font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            }

            Text{
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                text:  "To: " + mytransferdialog.to
                font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            }

        }

        onAccepted: {
            if ( realRoot.uname !== "" )  {
                if ( mytransferdialog.amount <= MiddleMan.pMyFantasyNameBalance.bits) {
                    MiddleMan.doTransfer(mytransferdialog.amount,mytransferdialog.to)
               }
            }

        }

    }

}
