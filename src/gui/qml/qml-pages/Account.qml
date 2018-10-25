import QtQuick 2.4
import QtQuick.Controls 2.3

import QtQuick.Controls 1.4 as C12

//import ProRotoQml.Protoblock 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItems

import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1

import QtQuick.Controls 2.3 as Mat2



Item {
    Component.onCompleted: { pageHelper.title = "Account Settings"; }
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
                    onClicked: MiddleMan.useName(model.name)
                    itemValueLabel.font.family: fontfamFB

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
//                    onAccepted: nameCheckBlank(nameText.text)
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

        Card {
            id: othernamePicker
            width: parent.width / 1.07
            height: (othernamePickerBannner.height + 10) + playasgrid.height + ProtoScreen.guToPx(4)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: newNameCard.bottom
            anchors.topMargin: ProtoScreen.guToPx(4)
            Banner {
                id: othernamePickerBannner
                width: parent.width
                height: ProtoScreen.guToPx(5)
                text: "View-Only Name"
                backgroundColor: themeroot.theme.primaryColor
                helpShown: true
                helperHeader: "View-Only help"
                helperTxt: "Choose any account to use. This will be view-only"
            }

            GridLayout {
                anchors.top: othernamePickerBannner.bottom
                anchors.topMargin: ProtoScreen.guToPx(1)
                id: playasgrid
                width: othernamePicker.width//ProtoScreen.guToPx(80)
                height: ProtoScreen.guToPx(12)
                columns: 8
                rows: 2
                Label {
                    text: "View As:"
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

                TextField {
                    id: playasname
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
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    Layout.preferredWidth: ProtoScreen.guToPx(50)
                    textColor: status === 0 ? Colors.black : status === 1 ? Colors.green : Colors.red
                    onTextChanged: {
                        console.log("playasname onTextChanged " +  textColor)
                        if ( focus && !setting ) {
                            MiddleMan.pAccountsModel.filterString = text;
                            MiddleMan.pAccountsModel.invalidate()
                            if ( !comboplay.popup.visible && text !== "")
                                comboplay.popup.open();

                            status = 0
                        }
                    }

                    onAccepted: {
                        accepting = true;
                        console.log("playasname onAccepted")
                        if ( text !== comboplay.currentText ) {
                            var idx = comboplay.find(text, Qt.MatchFixedString)
                            if (idx > -1) {
                                setting = true;
                                text = comboplay.textAt(idx)
                                setting = false;
                                status = 1
                            }
                            else {
                                status = -1
                            }
                        }
                        else {
                            if ( comboplay.currentIndex !== -1)
                                status = 1
                            else
                                status = -1
                        }

                        if ( comboplay.popup.visible )
                            comboplay.popup.close();

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
                    anchors.right: playasname.right
                    flat: true
                    inputMethodHints: Qt.ImhNoPredictiveText
                    id: comboplay
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

                    onActivated: {
                        console.debug("combo onActivated currentindex " + currentIndex + " currentText " + currentText + " edit " + editText + " displayText " + displayText)

                        if ( currentIndex !== -1) {
                            playasname.setting = true
                            playasname.text = currentText
                            confbut.forceActiveFocus()
                            MiddleMan.pAccountsModel.filterString = currentText;
                            MiddleMan.pAccountsModel.invalidate()
                            playasname.status = 1
                            currentIndex = -1
                            playasname.setting = false
                        }
                    }

                    onCurrentTextChanged: {
                        displayText = ""
                    }

                    onAccepted: {
                        console.debug("onAccepted currentindex " + currentIndex + " currentText " + currentText + " edit " + editText + " displayText " + displayText)
                    }

                }

                Button {
                    enabled: MiddleMan.liveSync === "Live" && playasname.status === 1
                    elevation: 2
                    id: confbut
                    text: "Confirm"
                    Layout.column: 3
                    Layout.row: 2
                    Layout.columnSpan: 2
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    onClicked : {
                        console.log ( " onClicked " + playasname.status);
                        if ( playasname.status === 0) {
                            console.log ( " onClicked black");
                            playasname.accepted()
                        }

                        if ( playasname.status === -1) {
                            console.log ( " onClicked red");
                        }
                        else {
                            MiddleMan.useName(playasname.text)
                        }
                    }
                    Layout.preferredHeight: ProtoScreen.guToPx(3)
                    Layout.preferredWidth: ProtoScreen.guToPx(12)

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
