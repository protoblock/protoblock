import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import Qt.labs.settings 1.0

import Material.ListItems 1.0 as ListItems

import ProRotoQml.Theme 1.0
import Material 1.0 as Material


//Item {

Item {
    id: swappane
    property string btcadddr: MiddleMan.toBTCAddess(MiddleMan.pMyFantasyNameBalance.pk);
    property real btcbal: 3.089109// MiddleMan.toBTCbalance(swappane.btcadddr);
    property string sbtcbal: btcbal.toLocaleString();
    property int fbbal: MiddleMan.pMyFantasyNameBalance.net
    property bool btcsell: true
    property real swaprate: ratesb.value / 1000000
    property int satoshirate: ratesb.value

    ColumnLayout {
        width: ProtoScreen.guToPx(80)
        anchors.horizontalCenter: parent.horizontalCenter
        id: columnLayout
        //        anchors.bottomMargin: 268
        anchors.top: parent.top
        anchors.margins: ProtoScreen.guToPx(3)

        GridLayout {
//            id: rlay
//            Layout.alignment: Qt.AlignHCenter
//            anchors.horizontalCenter: parent.horizontalCenter
//            width: parent.width
            columnSpacing: ProtoScreen.guToPx(2)
            Layout.fillWidth: true
            Layout.fillHeight: true
            anchors.centerIn: parent
            anchors.fill: parent
            id: grid
            columns: 5
//            width: parent.
//            Layout.alignment: Qt.AlignCenter
//            layoutDirection: swappane.btcsell ? Qt.LeftToRight : Qt.RightToLeft

            Label {
                text: "SEND"
                Layout.column: 1
                Layout.columnSpan: 2
                Layout.row: 1
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                font.pixelSize: (ProtoScreen.font(ProtoScreen.NORMAL))
            }

            Label {
                Layout.row: 1
                Layout.column: 3
                Layout.columnSpan: 1
            }

            Label {
                text: "RECEIVE"
                Layout.column: 4
                Layout.columnSpan: 2
                Layout.row: 1
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                font.pixelSize: (ProtoScreen.font(ProtoScreen.NORMAL))
            }

            ListItems.Subtitled {
                id: ibtc
                Layout.row: 2
                Layout.column: swappane.btcsell ? 1 : 4
                Layout.columnSpan: 2
                border.color: "#FF9900"
                radius: 10
                elevation: 0
                Layout.fillWidth: true
                Layout.preferredWidth: ProtoScreen.guToPx(40)
                text: "Bitcoin"
                subText: (!swappane.btcsell ? //("1 ƑɃ = " + swappane.satoshirate.toLocaleString()  + " SAT")
                                              ("1 ƑɃ = " +  Number(swappane.swaprate.toLocaleString(Qt.locale("en-US"), 'f', 8)) + " BTC")
                                            : swappane.sbtcbal  + " BTC");
                action:Image{
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/btc.png"
                }
                itemLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                itemValueLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
            }

            Material.IconButton {
                Layout.row: 2
                Layout.column: 3
                Layout.columnSpan: 1
                Layout.alignment: Qt.AlignCenter
                Layout.fillWidth: false

                size: ProtoScreen.guToPx(5)
                action: Material.Action {
                    iconName: "awesome/exchange"
                    hoverAnimation: true
                }
                onClicked : {
                    swappane.btcsell = !swappane.btcsell
                }
            }

            ListItems.Subtitled {
                Layout.row: 2
                id: ifb
                border.color: "#2580a6"
                Layout.column: !swappane.btcsell ? 1 : 4
                Layout.columnSpan: 2
                radius: 10
                elevation: 0
                Layout.fillWidth: true
                Layout.preferredWidth: ProtoScreen.guToPx(40)
                text: "Fantasybit"
                subText: (swappane.btcsell ? ("1 BTC = " + (1.0/swappane.swaprate).toLocaleString())
                                            : (MiddleMan.pMyFantasyNameBalance.net).toLocaleString())
                          + " ƑɃ";

                action:Image{
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/fb.png"
                }
                itemLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                itemValueLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                itemSubLabel.font.family: fontfamFB
            }

            //fb input
            SpinBox {
                id: fbbox

                Layout.row: 3
                Layout.column: !swappane.btcsell ? 1 : 4
//                Layout.columnSpan: 2
//                Layout.preferredWidth: ProtoScreen.guToPx(25)

//                Layout.alignment: Qt.AlignCenter

                editable: !swappane.btcsell
                wheelEnabled: editable
                focus: editable
                focusPolicy: editable ? Qt.StrongFocus : Qt.NoFocus
                enabled: editable

                stepSize: 1
                from: 0
                to:  editable ? swappane.fbbal : Math.round(swappane.btcbal / swappane.swaprate )//(swappane.btcbal * swappane.swaprate)
                textFromValue: function(value, locale) {
                    console.log( "fb from value " + value)

                    return Number(value).toLocaleString(locale, 'f', 0) + " ƑɃ";
                }

                valueFromText: function(text, locale) {
                    console.log( "fb from text " + text)
                    var t = text.split(" ")[0]
                    var d = Number.fromLocaleString(Qt.locale("en-US"), t)
                    return d;
                }

                onValueChanged: {
                    console.log("fbbox new value " + value)
//                    if ( editable )
//                        btcbox.value = (value * swappane.satoshirate)
                }


//                font.family: fontfamFB
                value: editable ? 0 :  Math.round((btcbox.value * .00000001) / swappane.swaprate )//Number(btcbox.value / swappane.satoshirate)
//                /fbbox.value = Math.round((btcbox.value * .00000001) / swappane.satoshirate )

            }

            SpinBox {
                id: ratesb

                Layout.row: 3
                Layout.column: 3
//                Layout.columnSpan: 2
//                Layout.alignment: Qt.AlignCenter

                editable: true
                wheelEnabled: editable
                focus: editable
                focusPolicy: editable ? Qt.StrongFocus : Qt.NoFocus
                enabled: editable

                stepSize: 10
                from: 0
                to: 1000000

                property int decimals: 6
                property real realValue: value / to

                validator: DoubleValidator {
                    bottom: Math.min(ratesb.from, ratesb.to)
                    top:  Math.max(ratesb.from, ratesb.to)
                }

                textFromValue: function(value, locale) {
                    return Number(value / to).toLocaleString(locale, 'f', ratesb.decimals)
                }

                valueFromText: function(text, locale) {
                    return Number.fromLocaleString(locale, text) * to
                }

                value: 10
            }

            //btc input
            SpinBox {
                id: btcbox

                Layout.row: 3
                Layout.column: swappane.btcsell ? 1 : 4
//                Layout.columnSpan: 3
//                Layout.alignment: Qt.AlignCenter

                editable: swappane.btcsell
                wheelEnabled: editable
                focus: editable
                focusPolicy: editable ? Qt.StrongFocus : Qt.NoFocus
                enabled: editable

                stepSize: 10000
                from: 0
                to: (editable ? swappane.btcbal : swappane.fbbal * swappane.swaprate) * 100000000;
                property int decimals: 8
                property real realValue: value * .00000001
                validator: DoubleValidator {
                    bottom: Math.min(btcbox.from, btcbox.to)
                    top:  Math.max(btcbox.from, btcbox.to)
                }
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

                onValueChanged: {
//                    console.log("btcbox new value " + value)
//                    if ( editable ) {
//                        console.log("btcbox new value2 " + (value * .00000001) / swappane.swaprate)
//                        console.log("try set" + Math.round( (value * .00000001) / swappane.swaprate ))
//                        fbbox.value = Math.round((value * .00000001) / swappane.swaprate )
//                    }
                }
                value: editable ? 0 : (fbbox.value * swappane.satoshirate)
           }

            Button {
                Layout.row: 4
                Layout.column: 3
                Layout.alignment: Qt.AlignCenter

                id: button
                text: "Swap"
                onClicked: {
                    mySwapDialog.show()
                }
            }
        }

    }

    Material.Dialog {
        id: mySwapDialog

        height: ProtoScreen.guToPx(34)
        property int count: 0
        minimumHeight: ProtoScreen.guToPx(8)
        width: ProtoScreen.guToPx(77)
        minimumWidth: ProtoScreen.guToPx(16)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        property string side: "none"
        property int price
        property int qty
        property string player
        positiveButtonText: "Swap It"
        title: "Confirm Swap Order - " + (true ? ("Send ƑɃ - Receive BTC") : ("Send BTC - Receive FB"))
        text: "Protoblock Wallet: " + realRoot.uname
        dialogContent: Column {
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: ProtoScreen.guToPx(.5)

            Text{
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                text:  "xxx"
                // myTradeDialog.side + " " + myTradeDialog.qty.toString() +
                // " contract(s) at price " + myTradeDialog.price.toString()
                font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            }

            Text{
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                text:  "of " + myTradeDialog.player
                font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            }

        }

        onAccepted: {
//            if ( realRoot.uname === "" ) {
//                pageHelper.selectedTabIndex = themeroot.accountIndex;
//            }
//            else if ( MiddleMan.pMyFantasyNameBalance.stake <= 0 ) {
//                console.log("Error: Zero Balace - must have balance to trade")
//                title = "Error: Zero Balace - must have balance to trade";
//                if ( count === 0) {
//                    count = 1
//                    show()
//                }
//            }

//            else if ( inplay.symbol !== "" ) {
//                console.log(MiddleMan.pMyFantasyNameBalance.stake +  " Balace - must have balance to trade")

//                MiddleMan.doTrade(
//                     inplay.playerid,
//                    inplay.symbol
//                    ,(side == "Buy") ? true : false
//                 ,myTradeDialog.price
//                 ,myTradeDialog.qty
//                    )
//            }
            MiddleMan.doSwap();
        }

    }
}

