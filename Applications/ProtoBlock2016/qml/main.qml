import QtQuick 2.4
import QtQuick.Window 2.0


import ProRotoQml.Protoblock 1.0
import ProRotoQml.Sql 1.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Theme 1.0


import Material 1.0
import Material.ListItems 1.0 as ListItem
ApplicationWindow{
    id: root
  visible: true
  width: Device.productType === "osx"||Device.productType === "win32" ? 1200  :  Screen.width
  height: Device.productType === "osx"||Device.productType === "win32" ? 1220  :  Screen.height

    // Pages
    property var sections: [ levelOne, levelTwo, levelThree,levelFour,levelFive, levelSix ]
    property var sectionsIcons: [
        levelOneIcons,
        levelTwoIcons,
        levelThreeIcons,
        levelFourIcons,
        levelFiveIcons,
        levelSixIcons
    ]
    property var sectionTitles: [ "Home", "Projections", "Trading", "NFL News", "Chat", "Account" ]
    property var sectionTitlesAlias: [ "Home", "Projections", "Trading","NFL News", "Chat","Account" ]
    property var sectionTitlesIcons: [
        "qrc:/logoOnly.png",
        "qrc:/icons/ic_poll.png",
        "qrc:/icons/ic_poll.png",
        "qrc:/icons/newspaper.png",
        "qrc:/icons/ic_help.png",
        "qrc:/icons/action_account_circle.png"
    ]


    property int  currentTabInFocus: 0

    property bool  reloadhome: false

    property string pageSwitcher
    property string currentPage: sections[0][0]
    // we set this to 18 because there is no 18 so that it changes of the fly
    property int loginCardScale: 1
    property string  baseUrl: "http://protoblock.com/php/simple.php?url=https://158.222.102.83:4545/"

    theme {
        primaryColor: Colors.blue
        accentColor: Colors.amber
        tabHighlightColor: Colors.white
    }

    //    "Welcome", "WelcomeBack", "About", "Chat", "GetName", "PickUserName",
    //    "Players", , "UserSettings",
    //    , "TradingLanding","PickUserName" , "WeeklyLandingPage" , "SeasonLongLandingPage"
    //    ,"WeeklyTradingLanding","SeasonLongLevelTwo"


    // Level One
    property var levelOne: [ "Protoblock News" , "About" , "Contact Us","FAQS"  ]
    property var levelOneIcons: [
        "qrc:/icons/newspaper.png" ,
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_contact_mail.png" ,
        "qrc:/icons/ic_help.png"
    ]
    // Level Two
    property var levelTwo: [ "Projections" ]
    property var levelTwoIcons: [
        "qrc:/icons/newspaper.png"
    ]
    // Level Three News
//    property var levelThree: [ "SeasonLongLandingPage", "WeeklyLandingPage" ]
    property var levelThree: [ "SeasonLongLandingPage" ]

    property var levelThreeIcons: [
        "qrc:/icons/newspaper.png" ,
//        "qrc:/icons/ic_help.png",
    ]
    //Level four
    property var  levelFour: [
        "News", "Tweet search" ,"CBS Search" , "Espn Search", "Nfl Search" ,"Roto Search"
    ]
    property var levelFourIcons: [
        "qrc:/icons/newspaper.png" ,
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_contact_mail.png" ,
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_help.png"
    ]

    //Level Five
    property var levelFive: [
        "Chat"
    ]
    property var levelFiveIcons: [
        "qrc:/icons/newspaper.png" ,

    ]

    // Level Six
    property var levelSix: [ "Account" , "Import-Export" , "FAQ"  ]
    property var levelSixIcons: [
        "qrc:/icons/account_action_circle.png" ,
        "qrc:/icons/ic_sync.png",
        "qrc:/icons/ic_lightbulb.png"
    ]


    //Login dialog
    Dialog {
        id: loginDialog
        title: "Please Login"
        anchors.centerIn: parent
        positiveButtonText: "Done"
    }


    initialPage: TabbedPage {
        id: pageHelper
        title: "ProtoBlock 2016"
        onSelectedTabChanged: {
            title = sectionTitles[selectedTabIndex]
            var cp = sectionTitles[selectedTabIndex]
            rootLoader.source = Qt.resolvedUrl(cp.replace(/\s/g, "") + ".qml" )
            console.log("SELECTED TAB      " +selectedTabIndex)
        }
        actionBar.maxActionCount: navDrawer.enabled ? 3 : 4
        backAction: navDrawer.action
        property bool doit: navDrawer.enabled
        NavigationDrawer {
            id: navDrawer
            enabled:{
                if ( Device.name === "phone" || Device.name === "tablet"){
                    true
                }else if (pageHelper.width < Unit.dp(600)){
                    true
                }else{
                    false
                }
            }
            Flickable {
                anchors.fill: parent
                contentHeight: Math.max(content.implicitHeight, height)
                Column {
                    id: content
                    anchors.fill: parent
                    Repeater {
                        model: navDrawer.enabled ? sections : 0

                        delegate: Column {
                            property var section: modelData
                            property string strCheck: section[0]
                            width: parent.width
                            ListItem.Subtitled {
                                id: tabMin
                                backgroundColor: Colors.primaryColor
                                text: sectionTitles[index]
                                action: Image{
                                    source: sectionTitlesIcons[index]
                                    height: 32
                                    width: 32
                                    fillMode: Image.PreserveAspectFit
                                }
                                onClicked:{
//                                    rootLoader.source = "qrc:/" +sectionTitles[index] + ".qml"
                                }


                                Component.onCompleted: {

                                        if ( rootLoader.myi >= sectionTitles.length)
                                            return

                                        console.log(" i " + rootLoader.myi)
                                        rootLoader.myi = rootLoader.myi+1
                                        if (rootLoader.myi === sectionTitles.length ){
                                            console.log("All Done")

                                            defaultname = MiddleMan.init()
                                            console.log("default name: " + defaultname)
                                            if ( defaultname  === "" ){
                                                rootLoader.source =  Qt.resolvedUrl("qrc:/Account.qml")
                                                pageHelper.selectedTabIndex = 5
                                            }else{
                                                rootLoader.source =  Qt.resolvedUrl("qrc:/Home.qml")
                                                pageHelper.selectedTabIndex = 0
                                            }
                                        }
                                    }

                            }
                            Repeater {
                                model: section

                                // TODO iocns
                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == strCheck
                                    onClicked: {
//                                        console.log(sectionTitles[index] )
//                                        rootLoader.source = "qrc:/"+sectionTitles[index] + ".qml"

                                        strCheck = section[index]
                                        pageHelper.title = modelData
                                        var theFile = modelData
                                        rootLoader.source  = Qt.resolvedUrl(theFile.replace(/\s/g, "") + ".qml" )
                                    }
                                }
                            }
                        }

                    }
                }
            }
        }
        ProgressCircle {
            id: actInd
            anchors.centerIn: rootLoader
            visible: rootLoader.status == Loader.Loading
        }

        Loader {
            id: rootLoader
            property int myi: 0
            width: root.width - (navDrawer.enabled ? 0 : 250)
            height: navDrawer.height
            asynchronous: true
            visible: status == Loader.Ready
            anchors {
                right: parent.right
            }
            source: {
                var theFile = navDrawer.enabled ?  root.currentPage : currentPage
                Qt.resolvedUrl(theFile.replace(/\s/g, "") + ".qml" )
            }
//            onSourceChanged: pageHelper.title = sec
            onStatusChanged: console.log("Loader " + status)
        }


        Repeater {
            model: !navDrawer.enabled ? sections : 0
            delegate: Tab {
                title: sectionTitles[index]
                iconName: sectionTitlesIcons[index]
                property string currentPage: modelData[0]
                property var section: modelData
                source: "qrc:/LeftMenu.qml"

                Component.onCompleted: {

                    if ( rootLoader.myi >= sectionTitles.length)
                        return

                   console.log(" i2 " + rootLoader.myi)
                    rootLoader.myi = rootLoader.myi+1
                    if (rootLoader.myi === sectionTitles.length ){
                        console.log("All Done2")

                        defaultname = MiddleMan.init()
                        console.log("default name: " + defaultname)
                        if ( defaultname  === "" ){
                            rootLoader.source =  Qt.resolvedUrl("qrc:/Account.qml")
                            pageHelper.selectedTabIndex = 5
                        }else{
                            rootLoader.source =  Qt.resolvedUrl("qrc:/Home.qml")
                            pageHelper.selectedTabIndex = 0
                        }
                    }

                }
            }
        }
    }


