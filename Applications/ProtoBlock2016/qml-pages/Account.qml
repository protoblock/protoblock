import QtQuick 2.4

import ProRotoQml.Protoblock 1.0
import Material 1.0
import Material.ListItems 1.0 as ListItems

Item {
    Component.onCompleted: pageHelper.title = "Accounts Settings"
    Flickable{
        interactive: true
        width: parent.width
        height: parent.height
        contentHeight: accountInfoTxt.height + (32* 3.2) +(namePicker.height + newNameCard.height)
        contentWidth: parent.width
        boundsBehavior:  Flickable.StopAtBounds
        Card {
            id: accountInfoTxt
            height:welcomeTxt.paintedHeight + 20
            width: parent.width / 1.07
            elevation: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 32
            anchors.top: parent.top
            backgroundColor: root.theme.primaryColor
            Label {
                id: welcomeTxt
                width: parent.width / 1.07
                font.pixelSize: Qt.platform.os === "android" ? 32 : 22
                font.family: "Roboto"
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: "Protoblock players must claim a name to create an account" +
                      "Chose your Twitter handle or fantasy team name. " +
                      "this is your fantasy identity and we will be used for display on the projection leaderboard"
                wrapMode: Text.WordWrap
            }
        }



        Card{
            id: namePicker
            width: parent.width / 1.07
            height: (72 * repeater.count) + (namePickerBannner.height + 10)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: accountInfoTxt.bottom
            anchors.topMargin: 32
            opacity: repeater.opacity
            Banner{
                id: namePickerBannner
                width: namePicker.width
                height: 48
                text: "Choose Name"
                opacity: repeater.opacity
            }
            ListView {
                id: repeater
                width: namePicker.width
                height: namePicker.height - 48
                clip: true
                interactive: true
                onCountChanged: count < 1 ? opacity = 0 : opacity = 1
                anchors.top: namePickerBannner.bottom
                model: MiddleMan.goodList().length
                delegate:
                    ListItems.Subtitled{

                    elevation: 2
                    width: parent.width
                    text: "FantasyName: " +  MiddleMan.goodList()[index]
                    action: RoundImage{
                        height: parent.height
                        width : height
                        fillMode: Image.PreserveAspectFit
                        opacity: MiddleMan.goodList()[index] === uname ? 1 : 0
                        source:  "qrc:/icons/action_account_circle.png"
                        Behavior on opacity {NumberAnimation{duration: 80}}
                    }
                    valueText: "Balance : " + " 0"
                    onClicked: MiddleMan.useName(MiddleMan.goodList()[index])
                }
            }
        }


        Card{
            id: newNameCard
            width: parent.width / 1.07
            height: nameText.height + (clamNameButton.height + 32)  + 64
            elevation: 1
            anchors.top: namePicker.bottom
            anchors.topMargin: 32
            anchors.horizontalCenter: parent.horizontalCenter
            Column{
                // add a banner
                anchors.fill: parent
                spacing: 16
                Banner{
                    width: parent.width
                    height: 48
                    text: "Claim New Name"
                }

                TextField {
                    id: nameText
                    width: parent.width / 1.07
                    font.family: "Default"
                    placeholderText: "please enter in a new fantasy name"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onAccepted: nameCheckBlank(nameText.text)
                }
                Button{
                    id: clamNameButton
                    text: "Claim New Name"
                    width: parent.width / 1.07
                    elevation: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: nameCheckBlank(nameText.text)
                }
            }
        }

        Dialog {
            id: accountErrorDialog
            title: "Error in Signup"
            positiveButtonText: "back"
            negativeButtonText: "import"
            //            onAccepted: loginCardScale = 1
            //            onRejected:  loginCardScale = 1
            Text{
                width: parent.width
                height: Unit.dp(160)
                wrapMode: Text.WordWrap
                text:  errorString
            }
            onRejected: {
                rootLoader.source = "qrc:/Import-Export.qml"
            }
        }

        // FIXME why is there 2 connection points ?
        // this could be the cause of the bugs in startup
        Connections {
            target: MiddleMan
            onNameCheckGet: {
                if(status === "true" ) {
                    MiddleMan.signPlayer(name)
                    root.reloadhome = false
                    rootLoader.source = "qrc:/Projections.qml"
                    pageHelper.selectedTabIndex = 1;
                }
                else {
                    errorString = name + " is taken. please try with a different name. If this your name from another device, please click import or contact Protoblock for help"
                    accountErrorDialog.open()
                }
            }
        }
    }

    function nameCheckBlank(s) {
        if ( s.length === 0 ) {
            return
        }
        else if ( s.length > 45) {
            errString = "name to long"
            accountErrorDialog.show()
        }

        else {
            MiddleMan.checkname(s)
        }
    }
}
