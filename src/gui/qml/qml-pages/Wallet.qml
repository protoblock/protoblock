import QtQuick 2.6
import QtQuick.Controls 2.3
//import ProRotoQml.Protoblock 1.0

import QtQuick.Controls 1.4 as C12
import Material 1.0 as Material
import Material.ListItems 1.0 as ListItems
import QtQuick.Layouts 1.3
import ProRotoQml.Theme 1.0
//import QtQuick.Controls.Material 2.3 as MaterialC
import QtQuick.Controls 2.3 as Mat2
import ProRotoQml.Theme 1.0


Item {
    id: walltti
    Component.onCompleted: {
        pageHelper.title = "Wallet";
//        console.log("onCompleted netbal " + netbal)
//        console.log("onCompleted stakebal " + stakebal)
//        console.log("onCompleted maximumValue " + sendqty.maximumValue)
        console.log("wallet onCompleted walltti " + walltti.width)
        console.log("wallet onCompleted stackView " + stackView.width)

    }
    property string btcadddr: !MiddleMan.pMyFantasyNameBalance ?  "" :
                              MiddleMan.pMyFantasyNameBalance.btcaddr;
    property int satbal: !MiddleMan.pMyFantasyNameBalance ?  0 :
                          MiddleMan.bitcoinBalance

    property real btcbal: satbal * .00000001
    property string sbtcbal: btcbal.toLocaleString();

    property string stakebal: !MiddleMan.pMyFantasyNameBalance ?  "0" : (MiddleMan.pMyFantasyNameBalance.stake).toLocaleString();
    property string skillbal: !MiddleMan.pMyFantasyNameBalance ?  "0" : (MiddleMan.pMyFantasyNameBalance.bits).toLocaleString();
    property string netbal: !MiddleMan.pMyFantasyNameBalance ?  "0" : (MiddleMan.pMyFantasyNameBalance.net).toLocaleString();

    property bool fbselected: true

    TextEdit {
        id: cliphelper
        visible: false
    }
    ColumnLayout {
        width: ProtoScreen.guToPx(80)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.margins: ProtoScreen.guToPx(3)
        spacing: ProtoScreen.guToPx(3)

        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: ProtoScreen.guToPx(10)

            ListItems.Subtitled {
                border.color: fbselected ? "#2580a6" : "transparent"
                radius: 10
                elevation: 1
                text: "Fantasybit"
//                subText: "Skill[" + skillbal + " ƑɃ] Stake[" + stakebal +" ƑɃ] Net[" + netbal + " ƑɃ]";
//                itemSubLabel.font.family: fontfamFB
                valueText: netbal
                action:Image{
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/fb.png"
                }
                itemLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                itemValueLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.NORMAL))
                itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))

                itemValueLabel.font.bold: true
                interactive: true
                onClicked: {
                    fbselected = true
                }
            }

            ListItems.Subtitled {
                border.color: !fbselected ? "#FF9900" : "transparent"

                radius: 10
                elevation: 1
                text: "Bitcoin"
//                subText: btcadddr
                valueText: sbtcbal

                action:Image{
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/btc.png"
                }
                itemLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                itemValueLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.NORMAL))
                itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                itemValueLabel.font.bold: true

                interactive: true
                onClicked: {
                    fbselected = false
                }
            }
        }

