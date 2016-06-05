import QtQuick 2.4

import ProRotoQml.Protoblock 1.0
import Material 1.0
import Material.ListItems 1.0 as ListItems

Card {

    Component.onCompleted: {
        pageHelper.title = "Import-Export"
        secretTxt.text = ""
    }
    height: parent.height / 1.07
    width: parent.width / 1.07
    elevation: 5
//    anchors.top: parent.top
//    anchors.topMargin:  parent.height / 22

    property string  errmsg

    Column{
        anchors.fill: parent
        spacing: 10

        Label {
            id: welcomeTxt
//            anchors.top: parent.top
//            anchors.topMargin:  parent.height / 22
//            width: parent.width / 1.07
            font.pixelSize: Qt.platform.os === "android" ? 32 : 22
            font.family: "Roboto"
            horizontalAlignment: Text.HorizontalFit
            text: "Protoblock account names are not stored on a central server... blah" +
                  "Import from another device. " +
                  "Export or backup"

            wrapMode: Text.WordWrap
        }

        ListItems.Subtitled{
            elevation: 1
            width: parent.width / 1.07
            text: "FantasyName: " + root.uname
//                        subText: "Status of name " +  unameStatus
            action: RoundImage{
                height: parent.height
                width : height
                fillMode: Image.PreserveAspectFit
                source: "qrc:/icons/action_account_circle.png"
            }
            valueText: "Balance : " + " 0"
        }

        TextField {
            id: nameText
            width: parent.width / 1.07
            font.family: "Default"
            placeholderText: "please enter in 12 secret words"
            anchors.horizontalCenter: parent.horizontalCenter
//            onTextChanged: {
//                root.uname = text
//            }
        }

        Button{
            property string mypk
            id: importButton
            text: "Import From Secret Backup"
//            width: parent.width / 1.07
            elevation: 5
//            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                mypk = MiddleMan.importMnemonic(nameText.text)
                if ( mypk === "" )
                    errmsg = "Error: Import failed, please try again"
                else
                    errmsg = "Trying to import with key: " + mypk
//                if (text === "Claim New Name" ){
//                    MiddleMan.checkname(nameText.text)
//                    //checkNameTimmer.start()
//                }else{
//                    MiddleMan.importMnemonic(nameText.text);
//                }
//                myImportDialog.show()
                nameText.text = ""
                statusTxt.text = errmsg;
                secretTxt.text = ""
            }
        }

        Label {
            id: statusTxt
//            anchors.top: parent.top
//            anchors.topMargin:  parent.height / 22
//            width: parent.width / 1.07
            font.pixelSize: Qt.platform.os === "android" ? 32 : 22
            font.family: "Roboto"
            horizontalAlignment: Text.HorizontalFit
            text: ""

            wrapMode: Text.WordWrap
        }



        Button{
            property string mypk
            id: exportButton
            text: "Export Secret 12 Word for Exprt or Backup"
//            width: parent.width / 1.07
            elevation: 5
//            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
//                mypk = MiddleMan.importMnemonic(nameText.text)
//                if ( mypk === "" )
//                    errmsg = "Error: Import failed, please try again"
//                else
//                    errmsg = "Trying to import with key: " + mypk
//                if (text === "Claim New Name" ){
//                    MiddleMan.checkname(nameText.text)
//                    //checkNameTimmer.start()
//                }else{
//                    MiddleMan.importMnemonic(nameText.text);
//                }
                secretTxt.text = ""
                mySecretDialog.show()
//                statusTxt.text = errmsg;
            }
        }

        Label {
            id: secretTxt
//            anchors.top: parent.top
//            anchors.topMargin:  parent.height / 22
//            width: parent.width / 1.07
            font.pixelSize: Qt.platform.os === "android" ? 32 : 22
            font.family: "Roboto"
            horizontalAlignment: Text.HorizontalFit
            text: ""

            wrapMode: Text.WordWrap
        }

        Button{
            property string mypk
            id: clearSecret
            text: "Clear secret"
//            width: parent.width / 1.07
            elevation: 5
//            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                secretTxt.text = ""
            }
        }

        Dialog {
            id: mySecretDialog
            title: "12 word account Secret"
            positiveButtonText: "show secret"
            negativeButtonText: "get me out of here!"
            Text{
                width: parent.width
                height: Unit.dp(160)
                wrapMode: Text.WordWrap
                text:  "Please make sure nobody is behind you. Secret to your account: " +root.uname + ", will be displayed!"
            }
            onAccepted: {
                secretTxt.text = "click Clear when done. 12 word Secret for " + root.uname + ": " +
                        MiddleMan.getSecret()
                //"essence scatter shrimp holt try butler reed mushroom mix item mirror draft"
            }
        }
//        Dialog {
//            id: myImportDialog
//            title: "Import status"
//            positiveButtonText: "back"
//            Text{
//                width: parent.width
//                height: Unit.dp(160)
//                wrapMode: Text.WordWrap
//                text:  errmsg
//            }
//        }

    }
}

