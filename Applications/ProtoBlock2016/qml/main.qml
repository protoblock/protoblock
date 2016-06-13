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
    width: Device.productType === "osx"||Device.productType === "win32" ? ProtoScreen.guToPx(150)  :  Screen.width
    height: Device.productType === "osx"||Device.productType === "win32" ? ProtoScreen.guToPx(150)  :  Screen.height

    Component.onCompleted: {
        uname = MiddleMan.init()
        if ( uname  === "" ){
            loginDialog.toggle()
        }
        else
            rootLoader.source = "qrc:/Home.qml";
    }

    property string defaultname

    property string  errorString
    property bool  reloadleaders: false

    // LOGIN STUFF
    property string importExportStatus
    property string secretTxt
    property string statusTxt
    //    property string mypk


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
    property var sectionTitles: [ "Home", "Projections", "Trading", "NFL News", "Proto Chat", "Account" ]
    property var sectionTitlesAlias: [ "Home", "Projections", "Trading","NFL News", "Proto .Chat","Account" ]
    property var sectionTitlesIcons: [
        "qrc:/logoOnly.png",
        "qrc:/icons/ic_poll.png",
        "qrc:/icons/ic_poll.png",
        "qrc:/icons/newspaper.png",
        "qrc:/icons/ic_help.png",
        "qrc:/icons/action_account_circle.png"
    ]


    property int  currentTabInFocus: 0

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
    property var levelOne: [ "Protoblock News" , "About" , "Contact Us" ]
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
    property var levelThree: [ "Trading" ]