//    Indicators{
//        anchors.right: parent.right
//        anchors.top: parent.top
//        anchors.topMargin: 24
//        anchors.rightMargin: 48 * 2
//    }

    /////////////
    ListModel{id: postionModel}
    ListModel{id: weekModel}

    property string defaultname
//    Component.onCompleted: {
//        console.log( "The formfactor of this device is " + Device.name )
//        fillDefaultModels()
//        defaultname = MiddleMan.init()
//        if ( defaultname  === "" ){
//            rootLoader.source =  Qt.resolvedUrl("qrc:/Account.qml")
//            pageHelper.selectedTabIndex = 5
//        }else{
//            rootLoader.source =  Qt.resolvedUrl("qrc:/Home.qml")
//            pageHelper.selectedTabIndex = 0
//        }
//    }

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








//    Image{
//        id: loaderImage
//        source: "qrc:/logoFinal.png"
//        opacity: 0
//        scale: opacity
//        width: parent.width / 1.07
//        anchors.centerIn: parent
//    }




    // Set up the default connections to the databases

//    QmlSqlDatabase{
//        id: mainTfProdDb
//        databaseName: "/Users/satoshi/Desktop/fc/osx/ProRoto2016/assets/database/tfprod.db"
//        databaseDriver: QmlSqlDatabase.SQLight
//        connectionName: "protoblock"
//        onConnectionOpened: console.log("database Open")
//        onError: console.log("DB Error:  " +  errorString)
//        Component.onCompleted: addDataBase()
//    }


    Label {
        rotation: -45
        text: qsTr("Demo Not Live")
        color: "#60000000"
        anchors.centerIn: parent
        font.pixelSize: Unit.dp(48)
        font.bold:  true
    }


    Dialog {
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

    Connections {
        id: connectionPoints
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


//        onNameCheckGet: {
//            if(status === "true" ) {
//                MiddleMan.signPlayer(name)
////                    currentPage = "ProtoblockNews"
//            }
//            else {
//                errorString = name + " is taken. please try with a different name. If this your name from another device, please click import or contact Protoblock for help"
//                loginErrorDialog.open()
//            }
//        }


//        onNameCheckGet: {
//            console.log("onNameCheckGet " + status  + " \n" +  name )
//            if(status === "true" ) {
//                uname = name
//                MiddleMan.signPlayer(name)
//            }
//            else {
//                errmsg = name + " is taken. please try with a different name. If this your name from another device, please click import or contact Protoblock for help"
//                loginErrorDialog.open()
//            }
//        }

        onUsingFantasyName: {
            console.log("usingFantasyName " + name )
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



}
