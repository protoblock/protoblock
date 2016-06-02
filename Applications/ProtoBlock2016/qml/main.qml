import QtQuick 2.4
import QtQuick.Window 2.0


import ProRotoQml.Protoblock 1.0
import ProRotoQml.Sql 1.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Theme 1.0


import Material 1.0
import Material.ListItems 1.0 as ListItem

ApplicationWindow {
    id: root
    title: "Protoblock 2016 "
    width: Screen.width
    height:  Screen.height
    visible: true

    // Pages
    property var sections: [ basicComponents, styles, compoundComponents,chater ]
    property var sectionTitles: [ "Protoblock Home", "Projections", "Trading","NFL News",  "Chat" ]
    property string currentPage: sections[0][0]
    // we set this to 18 because there is no 18 so that it changes of the fly
    property string  err
    property string currentTeamInFocus
    property string currentHomeTeam
    property string currentAwayTeam
    property string uname: "NULL"
    property string errorString
    property int loginCardScale: 1
    property string  baseUrl: "http://protoblock.com/php/simple.php?url=https://158.222.102.83:4545/"

//    property alias currentPage: rootLoader.source

//    property var splashWindow: Splash {
//        onTimeout: {

//            root.visible = true
//            if (uname !== "NULL"){
//             console.log("have name")
//            }else{
//            loginDialog.show();
//            }
//        }
//    }

    theme {
        primaryColor: Colors.blue
        accentColor: Colors.amber
        tabHighlightColor: Colors.white
    }

    //    "Welcome", "WelcomeBack", "About", "Chat", "GetName", "PickUserName",
    //    "Players", , "UserSettings",
    //    , "TradingLanding","PickUserName" , "WeeklyLandingPage" , "SeasonLongLandingPage"
    //    ,"WeeklyTradingLanding","SeasonLongLevelTwo"




    // Level Two
    property var styles: [
           "SeasonLongLandingPage", "WeeklyLandingPage"
    ]
    // Level One
    property var basicComponents: [
        "WelcomeBack","Welcome", "About", "UserSettings"


    ]
    // Level Three News
    property var compoundComponents: [
        "News", "Twitter/Tweetsearch" ,"Feeds/CBSSearch" , "Feeds/EspnSearch", "Feeds/NflSearch" ,"Feeds/RotoSearch"
    ]

    property var  chater: [
        "Chat"
    ]



    initialPage: TabbedPage {
        id: pageHelper
        title: "ProtoBlock 2016"
        actionBar.maxActionCount: navDrawer.enabled ? 3 : 4
        actions: [
            Action {
                iconName: "image_color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
            },

            Action {
                iconName: "action_settings"
                name: "Settings"
                hoverAnimation: true
                onTriggered: {
                    console.log("Trigger Hello")
                    root.currentPage = "UserSettings"
                }onToggled: {
                    console.log("Toggle  Hello")

                    currentPage = "UserSettings"
                }

            },

            Action {
                iconName: "action_account_circle"
                name: uname
//                onTriggered: userPicker.show()
            }
        ]

        backAction: navDrawer.action

        NavigationDrawer {
            id: navDrawer
            enabled:{
                if ( Device.name === "phone" || Device.name === "tablet"){
                    true
                }else if (pageHelper.width < Unit.dp(400)){
                    true
                }else{
                    false
                }
            }
            onEnabledChanged: smallLoader.active = enabled

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

                            ListItem.Subheader {
                                backgroundColor: root.theme.primaryColor
                                text: sectionTitles[index]
                            }
                            Repeater {
                                model: modelData
                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == root.currentPage
                                    onClicked: {
                                        root.currentPage = modelData
                                        navDrawer.close()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Repeater {
            model: !navDrawer.enabled ? sections : 0
            delegate: Tab {
                title: sectionTitles[index]
                property string currentPage: modelData[0]
                property var section: modelData
                sourceComponent: tabDelegate
            }
        }

        Loader {
            id: smallLoader
            anchors.fill: parent
            sourceComponent: tabDelegate
            property var section: []
            visible: active
            active: false
        }
    }

//Login dialog
    Dialog {
        id: loginDialog
        width: parent.width / 1.07
        height: parent.height / 1.07
        title: "Please Login"
        anchors.centerIn: parent
        hasActions: false
        positiveButtonText: "Done"
        GetName{
            width: loginDialog.width
            height:  loginDialog.height
        }
    }


    Dialog {
        id: colorPicker
        title: "Pick color"
        positiveButtonText: "Done"

        MenuField {
            id: selection
            model: ["Primary color", "Accent color", "Background color"]
            width: Unit.dp(160)
        }

        Grid {
            columns: 7
            spacing: Unit.dp(8)

            Repeater {
                model: [
                    Colors.red, Colors.pink, Colors.purple, Colors.deepPurple, Colors.indigo,
                    Colors.blue, Colors.lightBlue, Colors.cyan, Colors.teal, Colors.green,
                    Colors.lightGreen, Colors.lime, Colors.yellow, Colors.amber, Colors.orange,
                    Colors.deepOrange, Colors.grey, Colors.blueGrey, Colors.brown, Colors.black,
                    "white"
                ]

                Rectangle {
                    width: Unit.dp(30)
                    height: Unit.dp(30)
                    radius:Unit.dp(2)
                    color: modelData
                    border.width: modelData === "white" ? Unit.dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)

                    Ink {
                        anchors.fill: parent
                        onPressed: {
                            switch(selection.selectedIndex) {
                                case 0:
                                    theme.primaryColor = parent.color
                                    break;
                                case 1:
                                    theme.accentColor = parent.color
                                    break;
                                case 2:
                                    theme.backgroundColor = parent.color
                                    break;
                            }
                        }
                    }
                }
            }
        }

        onRejected: {
            // TODO set default colors again but we currently don't know what that is
        }
    }


    // USERS INFO

//    Dialog {
//        id: userPicker
//        title: "User Information"
//        positiveButtonText: "Done"
//        MenuField {
//            model: userInfo.fantasyName
//            width: Unit.dp(160)
//        }
//        Text{
//            width: parent.width
//            height: Unit.dp(160)
//            wrapMode: Text.WordWrap
//            text: "Your Secert \n"+userInfo.mnemonicKey
//        }
//    }

    Component {
        id: tabDelegate

        Item {

            Sidebar {
                id: sidebar
                expanded: !navDrawer.enabled
                Column {
                    width: parent.width
                    Repeater {
                        model: section
                        delegate: ListItem.Standard {
                            text: modelData
                            selected: modelData == currentPage
                            onClicked: currentPage = modelData
                        }
                    }
                }
            }
            Flickable {
                id: flickable
                anchors {
                    left: sidebar.right
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }
                clip: true
                contentHeight: Math.max(rootLoader.implicitHeight + 40, height)
                Loader {
                    id: rootLoader
                    anchors.fill: parent
                    asynchronous: true
                    visible: status == Loader.Ready
                    source: {
                        var theFile = navDrawer.enabled ?  root.currentPage : currentPage
                        Qt.resolvedUrl(theFile.replace(/\s/g, "") + ".qml" )
                    }
                }

                ProgressCircle {
                    id: actInd
                    anchors.centerIn: parent
                    visible: rootLoader.status == Loader.Loading
                }
            }
            Scrollbar {
                flickableItem: flickable
            }
        }
    }


    /////////////
    ListModel{id: postionModel}
    ListModel{id: weekModel}

    Component.onCompleted: {
            console.log( "The formfactor of this device is " + Device.name )
        fillDefaultModels()
        MiddleMan.init()

//        root.visible = true
        if (uname !== "NULL"){
         console.log("have name")
        }else{
        loginDialog.show();
        }
    }

    function fillDefaultModels(){
//        console.log(" main qml 342")
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
            height: Unit.dp(160)
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
            console.log("onNameCheckGet " + status  + " \n" +  name )
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
            console.log("nameStatusChange " + MiddleMan.playersName )
            uname = MiddleMan.playersName;
             loginDialog.close()
        }

        onUsingFantasyName: {
            console.log("usingFantasyName " + MiddleMan.playersName )
            uname = MiddleMan.playersName;
            loginDialog.close()
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



}