//    ,"SeasonLongLandingPage", "WeeklyLandingPage"


    property var levelThreeIcons: [
        "qrc:/icons/newspaper.png" ,
        "qrc:/icons/ic_help.png",
    ]
    //Level four
    property var  levelFour: [
        "News", "Twitter" ,"CBS" , "ESPN", "NFL" ,"Roto World"
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
        "Proto Chat"
    ]
    property var levelFiveIcons: [
        "qrc:/icons/newspaper.png" ,

    ]

    // Level Six
    property var levelSix: [ "Account" , "Import-Export"  ]
    property var levelSixIcons: [
        "qrc:/icons/account_action_circle.png" ,
        "qrc:/icons/ic_sync.png",
        "qrc:/icons/ic_lightbulb.png"
    ]
    property string selectedComponent: sections[0][0]



    initialPage: TabbedPage {
        id: pageHelper
        title: "ProtoBlock 2016"

        onSelectedTabChanged: {
            title = sectionTitles[selectedTabIndex]

            var cp = sectionTitles[selectedTabIndex]
            rootLoader.source = Qt.resolvedUrl(cp.replace(/\s/g, "") + ".qml" )
        }

        actionBar.maxActionCount: navDrawer.enabled ? 3 : 4
        backAction: navDrawer.action
        NavigationDrawer {
            id: navDrawer
            enabled:{
                if ( ProtoScreen.formFactor === "phone" || ProtoScreen.formFactor === "tablet"){
                    true
                }else if (pageHelper.width < ProtoScreen.guToPx(120)){
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
                        model: sections
                        delegate: Column {
                            width: parent.width
                            ListItem.Subtitled {
                                id: tabMin
                                backgroundColor: Colors.primaryColor
                                text: sectionTitles[index]
                                action: Image{
                                    source: sectionTitlesIcons[index]
                                    height: ProtoScreen.guToPx(4)
                                    width:ProtoScreen.guToPx(4)
                                    fillMode: Image.PreserveAspectFit
                                }
                                onClicked:{
                                    var theFile = modelData;
                                    rootLoader.source = Qt.resolvedUrl(theFile.replace(/\s/g, "") + ".qml" )

                                    navDrawer.close()
                                    navDrawer.showing = false
                                }
                            }
                            Repeater {
                                model: modelData
                                // TODO iocns
                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == root.currentPage
                                    onClicked: {
                                        var theFile = modelData;
                                        Qt.resolvedUrl(theFile.replace(/\s/g, "") + ".qml" )
                                        rootLoader.source = "qrc:/"+theFile + ".qml"
                                        navDrawer.close()
                                        navDrawer.showing = false
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }



        Loader {
            id: rootLoader
            // sidebar is ProtoScreen.guToPx(31.25)
            width: navDrawer.enabled === true ? (root.width - navDrawer.width)  :  (pageHelper.width - ProtoScreen.guToPx(31.25) )
            height:navDrawer.height
            visible: status == Loader.Ready
            anchors.right: parent.right
        }



        ProgressCircle {
            id: actInd
            anchors.centerIn: rootLoader
            visible: rootLoader.status == Loader.Loading
        }

        Repeater {
            model: !navDrawer.enabled ? sections : 0
            delegate: Tab {
                title: sectionTitles[index]
                iconName: sectionTitlesIcons[index]
                property string currentPage: modelData[0]
                property var section: modelData
                source: "qrc:/LeftMenu.qml"
            }
        }




    }// END TABED PAGE

    Indicators{
        id: indicators
        anchors{
            top: parent.top
            topMargin: ProtoScreen.guToPx(1.7)
            right: parent.right
            rightMargin: ProtoScreen.guToPx(2)
        }
    }

//    Label {
//        rotation: -45
//        text: qsTr("Demo Not Live")
//        color: "#40000000"
//        anchors.centerIn: parent
//        font.pixelSize: ProtoScreen.font( ProtoScreen.XXLARGE)
//        font.bold:  true
//    }


    /////////////
    ///////////////////




    //    SIMPLE MODELS
    ListModel{id: postionModel}
    ListModel{
        id: weekModel
        Component.onCompleted: {
            fillDefaultModels()
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

    // Set up the default connections to the databases
    QmlSqlDatabase{
        id: mainTfProdDb
//        databaseName: "162.254.67/tfprod.db"
        databaseName: "/Users/satoshi/Desktop/fc/osx/protoblock/assets/database/tfprod.db"
        databaseDriver: QmlSqlDatabase.SQLight
        connectionName: "protoblock"
        onError: console.log("DB Error:  " +  errorString)
        Component.onCompleted: addDataBase()
    }



    /// DIALOGS
    Dialog {
        id: usingNameDialog
        title: "Account"
        Text{
            width: parent.width
            height: parent.height
            wrapMode: Text.WordWrap
            font.pixelSize:  ProtoScreen.font(ProtoScreen.LARGE)
            text: msgString
        }
    }

    //Login dialog (only when user does not have a secert3)
    Dialog {
        id: loginDialog
        hasActions: false
        width: root.width
        height: root.height
        contentMargins: 0
        GetName{
            width: root.width
            height: root.height
        }
    }

    Dialog {
        id: accountErrorDialog
        title: "Error in Signup"
        positiveButtonText: "Back"
        negativeButtonText:  loginDialog.visible ?  "" : "Import"
        onRejected: {
            if (loginDialog.visible === false){
                rootLoader.source = "qrc:/Import-Export.qml"
            }
        }
        Column{
            spacing: ProtoScreen.guToPx(2)
            width: parent.width
            height: parent.height
            Text{
                width: parent.width
                height: ProtoScreen.guToPx(20)
                wrapMode: Text.WordWrap
                text:  errorString
                font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            }
            Button{
                text: "Email protoblock staff"
                elevation: loginDialog.visible === true ?5 : 0
                width: loginDialog.visible === true ? parent.width: 0
                onClicked: Qt.openUrlExternally("mailto:contact@protoblock.comexample.com=Login%20support")
            }
        }
    }


    Dialog{
        id: loginErrorDialog
        title: "Error in Signup"
        Label{
            width: parent.width
            height: parent.height
            wrapMode: Text.WordWrap
            text:  root.errorString
            font.pixelSize:ProtoScreen.font( ProtoScreen.LARGE)
        }
    }


    Dialog {
        id: myImportDialog
        title: "Import status"
        positiveButtonText: "Back"
        Column{
            anchors.fill: parent
            Text{
                width: parent.width
                wrapMode: Text.WordWrap
                text:  importExportStatus
                font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            }
        }
    }


    Connections {
        target: MiddleMan

        onNameCheckGet: {
            if(status === "true" ) {
                MiddleMan.signPlayer(name)
                root.reloadleaders = false
                rootLoader.source = "qrc:/Projections.qml"
                pageHelper.selectedTabIndex = 1;
            }
            else {
                errorString = name + " is taken. Please try with a different name. Or if you feel that this is in fact your name from last year. Or would like to import from another device."
                if (loginDialog.visible){
                    +" Please click back. Then click \"I Already Have a Name\" "
                }else if (!loginDialog.visible){
                    + " Please click Import below."
                }
                + "For more assistance please contact the protoblock staff at <contact@protoblock.com>"
                accountErrorDialog.open()
            }
        }

        onUsingFantasyName: {
            if ( uname !== name) {
                uname = name
                msgString = "Congraulation You are now playing as: " + name
                if( pageHelper.selectedTabIndex === 5 || loginDialog.visible === true){
                    usingNameDialog.toggle()
                }
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
