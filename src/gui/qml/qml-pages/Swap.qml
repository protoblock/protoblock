import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import Qt.labs.settings 1.0

import Material.ListItems 1.0 as ListItems

import ProRotoQml.Theme 1.0
import Material 1.0 as Material


Item {
Pane {
    id: swappane
    property string btcadddr: MiddleMan.toBTCAddess(MiddleMan.pMyFantasyNameBalance.pk);
    property string btcbal: MiddleMan.toBTCbalance(swappane.btcadddr).toLocaleString();
    property bool btcsell: true
    property real swaprate: .00001
    property int satoshirate: (swaprate * 100000000.0)

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
            columns: 7
//            width: parent.
//            Layout.alignment: Qt.AlignCenter
//            layoutDirection: swappane.btcsell ? Qt.LeftToRight : Qt.RightToLeft

            Label {
                text: "SEND"
                Layout.column: 1
                Layout.columnSpan: 3
                Layout.row: 1
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                font.pixelSize: (ProtoScreen.font(ProtoScreen.NORMAL))
            }

            Label {
                text: "RECEIVE"
                Layout.column: 5
                Layout.columnSpan: 3
                Layout.row: 1
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                font.pixelSize: (ProtoScreen.font(ProtoScreen.NORMAL))
            }


            ListItems.Subtitled {
                id: ibtc
                Layout.row: 2
                Layout.column: swappane.btcsell ? 1 : 5
                Layout.columnSpan: 3
                border.color: "#FF9900"
                radius: 10
                elevation: 0
                Layout.fillWidth: true
                Layout.preferredWidth: ProtoScreen.guToPx(40)
                text: "Bitcoin"
                subText: (!swappane.btcsell ? //("1 ƑɃ = " + swappane.satoshirate.toLocaleString()  + " SAT")
                                              ("1 ƑɃ = " +  Number(swappane.swaprate.toLocaleString(Qt.locale("en-US"), 'f', 8)) + " BTC")
                                            : swappane.btcbal  + " BTC")
                        ;

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
                Layout.column: 4
                Layout.columnSpan: 1

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
                Layout.column: !swappane.btcsell ? 1 : 5
                Layout.columnSpan: 3
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
        }

        ListItems.Standard {
            border.color: "#2580a6"
            radius: 10
            elevation: 0
            Layout.fillWidth: true
            Layout.preferredWidth: ProtoScreen.guToPx(40)
            text: "Text"
            valueText: "valueText"
            iconName: "clone";

            content: Button {
                    id: button
                    text: qsTr("content")
                }

//            action: Button {
//                    text: qsTr("action")
//                }
        }

//        Column {
//            anchors.fill: parent
//            width: parent.width
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
//        }

    }

//        RowLayout {
//            Button {
//                id: button
//                text: qsTr("Deposit")
//            }

//            Button {
//                id: button1
//                text: qsTr("Withdraw")
//            }
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
}

//Component {
//    id: isbtc
//}

}

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
