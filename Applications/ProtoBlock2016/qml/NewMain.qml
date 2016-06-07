import QtQuick 2.0
import QtQuick.Window 2.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Protoblock 1.0

import Material 1.0 as Material
Window {
    id: realRoot
    //1200 w
    // 1219 h
    title: "Protoblock 2016 "
    width: Device.productType === "osx"||Device.productType === "win32" ? 1200  :  Screen.width
    height: Device.productType === "osx"||Device.productType === "win32" ? 1220  :  Screen.height
    property string theme: "Material"
    property string errorString
    property string  uname
    property string  err
    property string currentTeamInFocus
    property string currentHomeTeam
    property string currentAwayTeam
    //    property string uname: "NULL"
    property string msgString

    onThemeChanged:
        if(theme === "Material") {
            newLoader.source = "qrc:/main.qml"
        }else{
            newLoader.source = "qrc:/"+theme+".qml"
        }
    visible: false
    Loader{
        id: newLoader
        anchors.fill: parent
        Component.onCompleted: {
            if(theme === "Material") {
                source = "qrc:/main.qml"
            }else{
                source = "qrc:/"+theme+".qml"
            }
        }
    }

    Connections {
        target: MiddleMan
        //        onNameCheckGet: {
        //            console.log("onNameCheckGet " + status  + " \n" +  name )
        //            if(status === "true" )
        //            {
        //                console.log("name is not taken")
        //                MiddleMan.signPlayer(uname)
        //            }
        //            else
        //            {
        //                err = "This name is taken if you feel that you are this person. You can go back and claim you last years name.  Of if you need help feel free to send a email to support@protoblock.com"
        //                root.loginCardScale = 0
        //                loginErrorDialog.open()
        //                root.errorString =  err
        //            }
        //        }

        //        onNameStatusChanged: {
        //            console.log("nameStatusChange " + MiddleMan.playersName )
        //            uname = MiddleMan.playersName;
        //             loginDialog.close()
        //        }

        onUsingFantasyName: {
            if ( uname !== name) {
                uname = name
                if ( !isdefault ) {
                    msgString = "You are now playing as: " + name
                    usingNameDialog.open()
                }
                console.log("usingFantasyName " + name )

            }
        }

        onImportSuccess: {
            console.log(passfail + "onImportSucess " + name )

            if ( passfail ) {
                msgString = name + " - Imported!"
                usingNameDialog.open()
            }
            else {
                errorString = name
                loginErrorDialog.open()
            }
            console.log(passfail + "onImportSucess " + name )
        }
    }






    Material.Dialog {
        id: usingNameDialog
        title: "Account"
        positiveButtonText: "ok"
        Text{
            width: parent.width
            height: Unit.dp(160)
            wrapMode: Text.WordWrap
            text: msgString
        }
    }



    Material.Dialog {
        id: loginErrorDialog
        title: "Error in Signup"
        positiveButtonText: "back"
        //        onAccepted: loginCardScale = 1
        //        onRejected:  loginCardScale = 1
        Text{
            width: parent.width
            height: Unit.dp(160)
            wrapMode: Text.WordWrap
            text:  errorString
        }
    }
}