//}

/*
        ListItems.Subtitled {
            Layout.row: 3
            Layout.column: !swappane.btcsell ? 1 : 5
            Layout.columnSpan: 3
            radius: 10

//            border.color: "#2580a6"
//            radius: 10
            elevation: 1
            Layout.fillWidth: false
            Layout.preferredWidth: ProtoScreen.guToPx(20)
//            text: "Text"
//            valueText: "valueText"
//            iconName: "clone";

            secondaryItem: SpinBox {
                focusPolicy: Qt.StrongFocus
                wheelEnabled: true
                id: bbox2
                stepSize: 1
                from: 0
                to: fbabale
                value: 0
                property int decimals: 8
                property real realValue: value * .00000001
                validator: DoubleValidator {
                    bottom: Math.min(bbox2.from, bbox2.to)
                    top:  Math.max(bbox2.from, bbox2.to)
                }
                textFromValue: function(value, locale) {
                    console.log( " from value" + value)
                    return Number(value * 00000001 ).toLocaleString(Qt.locale("en-US"), 'f', bbox2.decimals) + " BTC"
                }

                valueFromText: function(text, locale) {
                    console.log( " from text " + text)
                    var t = text.split(" ")[0]
                    var d = Number.fromLocaleString(Qt.locale("en-US"), t)
                    var dd = d * 100000000
                    console.log(d / 0.00000001)
                    console.log(dd)
                    return Math.round(dd * 100000000) / 100000000
                }

                font.pixelSize: 10
                editable: !swappane.btcsell
                onValueChanged: {
                    console.log("new value " + value)
                }
//                Layout.alignment: Qt.AlignCenter
//                Layout.columnSpan: 1
//                Layout.rowSpan: 2
//                Layout.column: 6
//                Layout.row: 1
//                Layout.preferredHeight: 20
//                Layout.preferredWidth: image12.implicitWidth * 2
            }

            content: Button {
                    id: button
                    text: qsTr("SELL")
            }
        }
/*
//        Column {
//            anchors.fill: parent
//            width: parent.width


  /*
        SpinBox {
            focusPolicy: Qt.StrongFocus
            wheelEnabled: true
            focus: true
            id: box2
            stepSize: 1000
            from: 1000
            to: CoinSale.totalAvailable
            font.pixelSize: 12
            editable: true
            Layout.preferredHeight: 20
            Layout.preferredWidth: image12.implicitWidth * 2
            textFromValue: function(value, locale) {
                return Number(value).toLocaleString(locale, 'f', 0) + " ƑɃ";
            }

            valueFromText: function(text, locale) {
                console.log( "fb from text " + text)
                var t = text.split(" ")[0]
                var d = Number.fromLocaleString(Qt.locale("en-US"), t)
                return d;
            }

            font.family: fontfamFB


           value: bbox2.value / 2
           Layout.alignment: Qt.AlignCenter
            Layout.columnSpan: 1
            Layout.rowSpan: 2
            Layout.column: 5
            Layout.row: 1
        }
*/
        /*
            ListItems.Subheader {
                text: "Section Subheader"
            }

            ListItems.Standard {
                text: "Standard list item"
            }

            ListItems.Subtitled {
                text: "Subtitled list item"
                subText: "With some subtext!"
            }

            ListItems.Subtitled {
                text: "Subtitled list item"
                subText: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer nec eleifend arcu, eu convallis nisi."
                valueText: "2h ago"

                maximumLineCount: 3
            }

            ListItems.Subtitled {
                text: "Subtitled list item"
                subText: "With some subtext, icon, and secondary item!"
                secondaryItem: Switch {
                    id: enablingSwitch
                    anchors.verticalCenter: parent.verticalCenter
                }

                onClicked: enablingSwitch.checked = !enablingSwitch.checked

                action:Image{
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/btc.png"
                }


//                action: Material.Icon {
//                    anchors.centerIn: parent
//                    name: "icon://awesome/clone"
//                    size: ProtoScreen.guToPx(3)

////                    size: dp(32)
//                }
            }

            ListItems.SimpleMenu {
                text: "Subtitled list item"
                model: ["A", "B and some long text that should not clip", "C"]
            }
*/
            //        }




//        QRCode {
//            id: qrc
////            anchors.centerIn: parent
////            anchors.fill: parent
////            anchors.horizontalCenter: parent.horizontalCenter
//            value: btcadddr
//            level: "H"
//        }

//    }

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/

//ListItems.Subtitled {
//    border.color: "#FF9900"
//    radius: 10
//    elevation: 0
//    Layout.fillWidth: false
//    Layout.preferredWidth: ProtoScreen.guToPx(50)
//    text: MiddleMan.pMyFantasyNameBalance.name
//    valueText: "1BPw53Gf5faxAwSmXrCqHDWkhM1xK2jUX9";//btcadddr
//    subText: MiddleMan.toBTCbalance(valueText);

//    action:Image{
//        height: parent.height
//        fillMode: Image.PreserveAspectFit
//        source: "qrc:/btc.png"
//    }
//    itemLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
//    itemValueLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
//    itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
//    //            itemValueLabel.font.family: fontfamFB
//}
