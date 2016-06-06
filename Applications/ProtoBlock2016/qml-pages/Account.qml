import QtQuick 2.4

import ProRotoQml.Protoblock 1.0
import Material 1.0
import Material.ListItems 1.0 as ListItems

Item {
    property string errmsg
    Component.onCompleted: pageHelper.title = "Accounts Settings"
    Flickable{
        interactive: true
        width: parent.width
        height: parent.height
        contentHeight: parent.height * 1.10
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
                      "this is your fantasy identity. and we will be used for display on the projjection leaderboard"

                wrapMode: Text.WordWrap
            }
        }



        Card{
            property bool hide
            id: namePicker
            enabled: {
                if (MiddleMan.goodList().length > 0) {
                     true
                }
                else {false}
            }

            width: parent.width / 1.07
            height: {
                if (MiddleMan.goodList().length > 0)
                    parent.height / 1.7
                else 0
             }

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: accountInfoTxt.bottom
            anchors.topMargin: 32
            Banner{
                id: namePickerBannner
                width: namePicker.width
                height: 48
                text: "Choose Name"
            }
            ListView {
                id: repeater
                width: namePicker.width
                height: namePicker.height - 48
                clip: true
                interactive: true
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
//                        opacity: MiddleMan.goodList()[index] === uname ? 1 : 0
                        source:  MiddleMan.goodList()[index] === uname ?
                                     "qrc:/icons/action_account_circle.png" : ""
                        Behavior on opacity {NumberAnimation{duration: 1200}}
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
                }
                Button{
                    id: clamNameButton
                    text: "Claim New Name"
                    width: parent.width / 1.07
                    elevation: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        MiddleMan.checkname(nameText.text)
                        //                if (text === "Claim New Name" ){
                        //                    MiddleMan.checkname(nameText.text)
                        //                    //checkNameTimmer.start()
                        //                }else{
                        //                    MiddleMan.importMnemonic(nameText.text);
                        //                }
                    }
                }
            }
        }







        Dialog {
            id: loginErrorDialog
            title: "Error in Signup"
            positiveButtonText: "back"
            negativeButtonText: "import"
            //            onAccepted: loginCardScale = 1
            //            onRejected:  loginCardScale = 1
            Text{
                width: parent.width
                height: Unit.dp(160)
                wrapMode: Text.WordWrap
                text:  errmsg
            }
            onRejected: rootLoader.source =  Qt.resolvedUrl("qrc:/Import-Export.qml")
//            currentPage = "Import-Export"
        }
        // FIXME why is there 2 connection points ?
        // this could be the cause of the bugs in startup
        Connections {
            target: MiddleMan
            onNameCheckGet: {
                console.log("onNameCheckGet " + status  + " \n" +  name )
                if(status === "true" ) {
                    console.log("name is not taken")
                    //                    root.uname = name
                    MiddleMan.signPlayer(name)
//                    currentPage = "ProtoblockNews"
                    rootLoader.source =  Qt.resolvedUrl("qrc:/ProtoblockNews.qml")
                }
                else {
                    errmsg = name + " is taken. please try with a different name. If this your name from another device, please click import or contact Protoblock for help"
                    //                    root.loginCardScale = 0
                    loginErrorDialog.open()
                    //                    root.errorString =  err
                }
            }

            //            onUsingFantasyName: {
            //                console.log("usingFantasyName " + MiddleMan.playersName )
            //                uname = MiddleMan.playersName;
            //                loginDialog.close()
            //            }
        }

    }
}

