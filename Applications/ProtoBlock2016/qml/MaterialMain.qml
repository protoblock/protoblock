import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.XmlListModel 2.0

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Sql 1.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Theme 1.0

import Material 1.0 as Material
import Material.ListItems 1.0 as ListItem
import Material.Extras 1.0

import Communi 3.0

Material.ApplicationWindow{
    id: themeroot
    visible: true
    width: (Device.productType === "windows" || Device.productType === "osx") ? Math.min(ProtoScreen.guToPx(150), ProtoScreen.availableWidth * .95)
                                                                              : ProtoScreen.availableWidth
    height: (Device.productType === "windows" || Device.productType === "osx") ? Math.min(ProtoScreen.guToPx(150), ProtoScreen.availableHeight * .95)
                                                                               : ProtoScreen.availableHeight


    color: "transparent"
    Component.onCompleted: {
        setX(ProtoScreen.availrect.x + ProtoScreen.availableWidth /2 - width / 2 );
        setY(ProtoScreen.availrect.y + (ProtoScreen.availableHeight - height))

        console.log( "actual " + height + " ProtoScreen.guToPx(150) "  + ProtoScreen.guToPx(150) + " real " + realRoot.height
                    + " avail " + ProtoScreen.availableHeight + " all " + ProtoScreen.desktopHeight + " design " + ProtoScreen.designHeight)
        console.log("Primary Color " +  Colors.primaryColor  +  " themeroot Active ?  " + themeroot.active)
        uname = MiddleMan.init()
        if ( uname  === "" ){
            loginDialog.toggle()
        }
        else
            rootLoader.source = "qrc:/2016Season.qml";
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
    property var sections: [ levelOne, levelTwo, levelThree,levelFour, levelFive,levelSix ]
    property var sectionsIcons: [
        levelOneIcons,
        levelTwoIcons,
        levelThreeIcons,
        levelFourIcons,
        levelFiveIcons,
        levelSixIcons
    ]
    property var sectionTitles: [ "Home", "Projections", "Trading", "NFLNews", "ProtoChat", "Account" ]
    property var sectionTitlesAlias: [ "Home", "Projections", "Trading","NFLNews", "ProtoChat","Account" ]
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
    property int loginCardScale: 1
    property string  baseUrl: "http://protoblock.com/php/simple.php?url=https://158.222.102.83:4545/"

    theme {
        primaryColor: Colors.primaryColor
        accentColor: Colors.accentColor
        tabHighlightColor: Colors.accentColor
    }

    // Level One
    property var levelOne: [ "Protoblock News" , "About" , "Contact Us","Settings" ]
    property var levelOneIcons: [
        "qrc:/icons/newspaper.png" ,
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_contact_mail.png" ,
        "qrc:/icons/ic_help.png"
    ]
    // Level Two
    property var levelTwo: [ "Projections" , "2015 Leaderboard" ]
    property var levelTwoIcons: [
        "qrc:/icons/newspaper.png"
    ]
    // Level Three News
    property var levelThree: [ "2016 Season" , "2016 Weekly", "2017 Season", "2015 Final" ]

    //    ,"SeasonLongLandingPage", "WeeklyLandingPage"


    property var levelThreeIcons: [
        "qrc:/icons/newspaper.png" ,
        "qrc:/icons/newspaper.png" ,
        "qrc:/icons/newspaper.png",
        "qrc:/icons/newspaper.png"
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



    initialPage:  Material.TabbedPage {
        id: pageHelper
        title: "ProtoBlock 2016"
        onSelectedTabChanged: {
            title = sectionTitles[selectedTabIndex]

            var cp = sectionTitles[selectedTabIndex]
            rootLoader.source = Qt.resolvedUrl("qrc:/"+ cp.replace(/\s/g, "") + ".qml" )
        }

        actionBar.customContent:
            Material.Label{
            text: realRoot.uname
            verticalAlignment: navDrawer.enabled ? Text.AlignVCenter : Text.AlignTop
            font{
                family: "Roboto"
                bold: true
                pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
            }
            color: "white"

            anchors{
                right: parent.left
                rightMargin: ProtoScreen.guToPx(2)
                top: parent.top
                topMargin:{
                    if (navDrawer.enabled === false ){
                        ProtoScreen.guToPx(1)
                    }
                }
                bottom: parent.bottom
            }
        }
        actionBar.maxActionCount: navDrawer.enabled ? 1:3
        actions: [
            Material.Action {
                iconName: "qrc:/icons/action_account_circle.png"
                name: "Account"
                onTriggered: {
                    rootLoader.source = "qrc:/Account.qml"
                    pageHelper.selectedTabIndex = 5
                    pageHelper.title = "Account Settings"
                }
            },
            Material.Action {
                iconName: "qrc:/icons/action_settings.png"
                name: "Settings"
                hoverAnimation: true
                onTriggered: {
                    rootLoader.source  = "qrc:/Settings.qml"
                    pageHelper.selectedTabIndex = 0
                    pageHelper.title = "System Settings"
                }
            }
        ]
        backAction: navDrawer.action
        Material.NavigationDrawer {
            id: navDrawer
            enabled:{
                if ( ProtoScreen.formFactor === "phone" || ProtoScreen.formFactor === "tablet" || ProtoScreen.formFactor === "phablet" ){
                    true
                }else if (pageHelper.width < ProtoScreen.guToPx(120)){
                    true
                }else  {
                    false
                }

            }
            Flickable {
                anchors.fill: parent
                contentHeight: Math.max( (content.implicitHeight + ProtoScreen.guToPx(1)), height)
                Column {
                    id: content
                    anchors.fill: parent
                    Repeater {
                        model: sections
                        delegate: Column {
                            width: parent.width
                            ListItem.Subtitled {
                                id: tabMin
                                backgroundColor: themeroot.theme.primaryColor
                                text: sectionTitles[index]
                                action: Image{
                                    source: sectionTitlesIcons[index]
                                    height: ProtoScreen.guToPx(4)
                                    width:ProtoScreen.guToPx(4)
                                    fillMode: Image.PreserveAspectFit
                                }
                                onClicked:{
                                    var theFile = sectionTitles[index];
                                    var theSource = Qt.resolvedUrl("qrc:/" +  theFile.replace(/\s/g, "")  + ".qml" )
                                    rootLoader.source = theSource

                                    navDrawer.close()
                                    navDrawer.showing = false
                                }
                            }
                            Repeater {
                                model: modelData
                                // TODO iocns
                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == currentPage
                                    onClicked: {
                                        var theFile = modelData;
                                        var theSource = Qt.resolvedUrl("qrc:/" +theFile.replace(/\s/g, "") + ".qml" )
                                        rootLoader.source = theSource
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
            width: navDrawer.enabled === true ? themeroot.width  :  (pageHelper.width - ProtoScreen.guToPx(31.25) )
            height: navDrawer.enabled === true ? themeroot.height : navDrawer.height
            visible: status == Loader.Ready
            anchors.right: parent.right
        }

        Material.ProgressCircle {
            id: actInd
            anchors.centerIn: er
            visible: rootLoader.status == Loader.Loading
        }

        Repeater {
            model: !navDrawer.enabled ? sections : 0
            delegate:  Material.Tab {
                title: sectionTitles[index]
                iconName: sectionTitlesIcons[index]
                property string currentPage: modelData[0]
                property var section: modelData
                source: "qrc:/LeftMenu.qml"
            }
        }

    }// END TABED PAGE


    Material.Label {
        rotation: -45
        text: MiddleMan.isTestNet() ? qsTr("Demo Testing") : qsTr("Live")
        color: "#40000000"
        anchors.centerIn: parent
        font.pixelSize: ProtoScreen.font( ProtoScreen.XXLARGE)
        font.bold:  true
        visible: MiddleMan.isTestNet()
    }


    /////////////
    // End OF GUI Easy Look up
    ///////////////////

    //     WHY !!!!!!!!!!!!!!!!!!!

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


    /// DIALOGS
    Material.Dialog {
        id: usingNameDialog
        title: "Protoblock Player Name"
        Text{
            width: parent.width
            height: parent.height
            wrapMode: Text.WordWrap
            font.pixelSize:  ProtoScreen.font(ProtoScreen.LARGE)
            text: msgString
        }
    }

    //Login dialog (only when user does not have a secert3)
    Material.Dialog {
        id: loginDialog
        hasActions: false
        width: themeroot.width / 1.07
        anchors.centerIn: parent
        height: themeroot.height - 1
        contentMargins: 0
        GetName{
            width: loginDialog.width
            height: loginDialog.height
        }
    }

    Material.Dialog {
        id: accountErrorDialog
        title: "Unavailable"
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
        }



    }


    Material.Dialog{
        id: loginErrorDialog
        title: "Error in Signup"
        Material.Label{
            width: parent.width
            height: parent.height
            wrapMode: Text.WordWrap
            text:  themeroot.errorString
            font.pixelSize:ProtoScreen.font( ProtoScreen.LARGE)
        }
    }


    Material.Dialog{
        id: chatErrorDialog
        title: "Error In chat"
        Material.Label{
            width: parent.width
            height: parent.height
            wrapMode: Text.WordWrap
            text:  "We are sorry but you are either not on the internet or have not cliamed a name"
            font.pixelSize:ProtoScreen.font( ProtoScreen.LARGE)
        }
    }



    Material.Dialog {
        id: myImportDialog
        title: "Import status"
        positiveButtonText: "OK"
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


    Material.Dialog {
        id: updateDialog
        title: "Update Available"
        positiveButtonText: "Back"
        Column{
            anchors.fill: parent
            spacing: 3
            Text{
                width: parent.width
                wrapMode: Text.WordWrap
                text: "There is a update available for Protoblock."
                font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            }
            Material.Button{
                width: parent.width / 1.07
                text: "Download Now"
                elevation: 0
                onClicked: Qt.openUrlExternally("http://protoblock.com/template/downloads.html")
            }
        }
    }


    Material.Dialog {
        id: helperDialog
        title: realRoot.helperHeader
        positiveButtonText: "Ok"
        onAccepted: {
            realRoot.helperHeader = "Help"
            realRoot.helperTxt = ""
        }
        Column{
            anchors.fill: parent
            spacing: 3
            Material.Label{
                width: parent.width
                wrapMode: Text.WordWrap
                text: realRoot.helperTxt
                font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            }
        }
    }


    Connections {
        target: MiddleMan

        onNameCheckGet: {
            if(status === "true" ) {
                MiddleMan.signPlayer(name)
                if ( loginDialog.visible )
                    loginDialog.close()

                themeroot.reloadleaders = false
                rootLoader.source = "qrc:/Projections.qml"
                pageHelper.selectedTabIndex = 1;
            }
            else {
                errorString = name + " is already claimed. Please try with a different name. If this is your name from last year or another device, "
                if (loginDialog.visible){
                    errorString = errorString + "please find the import/export features within the Account tab."
                }else if (!loginDialog.visible){
                    errorString = errorString + " Please click Import below."
                }
                errorString = errorString + "\n\nFor more assistance please contact the protoblock staff at <contact@protoblock.com>"
                accountErrorDialog.open()
            }
        }

        onUsingFantasyName: {
            if ( uname !== name) {
                uname = name
                msgString = "You are now playing as: " + name
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


    function compairVersions(d){
        if (realRoot.version < d){
            //            console.log("there is a update")
            updateDialog.toggle()
        }else{
            //            console.log("There are NO UPDATES ")
        }
    }

    // check for updates
    XmlListModel {
        id: updateMachine
        source:"http://protoblock.com/version.xml"
        query: "/updatemachine"
        XmlRole{name: "version";query: "version/string()"}
        XmlRole{name: "libs";query: "libs/string()"}
        XmlRole{name: "changelog";query: "changelog/string()"}
        onStatusChanged: {
            switch(status){
            case XmlListModel.Error :
                //                console.log("ERROR IN UPDATE MACHINE ")
                break;
            case XmlListModel.Ready:
                compairVersions(updateMachine.get(0).version)
                break;
            }
        }
    }
    IrcConnection {
        property string  tempName: realRoot.uname === "" ? "protblockUser" + Math.floor(Math.random() * 5000) + 1  : realRoot.uname
        property string tempName1: ""
        id: ircConnectionPoint
        host: "162.254.24.67"
        port: 6667
        secure: false
        saslMechanism: ""
        nickName: tempName
        realName:tempName
        userName:tempName
        password:""
    }

    IrcBufferModel {
        id: ircBufferModel
        sortMethod: Irc.SortByTitle
        connection:ircConnectionPoint
        onMessageIgnored: ircServerBuffer.receiveMessage(message)
        function quit() {
            bufferModel.clear()
            ircConnectionPoint.quit("Fantasy Just Got Real")
            ircConnectionPoint.close()
        }
    }
    IrcBuffer {
        id: ircServerBuffer
        sticky: true
        persistent: true
        name: ircConnectionPoint.displayName
        Component.onCompleted: ircBufferModel.add(ircServerBuffer)
    }

    /*!
      * This is the left gesture bar that is used only for
      * tablets and phones and phablets. It is a swipe gesture to
      * open up the navbar. Kinda buggy but better then nothing.
      */
    Rectangle{
        id:leftGesture
        color: "transparent"
        width:{
            if(ProtoScreen.formFactor === "phone"
                    || ProtoScreen.formFactor === "phablet"
                    || ProtoScreen.formFactor === "tablet" )
            {
                ProtoScreen.guToPx(1)
            }
            else
            {
                0
            }
        }
        height: parent.height
        x: {
            if (navDrawer.enabled === true && navDrawer.showing === false){
                0
            }else{
                navDrawer.width
            }
        }
    }
    MouseArea{
        anchors.fill: leftGesture
        anchors.margins:leftGesture.width > 0 ? ProtoScreen.guToPx(3) : 0
        drag.target: leftGesture
        drag.minimumX: 0
        drag.axis: Drag.XAxis
        onPressed: {
            if(ProtoScreen.formFactor === "phone" || ProtoScreen.formFactor === "phablet"
                    || ProtoScreen.formFactor === "tablet" )
            {
                if (navDrawer.showing === false)
                {
                    navDrawer.showing = true
                }
                else if (navDrawer.showing === true)
                {
                    navDrawer.showing = false
                }
            }
        }
    }
}
