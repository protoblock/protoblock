import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Sql 1.0
import Material 1.0 as Material
Material.ApplicationWindow {
    id: root
    width: Screen.width
    height: Screen.height
    visible: true
    property string uname
    property string currentHomeTeam
    property string currentAwayTeam
    property string errorString
    property string importExportStatus
    Component.onCompleted: {
        fillDefaultModels();
    }
    Loader {
        id: rootL
        width: parent.width
        height: parent.height
        source: Qt.resolvedUrl("qrc:/MainPinky.qml")
    }

    ListModel{id: postionModel}
    ListModel{id: weekModel}

    // check for updates
    BaserModel {
        id: updateMachine
        source:"http://50.0.50.101/version.json"
        onUpdated: {
            var vObject = JSON.parse(json)
            console.log(vObject.length)
            for (var i = 0 ; i < vObject.length ; i++){
                console.log( vObject[i].version)
                console.log( vObject[i].changeLog)
            }
        }
    }

    function fillDefaultModels(){
        var positionArray = ["all positions","QB","RB","WR","TE","K","DEF"];
        for (var i in positionArray){
            postionModel.append({'text': positionArray[i] })
        }
        for (var ii = 0 ; ii < 17; ii++){
            if(ii === 0 ){
                weekModel.append({"text" : "all weeks"})
            }
            else
            {
                weekModel.append({"text" : ii.toString() })
            }
        }
    }

    Connections {
        target: MiddleMan
        onNameCheckGet: {
            //            console.log("onNameCheckGet " + status  + " \n" +  name )
            if(status === "true" )
            {
                //                console.log("name is not taken")
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
            //            console.log("nameStatusChange " + MiddleMan.playersName  +" " + MiddleMan.playersStatus )
            uname = MiddleMan.playersName;
        }

        onUsingFantasyName: {
            //            console.log("usingFantasyName " + MiddleMan.playersName )
            uname = MiddleMan.playersName;
            //            currentPage = "MainPinky"
        }
    }



    // Set up the default connections to the databases

    QmlSqlDatabase{
        id: mainTfProdDb
        databaseName: "/Users/satoshi/Desktop/fc/osx/ProRoto2016/assets/database/tfprod.db"
        databaseDriver: QmlSqlDatabase.SQLight
        connectionName: "protoblock"
        onConnectionOpened: console.log("database Open")
        onError: console.log("DB Error:  " +  errorString)
        Component.onCompleted: addDataBase()
    }

    Dialog {
        id: usingNameDialog
        title: "Account"
        Material.Label{
            width: parent.width
            wrapMode: Text.WordWrap
            text: msgString
        }
    }

    //Login dialog
    Dialog {
        id: loginDialog
        title: "Please Login"
    }

    Material.Dialog {
        id: accountErrorDialog
        title: "Error in Signup"
        positiveButtonText: "Back"
        negativeButtonText: "Import"
        //            onAccepted: loginCardScale = 1
        //            onRejected:  loginCardScale = 1
        Material.Label{
            width: parent.width
            height: Unit.dp(160)
            wrapMode: Text.WordWrap
            text:  errorString
        }
        onRejected: {
            rootLoader.source = "qrc:/Import-Export.qml"
        }
    }

    Dialog{
        id: loginErrorDialog
        title: "Error in Signup"
        Material.Label{
            width: parent.width
            height: parent.height
            wrapMode: Text.WordWrap
            text:  root.errorString
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


        onNameCheckGet: {
            if(status === "true" ) {
                MiddleMan.signPlayer(name)
                root.reloadhome = false
                rootLoader.source = "qrc:/ProtoblockNews.qml"
                pageHelper.selectedTabIndex = 0;
            }
            else {
                errorString = name + " is taken. please try with a different name. If this your name from another device, please click import or contact Protoblock for help"
                accountErrorDialog.open()
            }
        }

        onUsingFantasyName: {
            if ( uname !== name) {
                if ( !isdefault ) {
                    msgString = "You are now playing as: " + name
                    usingNameDialog.open()
                }
                console.log("usingFantasyName " + name )
            }
            uname = name
            getNames()
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
}

