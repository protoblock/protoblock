import QtQuick 2.4
import QtQuick.Controls 1.4 as Controls
import ProRotoQml.Protoblock 1.0
import Material 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
import QtQuick.Controls 1.4 as Controls

Item{
    Component.onCompleted: {
        pageHelper.title = "Import-Export"
        secretTxt.text = ""
    }
    Flickable{
        width: parent.width
        height: parent.height
        contentHeight: (parent.height + bacard2.height) * 1.2
        contentWidth: parent.width
        boundsBehavior:  Flickable.StopAtBounds
        interactive: true
        Column{
            width: parent.width / 1.07
            anchors{
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: ProtoScreen.guToPx(2)
                bottom: parent.bottom
            }
            spacing: ProtoScreen.guToPx(1.25)
            Label {
                id: welcomeTxt
                width: parent.width
                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                horizontalAlignment: Text.AlignHCenter
                text: "Your Protoblock name is your identity and is managed by your device. There is no central server; instead, your device stores a secret 12-word phrase for back-up and recovery. This means that you and only you control your password. This also means that if you lose your 12-word password, you will not be able to log back in and could possibly lose your saved data.  Please never share this with anyone. Use the export option to see your 12-word secret or use the import tab to import one that you may have on a different device."
                wrapMode: Text.WordWrap
            }

            ListItems.Subtitled{
                id: thelist
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



            Card{
                id: bcard
                height: nameText.height + importButton.height + imBan.height + ProtoScreen.guToPx(6)
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                elevation: 5
                Banner{
                    id: imBan
                    text: "Import"
                    backgroundColor: themeroot.theme.primaryColor
                    helpShown: true
                }
                Column{
                    width: parent.width
                    height: parent.height - imBan.height
                    anchors.top: imBan.bottom
                    anchors.topMargin: ProtoScreen.guToPx(1)
                    spacing: ProtoScreen.guToPx(4)

                  TextField {
                        id: nameText
                        width: parent.width / 1.07
                        helperText: "Please enter in your 12 word secret"
                        anchors.horizontalCenter: parent.horizontalCenter
                        inputMethodHints: Qt.ImhNoPredictiveText;
                    }

                    Button{
                        id: importButton
                        text: "IMPORT"
                        width: parent.width / 1.07
                        elevation: 1
                        backgroundColor: Colors.blue
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {
                            var mypk = MiddleMan.importMnemonic(nameText.text)
                            if ( mypk === "" ) {
                                importExportStatus = "Error: Import failed, please try again"
                            } else {
                                importExportStatus = "Trying to import with key: " + mypk
                            }
                            myImportDialog.show()
                            nameText.text = ""
                            statusTxt.text = importExportStatus;
                            secretTxt.text = ""
                        }
                    }
                }
            }

            Card{
                id: bacard2
                height:(( imBan2.height + exportButton.height) * 2 ) + (clearSecret.height + secretTxt.paintedHeight)
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                elevation: 5
                Behavior on height{NumberAnimation{duration: 1200;easing.type: Easing.OutQuad}}
                Column{
                    width: parent.width
                    height: parent.height
                    spacing: ProtoScreen.guToPx(2)
                    Banner{
                        id: imBan2
                        text: "Backup / Export Secret"
                        backgroundColor: Colors.blue
                    }
                    Button{
                        property string mypk
                        id: exportButton
                        text: "Export"
                        width: parent.width / 1.07
                        elevation: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {
//                            console.log("Count of secert "+  secretTxt.contentWidth )
                            secretTxt.text = ""
                            mySecretDialog.show()
                        }
                    }
                    Button{
                        property string mypk
                        id: clearSecret
                        elevation: 2
                        visible: secretTxt.contentWidth >= 1 ? true : false
                        opacity: visible ? 1: 0
                        text: "Clear Secret"
                        backgroundColor: themeroot.theme.primaryColor
                        width: parent.width / 1.07
                        anchors.horizontalCenter: parent.horizontalCenter
                        Behavior on opacity {NumberAnimation{duration: 1000;}}
                        onClicked: {
                            if(secretTxt.contentWidth >= 1 ){
                                secretTxt.text = ""
                            }
                        }
                    }
                    Label {
                        id: secretTxt
                        width: parent.width / 1.07
                        font.pixelSize: ProtoScreen.guToPx(3)
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                        opacity: contentWidth > 0 ? 1 : 0
                        Behavior on visible {NumberAnimation{duration: 1200;}}
                    }
                }
            }
        }




        Dialog {
            id: mySecretDialog
            title: "12 word account secret"
            positiveButtonText: "Show"
            negativeButtonText: "Cancel"
            Label{
                width: parent.width / 1.07
                wrapMode: Text.WordWrap
                text:  "Secret to your account: " +realRoot.uname + ", will be displayed!"
            }
            onAccepted: {
                secretTxt.text = "12-word Secret for " + realRoot.uname + " Please do not share this with anyone: \n\n" +
                        MiddleMan.getSecret()
            }
        }
    }
}
