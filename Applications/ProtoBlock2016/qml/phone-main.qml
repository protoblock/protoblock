import QtQuick 2.3
import QtQuick.Window 2.2
import Material 1.0
import ProRotoQml.Protoblock 1.0
//import ProRotoQml.Files 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Sql 1.0

ApplicationWindow {
    id: root
    width: Screen.width
    height: Screen.height
    visible: false
    theme {
        primaryColor: Theme.primaryColor
        accentColor: Theme.accentColor
        tabHighlightColor: "white"
    }
    property string currentTeamInFocus
    property string currentHomeTeam
    property string currentAwayTeam
    property string uname
    property string errorString
    property int loginCardScale: 1
    property string  baseUrl: "http://protoblock.com/php/simple.php?url=https://158.222.102.83:4545/"
    property var splashWindow: Splash {
        onTimeout: root.visible = true
    }

    property variant pagesList  : [
        "Welcome", "WelcomeBack", "About", "Chat", "GetName", "PickUserName",
        "Players", "ProjectionsLevelOne", "UserSettings","News","Twitter/Tweetsearch"
        , "Feeds/CBSSearch" , "Feeds/EspnSearch", "Feeds/NflSearch" ,"Feeds/RotoSearch"
        , "TradingLanding","PickUserName" , "WeeklyLandingPage" , "SeasonLongLandingPage"
        ,"WeeklyTradingLanding","SeasonLongLevelTwo"
    ];

    Component.onCompleted: {
        fillDefaultModels();
        currentPage = "GetName"
    }

    // FIXME set so that we check for secert 3 and whatever the fuck jay wants
    property string currentPage
    onCurrentPageChanged:  {
        //            console.log("Loader should animate")
        loaderAnimation.start()
    }

    SequentialAnimation{
        id: loaderAnimation
        NumberAnimation { target: pageHelper ; property:  "m_opacity"; from: 1; to: 0 ; duration: 300 }
//        NumberAnimation { target: loaderImage ; property: "opacity"; from: 0; to: 1 ; duration: 250 }
//        NumberAnimation { target: loaderImage ; property: "opacity"; from: 1; to: 0 ; duration: 250 }
        NumberAnimation { target: pageHelper ; property: "m_opacity"; from: 0; to: 1 ; duration: 250 }
    }


    PageHelper{
        id: pageHelper
        width: parent.width
        height: parent.height
        object:
            Repeater {
            model: pagesList;
            anchors.fill: parent
            delegate: Loader {
                id: rootLoader
                active: false;
                asynchronous: true;
                width: parent.width
                height: parent.height
                visible: (currentPage === modelData);
                source: Qt.resolvedUrl("qrc:/%1.qml".arg(modelData))
                onSourceChanged:{
                    //                         console.log("source changed " + source)
                }
                onVisibleChanged: {
                    loadIfNotLoaded();
                }
                Component.onCompleted: {
                    loadIfNotLoaded();
                }
                function loadIfNotLoaded () {

                    // to load the file at first show
                    if (visible && !active) {
                        active = true;
                    }
                }
            }
        }
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
    Dialog {
        id: loginErrorDialog
        title: "Error in Signup"
        positiveButtonText: "back"
        onAccepted: loginCardScale = 1
        onRejected:  loginCardScale = 1
        Text{
            width: parent.width
            height: dp(160)
            wrapMode: Text.WordWrap
            text:  errorString
        }
    }
    Image{
        id: loaderImage
        source: "qrc:/logoFinal.png"
        opacity: 0
        scale: opacity
        width: parent.width / 1.07
        anchors.centerIn: parent
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
            currentPage = "WelcomeBack"
            pageHelper.buttonsEnabled = true
        }
    }



    // Set up the default connections to the databases

    QmlSqlDatabase{
        id: mainTfProdDb
        databaseName: "/Users/$$(USER)/Desktop/fc/osx/ProRoto2016/assets/database/tfprod.db"
        databaseDriver: QmlSqlDatabase.SQLight
        connectionName: "protoblock"
        onConnectionOpened: console.log("database Open")
        onError: console.log("DB Error:  " +  errorString)
        Component.onCompleted: addDataBase()

    }
}

