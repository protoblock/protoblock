import QtQuick 2.0
import QtQuick.Controls 1.4
import Material 1.0 as Material
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Protoblock 1.0
import QtQuick.Window 2.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Files 1.0

Material.ApplicationWindow{
    id: root
    width: Screen.width
    height: Screen.height
    visible: true
    property string  uname: "NULL"
    property string unameStatus
    property string err
    property int loginCardScale: 1

    Component.onCompleted: {
        console.log("FILENAME: "+ MiddleMan.encyptPath)
    }
    PageHelper{
        id: pageHelper
        width: root.width
        height:root.height
        title: "Please Claim a name"
        buttonsEnabled: false
        object :
            Loader{
            id: rootLoader
            width: parent.width / 1.07
            height:parent.height / 1.07
            opacity: 1
            anchors.centerIn: parent
            sourceComponent: login
            Behavior   on opacity {NumberAnimation{duration: 1200 ;}}
        }

        Component{
            id: login
            GetName{
                width: root.width
                height: root.height
            }
        }


        Component{
            id: afterLogin
            Material.Card{
                height:parent.height / 1.07
                width: parent.width / 1.07
                elevation: 5
                anchors.centerIn: parent
                Column{
                    width: parent.width
                    height: parent.height
                    spacing:  ProtoScreen.guToPx(3)

                    Rectangle{
                        width: parent.width
                        height: parent.height / 12
                        color: Colors.blue

                        Text {
                            id: t
                            horizontalAlignment: Image.AlignHCenter
                            text: qsTr("Thanks for claiming a name more to come soon " )
                            font.pointSize:  ProtoScreen.guToPx(3)
                            wrapMode: Text.WordWrap
                            color: "white"
                        }
                    }
                    ListItems.Subtitled{
                        id: cNameInFocus
                        elevation: 1
                        width: parent.width / 1.07
                        text: "FantasyName: " +  uname
                        subText: "Status of name " +  unameStatus
                        action: RoundImage{
                            height: parent.height
                            width : height
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/icons/action_account_circle.png"
                        }
                        valueText: "Balance : " + " 0"
                    }
                    Text {
                        id: uLabel
                        width: parent.width / 1.07
                        height: parent.height / 16
                        horizontalAlignment: Image.AlignHCenter
                        text: "Use Other Name"
                        font.pointSize:  ProtoScreen.guToPx(6)
                        wrapMode: Text.WordWrap
                    }
                    ComboBox{
                        id: cCombo
                        width: parent.width / 1.07
                        model:["Some Name","Another Name","YAN","NOT"]
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Material.Button {
                        id: nLabel
                        elevation: 1
                        width: parent.width / 1.07
                        height: parent.height / 16
                        text: "Create New Name"
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: rootLoader.sourceComponent = login
                    }


                    Material.Button {
                        elevation: 1
                        width: parent.width / 1.07
                        height: parent.height / 16
                        text: "Encrypt User " + uname
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: rootLoader.sourceComponent = eLogin
                    }
                }
            }
        }



        Component{
            id: eLogin
            EncryptLogin{
                width: parent.width
                height: parent.height
            }
        }






        Connections {
            target: MiddleMan

            Component.onCompleted:{
                MiddleMan.init();
            }
            onEngineStatusChanged:{
                console.log("ENGINE STATUS: " +engineStatus)



            }

            onNameCheckGet: {
                if(status === "true" )
                {
                    console.log("name is not taken")
                    MiddleMan.signPlayer(uname)
                }
                else
                {
                    err = "This name is taken if you feel that you are this person. You can go back and claim you last years name.  Of if you need help feel free to send a email to support@protoblock.com"
                    root.loginCardScale = 0
                    loginErrorDialog.open()
                    root.errorString =  err
                }
            }

            onNameStatusChanged: {
                if(MiddleMan.engineStatus === true ){
                    uname = MiddleMan.playersName;
                    unameStatus = MiddleMan.playersStatus
                }
                else
                {
                    console.log("Doing nothing as there is not a good engine")
                }
            }

            onUsingFantasyName: {
                if(MiddleMan.engineStatus === true ){
                    uname = MiddleMan.playersName;
                    console.log("THE PATH " + MiddleMan.encyptPath + uname +"-lock")
                    // check to see if there is a lock file for the username
                    if ( fCheck.exists(MiddleMan.encyptPath + uname +"-lock") === true ){
                        rootLoader.sourceComponent = eLogin
                        rootLoader.opacity = 1
                    }
                    else
                    {
                        rootLoader.sourceComponent = afterLogin
                        pageHelper.title = "Thanks For claiming a name"
                        rootLoader.opacity = 1
                    }
                }
                else
                {

                    console.log(MiddleMan.playersName + " Doing nothing as there is not a good engine")
                }
            }
        }

    QmlFile{ id:fCheck }

    Material.Dialog {
        id: loginErrorDialog
        title: "Error in Signup"
        positiveButtonText: "back"
        onAccepted: loginCardScale = 1
        onRejected:  loginCardScale = 1
        Label{
            width: parent.width
            wrapMode: Text.WordWrap
            text:  err
        }
    }
    }
}
