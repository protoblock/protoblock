import QtQuick 2.4

import ProRotoQml.Protoblock 1.0
import Material 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0

Item{
    Card {
        Component.onCompleted: {
            pageHelper.title = "Import-Export"
            secretTxt.text = ""
        }
        height: parent.height / 1.07
        width: parent.width / 1.07
        elevation: 5
        anchors.centerIn: parent

        Column{
            anchors.fill: parent
            spacing: ProtoScreen.guToPx(1.25)

            Label {
                id: welcomeTxt
                width: parent.width / 1.07
                font.pixelSize: Qt.platform.os === "android" ? 32 : 22
                font.family: "Roboto"
                horizontalAlignment: Text.AlignHCenter
                text: "Protoblock account names are not stored on a central server... blah" +
                      "Import from another device. " +
                      "Export or backup"

                wrapMode: Text.WordWrap
            }

            ListItems.Subtitled{
                elevation: 1
                width: parent.width / 1.07
                text: "FantasyName: " + realRoot.uname
                action:Image{
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
                placeholderText: "Please enter in you last years 12 secret words"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button{
                id: importButton
                text: "Import From Secret Backup"
                width: parent.width / 1.07
                elevation: 5
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    mypk = MiddleMan.importMnemonic(nameText.text)
                    if ( mypk === "" )
                        importExportStatus = "Error: Import failed, please try again"
                    else
                    importExportStatus = "Trying to import with key: " + mypk
                    myImportDialog.show()
                    nameText.text = ""
                    statusTxt.text = importExportStatus;
                    secretTxt.text = ""
                }
            }

            Label {
                id: statusTxt
                width: parent.width / 1.07
                font.pixelSize: Qt.platform.os === "android" ? 32 : 22
                font.family: "Roboto"
                horizontalAlignment: Text.AlignHCenter
                text: ""

                wrapMode: Text.WordWrap
            }



            Button{
                property string mypk
                id: exportButton
                text: "Export Secret 12 Word for Exprt or Backup"
                width: parent.width / 1.07
                elevation: 5
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    secretTxt.text = ""
                    mySecretDialog.show()
                }
            }

            Label {
                id: secretTxt
                width: parent.width / 1.07
                font.pixelSize: Qt.platform.os === "android" ? 32 : 22
                font.family: "Roboto"
                horizontalAlignment: Text.AlignHCenter
                text: ""
                wrapMode: Text.WordWrap
            }
            Button{
                property string mypk
                id: clearSecret
                text: "Clear secret"
                width: parent.width / 1.07
                elevation: 5
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    secretTxt.text = ""
                }
            }
        }
    }

    Dialog {
        id: mySecretDialog
        title: "12 word account Secret"
        positiveButtonText: "show secret"
        negativeButtonText: "get me out of here!"
        Label{
            width: parent.width / 1.07
            wrapMode: Text.WordWrap
            text:  "Please make sure nobody is behind you. Secret to your account: " +realRoot.uname + ", will be displayed!"
        }
        onAccepted: {
            secretTxt.text = "12 word Secret for " + realRoot.uname + ": \n\n" +
            MiddleMan.getSecret()
        }
    }

}
