import QtQuick 2.4

import ProRotoQml.Protoblock 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItems

import ProRotoQml.Theme 1.0

Item {
    Component.onCompleted: pageHelper.title = "Account Settings"
    Flickable{
        interactive: true
        width: parent.width
        height: parent.height
        contentHeight: accountInfoTxt.height + ( ProtoScreen.guToPx(4) * 3.2) +(namePicker.height + newNameCard.height) * (repeater.count <= 1 ? 2 : repeater.count )
        contentWidth: parent.width
        boundsBehavior:  Flickable.StopAtBounds

        Card {
            id: accountInfoTxt
            height:welcomeTxt.paintedHeight + ProtoScreen.guToPx(3)
            width: parent.width / 1.07
            elevation: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: ProtoScreen.guToPx(4)
            anchors.top: parent.top
            Label {
                id: welcomeTxt
                width: parent.width / 1.07
                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                font.family: "Roboto"
                horizontalAlignment: Text.AlignHCenter
                text: "A Protoblock Player name is your fantasy identity, and will be displayed on the projections leaderboard. " +
                      "Claim your name! Chose your Twitter handle or fantasy team name.";
                wrapMode: Text.WordWrap
            }
        }



        Card{
            id: namePicker
            width: parent.width / 1.07
            height: (ProtoScreen.guToPx(8) * repeater.count) + (namePickerBannner.height + 10) * repeater.opacity
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: accountInfoTxt.bottom
            anchors.topMargin: ProtoScreen.guToPx(4)
            opacity: repeater.opacity
            Banner{
                id: namePickerBannner
                width: namePicker.width
                height: ProtoScreen.guToPx(6)
                text: "Choose Name"
                opacity: repeater.opacity
                backgroundColor: themeroot.theme.primaryColor
                helpShown: true
                helperHeader: "Username help"
                helperTxt: "Choose the active Protoblock account.   Select a name from the list to change users."
            }
            ListView {
                id: repeater
                width: namePicker.width
                height: namePicker.height - ProtoScreen.guToPx(6)
                clip: true
                interactive: true
                onCountChanged: {
                    count < 1 ? opacity = 0 : opacity = 1
                }
                anchors.top: namePickerBannner.bottom
                model: MiddleMan.pGoodNameBalModel//MiddleMan.goodList().length
                delegate:
                    ListItems.Subtitled{
                    itemSubLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                    elevation: 2
                    width: parent.width
//                    height: ProtoScreen.guToPx(11)
                    text: model.name //+ " pk(" + model.pk + ")" // MiddleMan.goodList()[index]
                    subText: model.pk

                    action: Image{
                        height: parent.height
                        width : height
                        fillMode: Image.PreserveAspectFit
                        opacity: model.name === uname ? 1 : 0
                        source:  "qrc:/icons/action_account_circle.png"
                        Behavior on opacity {NumberAnimation{duration: 80}}
                    }
                    valueText: "Skill(" + model.bits + " ƑɃ) Stake(" + model.stake +" ƑɃ)"
                    onClicked: MiddleMan.useName(MiddleMan.goodList()[index])
                }
            }
        }


        Card{
            id: newNameCard
            width: parent.width / 1.07
            height: nameText.height + (clamNameButton.height + ProtoScreen.guToPx(6))  + ProtoScreen.guToPx(8)
            elevation: 5
            anchors{
                top: namePicker.bottom
                topMargin: ProtoScreen.guToPx(8)
                horizontalCenter: parent.horizontalCenter
            }
            Banner{
                id: claimBanner
                width: parent.width
                height: ProtoScreen.guToPx(5)
                text: "Claim New Name"
                backgroundColor: themeroot.theme.primaryColor
                helpShown: true
                helperHeader: "Claiming a new username"
                helperTxt: "This is your fantasy identity. Choose a name, enter it in, then click \"Cliam New Name\". You will then be playing with your new name."
            }
           Column{
                height:  parent.height - claimBanner.height
                width: parent.width
                anchors.top: claimBanner.bottom
                anchors.topMargin: ProtoScreen.guToPx(1)
                spacing: ProtoScreen.guToPx(4)
                TextField {
                    id: nameText
                    width: parent.width / 1.07
                    font.pixelSize: ProtoScreen.font(ProtoScreen.MEDIUM)
                    font.family: "Default"
                    helperText: "please enter in a new fantasy name"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onAccepted: nameCheckBlank(nameText.text)
                    inputMethodHints: Qt.ImhNoPredictiveText;
                }

                Button{
                    id: clamNameButton
                    text: "Claim New Name"
                    width: parent.width / 1.07
                    elevation: 2
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: nameCheckBlank(nameText.text)
                    backgroundColor: themeroot.theme.primaryColor
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
