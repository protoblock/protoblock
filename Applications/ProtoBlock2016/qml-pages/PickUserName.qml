import QtQuick 2.0
import QtQuick.Controls 1.4 as Contols
import Material 1.0
import ProRotoQml.Protoblock 1.0

Item {
    id: loginPage
    width: 200
    height: 200
    property string  err
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
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: aboutHeader.bottom
            anchors.topMargin: parent.height / 22
            fillMode: Image.PreserveAspectFit

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
                MainView{
                    id: nameText
                    width: parent.width / 1.07
                    anchors.horizontalCenter: parent.horizontalCenter

                }
                Button{
                    id: clamNameButton
                    text: "Use Name"
                    width: parent.width / 1.07
                    elevation: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                    }
                }
            }
        }//card
    }
}