//        Rectangle {
//            border.color: fbselected ? "#2580a6" : "#FF9900";
//            anchors.horizontalCenter: parent.horizontalCenter
//            width: ProtoScreen.guToPx(80)
//            Layout.fillHeight: true
//            height: stackView.height

        StackLayout  {
            id: stackView
    //        anchors.fill: parent
            currentIndex: fbselected ? 0 : 1
            anchors.margins: ProtoScreen.guToPx(1)

            ColumnLayout {
                spacing: ProtoScreen.guToPx(1)
                anchors.horizontalCenter: parent.horizontalCenter

//                anchors.top: parent.top
//                anchors.left: parent.left
//                anchors.margins: ProtoScreen.guToPx(2)

                ListItems.Subtitled {
                    id: thelist
                    elevation: 1
                    text: MiddleMan.pMyFantasyNameBalance.name
//                    subText: MiddleMan.pMyFantasyNameBalance.pk
                    subText: "Skill[" + skillbal + " ƑɃ] Stake[" + stakebal +" ƑɃ] Net[" + netbal + " ƑɃ]";
                    itemSubLabel.font.family: fontfamFB
                    action:Image{
                        height: parent.height
                        width : height
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icons/action_account_circle.png"
                    }
                    itemLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                    itemValueLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                    itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))

    //                valueText: "Skill[" + skillbal + " ƑɃ] Stake[" + stakebal +" ƑɃ] Net[" + netbal + " ƑɃ]";
    //                itemValueLabel.font.family: fontfamFB
        //            secondaryItem: Label {
        //                Layout.preferredWidth: ProtoScreen.guToPx(10)
        //                Layout.fillWidth: true
        //                Layout.fillHeight: false
        //                text: MiddleMan.toBTCAddess(MiddleMan.pMyFantasyNameBalance.pk);
        //            }
                }

                Material.Card {
                    id: sendcard
                    Layout.preferredWidth: ProtoScreen.guToPx(80)
                    Layout.preferredHeight: ProtoScreen.guToPx(18)
        //            height: ProtoScreen.guToPx(30)
        //            width: ProtoScreen.guToPx(100)
        //            anchors.margins: ProtoScreen.guToPx(1)

                    GridLayout {
                        anchors.centerIn: parent
                        id: scardgrid
                        anchors.fill: parent
                        anchors.margins: ProtoScreen.guToPx(2)
                        columns: 8
                        rows: 3
                        Label {
                            text: "Send to:"
                            Layout.preferredWidth: ProtoScreen.guToPx(10)

                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignRight

                            Layout.alignment: Qt.AlignCenter
                            Layout.column: 1
                            Layout.row: 1
                            Layout.columnSpan: 2
                            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                        }

                        Material.TextField {
                            id: nameto
                            property bool setting: false
                            property bool accepting: false
                            property int status: 0
                            placeholderText: "enter fantasyname"
                            inputMethodHints: Qt.ImhNoPredictiveText;
                            Layout.column: 3
                            Layout.row: 1
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: true
                            Layout.columnSpan: 3
        //                    Layout.fillHeight: true
                            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                            Layout.preferredWidth: ProtoScreen.guToPx(50)
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
        //                                comboto.currentIndex = idx;
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

                            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                            anchors.right: nameto.right
                            flat: true
        //                    highlighted: false
                            inputMethodHints: Qt.ImhNoPredictiveText
                            id: comboto
                            model: MiddleMan.pAccountsModel
                            currentIndex: -1

                            textRole: "name"

                            Layout.column: 6
                            Layout.row: 1
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: true
                            Layout.columnSpan: 2
                            Layout.preferredWidth: ProtoScreen.guToPx(20)

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
                                    currentIndex = -1
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
                            Layout.columnSpan: 2
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
                            Layout.column: 3
                            Layout.row: 2
                            Layout.alignment: Qt.AlignRight
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
                            enabled: MiddleMan.allowAction && sendqty.value > 0 && nameto.status === 1
                            elevation: 2
                            id: sendbut
                            text: "send"
        //                    backgroundColor: themeroot.theme.accentColor
                            Layout.column: 2
                            Layout.row: 3
                            Layout.columnSpan: 2
                            Layout.alignment: Qt.AlignCenter
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
                                    mytransferdialog.isbtc = false
                                    mytransferdialog.currency = "Fantasybits"
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

            ColumnLayout {
                spacing: ProtoScreen.guToPx(2)
//                anchors.top: parent.top
//                anchors.left: parent.left
//                anchors.margins: ProtoScreen.guToPx(2)

                ListItems.Subtitled {
                    elevation: 1
                    text: btcadddr
                    action:Image{
                        height: parent.height
                        width : height
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/icons/action_account_circle.png"
                    }
                    itemLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                    itemValueLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                    itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                    onClicked: {
                        cliphelper.text = text
                        cliphelper.selectAll()
                        cliphelper.copy()
                        ToolTip.show("address copied" ,2000)
                    }
                }

                Material.Card {
                    id: sendbtccard
                    Layout.preferredWidth: ProtoScreen.guToPx(80)
                    Layout.preferredHeight: ProtoScreen.guToPx(18)

                    GridLayout {
                        anchors.centerIn: parent
                        anchors.fill: parent
                        anchors.margins: ProtoScreen.guToPx(2)
                        columns: 8
                        rows: 3
                        Label {
                            text: "Send to:"
                            Layout.preferredWidth: ProtoScreen.guToPx(10)

                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignRight

                            Layout.alignment: Qt.AlignCenter
                            Layout.column: 1
                            Layout.row: 1
                            Layout.columnSpan: 2
                            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                        }

                        Material.TextField {
                            id: bnameto
                            property bool setting: false
                            property bool accepting: false
                            property int status: 0
                            placeholderText: "enter bitcoinaddress"
                            inputMethodHints: Qt.ImhNoPredictiveText;
                            Layout.column: 3
                            Layout.row: 1
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: true
                            Layout.columnSpan: 4
                            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                            Layout.preferredWidth: ProtoScreen.guToPx(50)
                            textColor: status === 0 ? Colors.black : status === 1 ? Colors.green : Colors.red
                            onTextChanged: {
                                console.log("nameto onTextChanged " +  textColor)
                                status = 0;
                            }

                            onAccepted: {
                                accepting = true;
                                console.log("nameto onAccepted")

                                if ( MiddleMan.isValidBitcoinAddress(text) )
                                    status = 1
                                else {
                                    status = -1
                                    console.log("invalid isValidBitcoinAddress" + text)
                                }

                                btcbox.forceActiveFocus();
                                accepting = false;
                            }

                            onFocusChanged: {
                                if ( focus )
                                    textChanged()
                                else if ( !accepting && text !== "" )
                                    accepted()
                            }
                        }

                        Label {
                            text: "Amount:"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignRight

                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignCenter
                            Layout.column: 1
                            Layout.row: 2
                            Layout.columnSpan: 2
                            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        }

                        SpinBox {
                            Layout.column: 3
                            Layout.row: 2

                            id: btcbox
                            editable: true
                            wheelEnabled: editable
                            enabled: editable

                            stepSize: 10000
                            from: 0
                            //max int is 2147483647 = that 21 times 100 million
                            to: walltti.satbal
                            property int decimals: 8
                            property double realValue: value * .00000001
                            textFromValue: function(value, locale) {
                                console.log( " btcbox from value" + value)
                                return Number(value * .00000001 ).toLocaleString(Qt.locale("en-US"), 'f', btcbox.decimals) + " BTC"
                            }

                            valueFromText: function(text, locale) {
                                console.log( " btcbox from text " + text)
                                var t = text.split(" ")[0]
                                var d = Number.fromLocaleString(Qt.locale("en-US"), t)
                                var dd = d * 100000000
                                return Math.round(dd * 100000000) / 100000000
                            }

                            value: 0
                       }

                        Material.Button {
                            enabled: MiddleMan.allowAction && btcbox.value > 0 && bnameto.status === 1
                            elevation: 2
                            text: "send"
                            Layout.column: 2
                            Layout.row: 3
                            Layout.columnSpan: 2
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            onClicked : {
                                console.log ( " onClicked " + bnameto.status);
                                if ( bnameto.status === 0) {
                                    console.log ( " bnameto onClicked black");
                                    bnameto.accepted()
                                }

                                if ( bnameto.status === -1) {
                                    console.log ( "bnameto onClicked red");
                                }
                                else {
                                    mytransferdialog.isbtc = true
                                    mytransferdialog.currency = "Bitcoin Satoshis"
                                    mytransferdialog.amount = btcbox.value
                                    mytransferdialog.to = bnameto.text
                                    mytransferdialog.show()
                                }
                            }
                            Layout.preferredHeight: ProtoScreen.guToPx(3)
                            Layout.preferredWidth: ProtoScreen.guToPx(9)

                        }
                    }
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
        property string currency: "Fantasybits"
        property int amount
        property bool isbtc: false
        positiveButtonText: "Send Now"
        title: "Confirm " + currency + " Transfer "
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
                text:  "Transfer: " + (mytransferdialog.amount).toLocaleString() + " " + mytransferdialog.currency
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
            if ( realRoot.uname === "" )  {
                if ( !isbtc ) {
                    if ( mytransferdialog.amount > 0 &&
                            mytransferdialog.amount <= Math.min(MiddleMan.pMyFantasyNameBalance.net, MiddleMan.pMyFantasyNameBalance.stake)) {
                        MiddleMan.doTransfer(mytransferdialog.amount,mytransferdialog.to);
                    }
                    else {
                        console.log("not enough funds" + Math.min(MiddleMan.pMyFantasyNameBalance.net, MiddleMan.pMyFantasyNameBalance.stake))  ;
                    }
                }
                else {
                    if ( mytransferdialog.amount > 0 &&
                            mytransferdialog.amount <= satbal) {
                        MiddleMan.doBtcTransfer(mytransferdialog.amount,mytransferdialog.to);
                    }
                    else {
                        console.log("not btc enough funds " + satbal;
                    }
                }
            }

        }

    }

}
