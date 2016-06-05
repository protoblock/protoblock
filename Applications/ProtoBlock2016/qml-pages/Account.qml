import QtQuick 2.4

import ProRotoQml.Protoblock 1.0
import Material 1.0
import Material.ListItems 1.0 as ListItems
Card {

    Component.onCompleted: pageHelper.title = "Accounts"
    height:parent.height / 1.07
    width: parent.width / 1.07
    elevation: 5
    anchors.top: parent.top
    anchors.topMargin:  parent.height / 22

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
            text: "Protoblock players must claim a name to create an account" +
                  "Chose your Twitter handle or fantasy team name. " +
                  "this is your fantasy identity. and we will be used for display on the projjection leaderboard"

            wrapMode: Text.WordWrap
        }

        Repeater {
            id: repeater
            model: MiddleMan.goodList().length
            ListItems.Subtitled{
                elevation: 1
                width: parent.width / 1.07
                text: "FantasyName: " +  MiddleMan.goodList()[index]
//                        subText: "Status of name " +  unameStatus
                action: RoundImage{
                    height: parent.height
                    width : height
                    fillMode: Image.PreserveAspectFit
                    source: MiddleMan.goodList()[index] === uname ?
                                "qrc:/icons/action_account_circle.png" : ""
                }
                valueText: "Balance : " + " 0"
                onClicked: MiddleMan.useName(MiddleMan.goodList()[index])
            }

        }


//        Button {
//            id: nLabel
//            elevation: 1
//            width: parent.width / 1.07
//            height: parent.height / 16
//            text: "Create New Name"
//            anchors.horizontalCenter: parent.horizontalCenter
//            onClicked: rootLoader.sourceComponent = login
//        }


        TextField {
            id: nameText
            width: parent.width / 1.07
            font.family: "Default"
            placeholderText: "please enter in a new fantasy name"
            anchors.horizontalCenter: parent.horizontalCenter
//            onTextChanged: {
//                root.uname = text
//            }
        }

        Button{
            id: clamNameButton
            text: "Claim New Name"
//            width: parent.width / 1.07
            elevation: 5
//            anchors.horizontalCenter: parent.horizontalCenter
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
            onRejected: currentPage = "Import-Export"

//            onAccepted: ""

        }

        Connections {
            target: MiddleMan
            onNameCheckGet: {
                console.log("onNameCheckGet " + status  + " \n" +  name )
                if(status === "true" ) {
                    console.log("name is not taken")
//                    root.uname = name
                    MiddleMan.signPlayer(name)
                    currentPage = "ProtoblockNews"
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

