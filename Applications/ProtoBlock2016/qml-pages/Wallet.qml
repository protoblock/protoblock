import QtQuick 2.4
import QtQuick.Controls 2.3
//import ProRotoQml.Protoblock 1.0

import QtQuick.Controls 1.4 as C12
import Material 1.0 as Material
import Material.ListItems 1.0 as ListItems
import QtQuick.Layouts 1.1
import ProRotoQml.Theme 1.0
//import QtQuick.Controls.Material 2.3 as MaterialC
import QtQuick.Controls 2.3 as Mat2


Item {
    Component.onCompleted: {
        pageHelper.title = "Wallet";
        console.log("onCompleted netbal " + netbal)
        console.log("onCompleted stakebal " + stakebal)
        console.log("onCompleted maximumValue " + sendqty.maximumValue)
    }

    property string stakebal: !MiddleMan.pMyFantasyNameBalance ?  "0" : (MiddleMan.pMyFantasyNameBalance.stake).toLocaleString();
    property string skillbal: !MiddleMan.pMyFantasyNameBalance ?  "0" : (MiddleMan.pMyFantasyNameBalance.bits).toLocaleString();
    property string netbal: !MiddleMan.pMyFantasyNameBalance ?  "0" : (MiddleMan.pMyFantasyNameBalance.net).toLocaleString();

    ColumnLayout {
        spacing: ProtoScreen.guToPx(2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: ProtoScreen.guToPx(2)
        ListItems.Subtitled {
            Layout.preferredWidth: ProtoScreen.guToPx(80)
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

            valueText: "Skill[" + skillbal + " ƑɃ] Stake[" + stakebal +" ƑɃ] Net[" + netbal + " ƑɃ]";
            itemValueLabel.font.family: fontfamFB
        }

        Material.Card {
            id: sendcard
            Layout.preferredWidth: ProtoScreen.guToPx(80)
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

                Material.TextField {
                    id: nameto
                    property bool setting: false
                    property bool accepting: false
                    property int status: 0
                    placeholderText: "enter fantasyname"
                    inputMethodHints: Qt.ImhNoPredictiveText;
                    Layout.column: 2
                    Layout.row: 1
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.columnSpan: 3
                    Layout.fillHeight: true
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    implicitWidth: ProtoScreen.guToPx(50)
                    textColor: status === 0 ? Colors.black : status === 1 ? Colors.green : Colors.red
                    onTextChanged: {
                        console.log("nameto onTextChanged " +  textColor)
                        if ( focus && !setting ) {
                            MiddleMan.pAccountsModel.filterString = text;
                            MiddleMan.pAccountsModel.invalidate()
                            if ( !comboto.popup.visible && text !== "")
                                comboto.popup.open();

                            status = 0
                        }
                    }

                    onAccepted: {
                        accepting = true;
                        console.log("nameto onAccepted")
                        if ( text !== comboto.currentText ) {
                            var idx = comboto.find(text, Qt.MatchFixedString)
                            if (idx > -1) {
                                comboto.currentIndex = idx;
                                setting = true;
                                text = comboto.textAt(idx)
                                setting = false;
                                status = 1
                            }
                            else {
                                status = -1
                            }
                        }
                        else {
                            if ( comboto.currentIndex !== -1)
                                status = 1
                            else
                                status = -1
                        }

                        if ( comboto.popup.visible )
                            comboto.popup.close();

                        sendqty.forceActiveFocus();
                        accepting = false;
                    }

                    onFocusChanged: {
                        if ( focus )
                            textChanged()
                        else if ( !accepting && text !== "" )
                            accepted()
                    }


                }

                Mat2.ComboBox {
                    anchors.right: nameto.right
                    flat: true

                    inputMethodHints: Qt.ImhNoPredictiveText
                    id: comboto
                    model: MiddleMan.pAccountsModel
                    currentIndex: -1

                    textRole: "name"

                    Layout.column: 5
                    Layout.row: 1
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    implicitWidth: ProtoScreen.guToPx(20)

                    Component.onCompleted: {
                        model.sortAgain("name",Qt.AscendingOrder)
                    }

//                    onEditTextChanged: {
//                        console.debug("onEditTextChanged currentindex " + currentIndex + " currentText " + currentText + " edit " + editText + " displayText " + displayText)
//                    }

                    onActivated: {
                        console.debug("combo onActivated currentindex " + currentIndex + " currentText " + currentText + " edit " + editText + " displayText " + displayText)

                        if ( currentIndex !== -1) {
                            nameto.setting = true
                            nameto.text = currentText
                            sendqty.forceActiveFocus()
                            MiddleMan.pAccountsModel.filterString = currentText;
                            MiddleMan.pAccountsModel.invalidate()
                            nameto.status = 1
                            nameto.setting = false
                        }
                    }

                    onCurrentTextChanged: {
                        displayText = ""
                    }

                    onAccepted: {
                        console.debug("onAccepted currentindex " + currentIndex + " currentText " + currentText + " edit " + editText + " displayText " + displayText)
                    }

//                    onCurrentIndexChanged: {
//                        console.debug("onCurrentIndexChanged currentindex " + currentIndex + " currentText " + currentText + " edit " + editText + " displayText " + displayText)
//                    }

//                    onFocusChanged: {
//                        console.debug("onFocusChanged currentindex " + currentIndex + " currentText " + currentText + " edit " + editText + " displayText " + displayText)
//                    }

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

                C12.SpinBox {
                    id: sendqty
                    enabled: MiddleMan.liveSync === "Live"
//                    decimals: 0
                    stepSize: 1.0
                    maximumValue: Math.max(0, Math.min(MiddleMan.pMyFantasyNameBalance.net, MiddleMan.pMyFantasyNameBalance.stake))
                    minimumValue: 0
//                    to: Math.max(0, Math.min(MiddleMan.pMyFantasyNameBalance.net, MiddleMan.pMyFantasyNameBalance.stake))
//                    from: 0

                    value: 0
                    Layout.column: 2
                    Layout.row: 2
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: false
                    Layout.preferredWidth: ProtoScreen.guToPx(9)
                    onValueChanged: {
                        console.log("netbal " + netbal)
                        console.log("stakebal " + stakebal)
                        console.log("maximumValue " + maximumValue)
                        console.log("maxnim" + Math.max(0, Math.min(netbal, stakebal)))

                    }

                }

                Material.Button {
                    enabled: MiddleMan.liveSync === "Live" && sendqty.value > 0 && nameto.status === 1
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
                        console.log ( " onClicked " + nameto.status);
                        if ( nameto.status === 0) {
                            console.log ( " onClicked black");
                            nameto.accepted()
                        }

                        if ( nameto.status === -1) {
//                            nameto.forceActiveFocus()
                            console.log ( " onClicked red");
                        }
                        else {
                            mytransferdialog.amount = sendqty.value
                            mytransferdialog.to = nameto.text
                            mytransferdialog.show()
                        }
                    }
                    Layout.preferredHeight: ProtoScreen.guToPx(3)
                    Layout.preferredWidth: ProtoScreen.guToPx(9)

                }
            }
        }
    }

    Material.Dialog {
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
                if ( mytransferdialog.amount > 0 &&
                        mytransferdialog.amount <= Math.min(MiddleMan.pMyFantasyNameBalance.net, MiddleMan.pMyFantasyNameBalance.stake)) {
                    MiddleMan.doTransfer(mytransferdialog.amount,mytransferdialog.to);
                }
                else {
                    console.log("not enough funds" + Math.min(MiddleMan.pMyFantasyNameBalance.net, MiddleMan.pMyFantasyNameBalance.stake))  ;
                }
            }

        }

    }

}
