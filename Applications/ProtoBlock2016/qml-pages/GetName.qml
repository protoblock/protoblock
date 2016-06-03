import QtQuick 2.0
import QtQuick.Controls 1.4 as Contols
import Material 1.0
//import Socket 1.0
import ProRotoQml.Protoblock 1.0

Item {
    id: loginPage
    width: 200
    height: 200
    Component.onCompleted:{
        pageHelper.title = "Please Login"
        pageHelper.buttonsEnabled = false
    }
    Card{
        id: loginCard
        width: parent.width / 1.07
        height:  parent.height / 1.07
        anchors.centerIn: parent
        elevation: 5
        scale:  root.loginCardScale
        opacity: scale
        Behavior on scale{NumberAnimation{duration:600 ; easing.type: Easing.OutQuint}}
        Rectangle{
            id: aboutHeader
            width: parent.width
            height:   parent.height / 10
            color: "#2196f3"
            Label  {
                text: "Please choose a name"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                font.family: "Roboto"
                font.pixelSize: parent.height / 2
                font.bold: true
                color: "white"
            }
        }
        Image {
            id: logo
            source: "qrc:/logoFinal.png"
            width: parent.width / 1.07
            height: parent.height / 5.7
            fillMode: Image.PreserveAspectFit
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: aboutHeader.bottom
            anchors.topMargin: parent.height / 22
        }

        Card{
            id: signInCard
            width: parent.width / 1.07
            height: parent.height / 1.5 - (aboutHeader.height + logo.height)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: logo.bottom
            anchors.topMargin: logo.height
            Column{
                width: parent.width
                height: parent.height / 2
                anchors.centerIn: parent
                spacing: nameText.height
               TextField{
                    id: nameText
                    width: parent.width / 1.07
                    font.family: "Default"
                    placeholderText: "please enter in a new fantasy name"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onTextChanged: {
                    // fix me handle import
                        root.uname = text

                    }
                }
                Button{
                    id: clamNameButton
                    text: "Claim New Name"
                    width: parent.width / 1.07
                    elevation: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        if (text === "Claim New Name" ){
                            MiddleMan.checkname(nameText.text)
                            //checkNameTimmer.start()
                        }else{
                            MiddleMan.importMnemonic(nameText.text);
                        }
                    }
                }
                Button{
                    text: "Already Have a Name"
                    width: parent.width / 1.07
                    elevation: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        nameText.text = ""
                        if (text === "Already Have a Name" ){
                            clamNameButton.text = "Submit"
                            nameText.placeholderText = "Please enter in you 12 word password"
                            text = "Back"
                        }else{
                                nameText.placeholderText = "please enter in a new fantasy name"
                                text = "Already Have a Name"
                                clamNameButton.text = "Claim New Name"
                        }
                    }
                }
            }
        }

        Label{
            id: errorLabel
            color: Theme.accentColor
            width: parent.width
            horizontalAlignment:  Text.AlignHCenter
            font.family: "Roboto"
            font.pixelSize: Qt.platform.os === "android" ? 32 : 22
            wrapMode: Text.WordWrap
            anchors.top: signInCard.bottom
            anchors.topMargin: paintedHeight*2
            onTextChanged: scale = scale +1
            Behavior on scale {
                NumberAnimation{
                    duration: 1200 ; from: 0 ; to: 1 ; easing.type: Easing.OutBounce
                }
            }
        }
    }//card
}
