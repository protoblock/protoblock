import QtQuick 2.4
import QtQuick.Controls 1.4 as Controls

import Material 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.0


Item{

    TextEdit {
        id: cliphelper
        visible: false
    }

    property real maxwidt: Math.min(ProtoScreen.guToPx(100),parent.width)

    Component.onCompleted: {
        pageHelper.title = "Import-Export"
        secretTxt.text = ""
        console.log ( "Import-Export on coplete")
    }

    Component.onDestruction: {
        console.log ( "Import-Export on destroy")
        cliphelper.text = realRoot.uname
        cliphelper.selectAll()
        cliphelper.copy()
        secretTxt.text = ""
    }

    Flickable{
        width: maxwidt
        height: parent.height
        contentHeight: parent.height * 2 //(parent.height + bacard2.height + bacard) * 1.2
        contentWidth: maxwidt
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
                width: parent.width / 1.07
                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                horizontalAlignment: Text.AlignHCenter
                text: "Your Protoblock name is your identity. It is managed by your device, there is no central server. A secret 12-word pass phrase is used for back-up and recovery. Only you control your password. If you lose your device and your 12-word password, access to your account will be lost. Use the export option to see your 12-word secret."
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
                itemLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                itemValueLabel.font.pixelSize: (ProtoScreen.font(ProtoScreen.SMALL))
                valueText: "Balance : " + (!MiddleMan.pMyFantasyNameBalance ? "0" : (MiddleMan.pMyFantasyNameBalance.stake).toLocaleString()) + " ƑɃ"
                itemValueLabel.font.family: fontfamFB
            }


            Card{
                id: bacard2
                height: imBan2.height + exportButton.height + ProtoScreen.guToPx(6)
                width: thelist.width
//                anchors.horizontalCenter: parent.horizontalCenter
                elevation: 1
                Behavior on height{NumberAnimation{duration: 1200;easing.type: Easing.OutQuad}}
                Column{
                    width: parent.width
                    height: parent.height
                    spacing: ProtoScreen.guToPx(2)
                    Banner{
                        id: imBan2
                        text: "Backup / Export Secret"
                        backgroundColor: themeroot.theme.primaryColor
                        helpShown: true
                        helperHeader: "Backup and export help"
                        helperTxt: "Use this to export your 12 word password.  This is so that you can also sign in on other devices with the import option "
                    }
                    Button{
                        id: exportButton
                        text: "EXPORT"
                        width: parent.width / 1.07
                        backgroundColor: themeroot.theme.primaryColor
                        elevation: 5
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {
                            secretTxt.text = ""
                            loginDialog.currentindex = loginDialog.secret
                            loginDialog.open()
                        }
                    }

/*
                    RowLayout {
                        id: rll
                        visible: secretTxt.contentWidth >= 1 ? true : false
                        width: parent.width / 2.0
                        spacing: 2
                        anchors.horizontalCenter: parent.horizontalCenter


                        Button{
//                            Layout.implicitWidth: parent.height / 2.0
                            Layout.fillWidth: true;
                            id: clearSecret
                            elevation: 2
                            visible: rll.visible //secretTxt.contentWidth >= 1 ? true : false
                            opacity: visible ? 1: 0
                            text: "Ok got it!"
                            backgroundColor: themeroot.theme.primaryColor
//                            width: parent.width
                            Behavior on opacity {NumberAnimation{duration: 1000;}}
                            onClicked: {
                                if(secretTxt.contentWidth >= 1 ){
                                    theSecert.text = ""
                                    secretTxt.text = ""
                                    console.log("clearing ")
                                    cliphelper.text = themeroot.uname
                                    cliphelper.selectAll()
                                    cliphelper.copy()
                                    copySecret.text = "Copy Secret"
                                    clearSecret.text = "Ok got it!"
                                    copySecret.enabled = true
                                }
                            }
                        }

                        Button{
                            Layout.fillWidth: true;
//                            Layout.implicitWidth: parent.height / 2.0
                            id: copySecret
                            elevation: 2
                            visible: rll.visible //secretTxt.contentWidth >= 1 ? true : false
                            opacity: visible ? 1: 0
                            text: "Copy Secret"
                            backgroundColor: themeroot.theme.primaryColor
//                            width: parent.width
                            Behavior on opacity {NumberAnimation{duration: 1000;}}

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    console.log("copying ")
                                    cliphelper.text = theSecert.text
                                    cliphelper.selectAll()
                                    cliphelper.copy()
                                    cliphelper.text = ""
                                    copySecret.text = "Secret Copied"
                                    copySecret.enabled = false
                                    clearSecret.text = "Ok got it! (clear clipboard)"

                                }
                            }
                        }
                    }

                    Label {
                        id: secretTxt
                        width: parent.width / 2.0
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        opacity: contentWidth > 0 ? 1 : 0
                        Behavior on visible {NumberAnimation{duration: 1200;}}

                    }
                    Card{
                         id:seCard
                         width: parent.width / 2.0
                         height:  theSecert.implicitHeight//Ban2.height * 2
                         elevation: visible? 5 : 0
                         opacity: secretTxt.opacity
                         anchors.horizontalCenter: parent.horizontalCenter
                        Label {
                            anchors.fill: parent
                            id: theSecert
                            width: parent.width * .90
//                            width: parent.width / 1.07
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                            font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
                            horizontalAlignment: Text.AlignHCenter
                            wrapMode: Text.WordWrap
                        }

                    }
*/
                }
            }

            Card{
                id: bcard
                height: nameText.height + importButton.height + imBan.height + ProtoScreen.guToPx(6)
                width: thelist.width
//                anchors.horizontalCenter: parent.horizontalCenter
                elevation: 1
                Banner{
                    id: imBan
                    text: "Import"
                    backgroundColor: themeroot.theme.primaryColor
                    helpShown: true
                    helperHeader: "Import and export help"
                    helperTxt: "Import last years fantasy name or import a name from another device. Just enter in you 12 word password and the import will happen. "
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
                        backgroundColor: themeroot.theme.primaryColor
                        elevation: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {
                            var mypk = MiddleMan.importMnemonic(nameText.text)
//                            if ( mypk === "" ) {
//                                importExportStatus = "Error: Import failed, please try again"
//                            } else {
//                                importExportStatus = "Trying to import with key: " + mypk
//                            }
//                            myImportDialog.show()
                            nameText.text = ""
                            statusTxt.text = importExportStatus;
                            secretTxt.text = ""
                        }
                    }
                }
            }

        }




        Dialog {
            height: ProtoScreen.guToPx(34)
            minimumHeight: ProtoScreen.guToPx(8)
            width: Math.min(height * 2,parent.width - ProtoScreen.guToPx(1))
            minimumWidth: ProtoScreen.guToPx(16)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

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
                secretTxt.text = "12-word password for " + realRoot.uname + " is displayed below. Please do not share this with anyone:"
                        //+ MiddleMan.getSecret()
                theSecert.text =  MiddleMan.getSecret()
            }
        }
//        Item {
//            id: clipit
//            opacity: 0

//            function copy(text) {
//                helper.text = text;
////                helper.selectAll();
//                helper.copy();
//            }

//        }
    }
}
