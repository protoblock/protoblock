import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.XmlListModel 2.0

//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Theme 1.0

import Material 1.0 as Material
import Material.ListItems 1.0 as ListItem
import Material.Extras 1.0

import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

import Communi 3.0

Material.ApplicationWindow{

    property string version: "2.3" //version
    property alias realRoot: themeroot

    property string  uname

    property string  err
    property string currentTeamInFocus
    property string currentHomeTeam
    property string currentAwayTeam
    property string helperTxt
    property string helperHeader: "Help"
    property bool reloadrowquote: true
    property string msgString
    property bool reloadorderpos: true


    statusBar: StatusBar {

        RowLayout {
            spacing: 2
            anchors.fill: parent
            Material.Label {
//                height: parent.height
//                anchors.left: parent.left
                text: " Block Number: " + MiddleMan.blocknum
                      + " - " + MiddleMan.liveSync
                      + " - " + MiddleMan.theSeason + " Week " + MiddleMan.theWeek
                font.pixelSize: ProtoScreen.font( ProtoScreen.SMALL)

            }

            Material.Label {
                text: MiddleMan.controlMessage
                font.pixelSize: ProtoScreen.font( ProtoScreen.SMALL)
                onLinkActivated: Qt.openUrlExternally(link)
                enabled: MiddleMan.liveSync === "Live"
            }

            SpinBox {
                enabled: false
                decimals: 0
                stepSize: 1.0
                maximumValue: 50000
                minimumValue:  MiddleMan.blocknum
                value: MiddleMan.height
                onValueChanged: {
                    MiddleMan.settheHeight(value)
                }
            }
        }
    }

    id: themeroot
    visible: true
//    width: ProtoScreen.availableWidth * .95//(Device.productType === "windows" || Device.productType === "osx") ? ProtoScreen.availableWidth //* .95 : ProtoScreen.availableWidth
//    height: ProtoScreen.availableHeight *.95//(Device.productType === "windows" || Device.productType === "osx") ? ProtoScreen.availableHeight //* .95 : ProtoScreen.availableHeight

    width: (Device.productType === "windows" || Device.productType === "osx") ?
               Math.min(1920, ProtoScreen.availableWidth) : ProtoScreen.availableWidth
//    height: (Device.productType === "windows" || Device.productType === "osx") ?
//               (ProtoScreen.availableWidth >= 1920 ?  1080 : ProtoScreen.availableHeight - ProtoScreen.guToPx(7)) : ProtoScreen.availableHeight

    height: ProtoScreen.availableHeight - ProtoScreen.guToPx(4)
    color: "transparent"
    Component.onCompleted: {
        setX(ProtoScreen.availrect.x + ProtoScreen.availableWidth /2 - width / 2 );
        setY(ProtoScreen.availrect.y + (ProtoScreen.availableHeight - height))

        console.log( "actual " + height + " ProtoScreen.guToPx(150) "  + ProtoScreen.guToPx(150) + " real " + realRoot.height
                    + " avail " + ProtoScreen.availableHeight + " all " + ProtoScreen.desktopHeight + " design " + ProtoScreen.designHeight)

        console.log( "actual width" + width + " ProtoScreen.guToPx(150) "  + ProtoScreen.guToPx(150) + " real " + realRoot.width
                    + " avail " + ProtoScreen.availableWidth + " all " + ProtoScreen.desktopWidth + " design " + ProtoScreen.designWidth)

        console.log("Primary Color " +  Colors.primaryColor  +  " themeroot Active ?  " + themeroot.active)

        uname = MiddleMan.init()
        if ( uname  === "" ){
//            loginDialog.toggle()
        }
        else {

        }

//        themeroot.show();
//        width = (Device.productType === "windows" || Device.productType === "osx") ?
//                   Math.min(1920, ProtoScreen.availableWidth) : ProtoScreen.availableWidth
//        height = (Device.productType === "windows" || Device.productType === "osx") ?
//                   (ProtoScreen.availableWidth >= 1920 ?  1080 : ProtoScreen.availableHeight) : ProtoScreen.availableHeight

//        themeroot.show();
//        themeroot.showMaximized()
        rootLoader.source = start

    }

    property string defaultname

    property string start: "qrc:/Trading.qml"
    property int startindex: 1
    property string  errorString
    property bool  reloadleaders: false

    // LOGIN STUFF
    property string importExportStatus
    property string secretTxt
    property string statusTxt
    //    property string mypk


    // Pages
    property var sections: [ levelOne, levelTwo, levelThree,levelFour, levelFive ]
    property var sectionsIcons: [
        levelOneIcons,
        levelTwoIcons,
        levelThreeIcons,
        levelFourIcons,
        levelFiveIcons
    ]
    property var sectionTitles: [ "Projections","Trading",  "NFL News", "Account", "Protoblock"  ]
    property var sectionTitlesAlias: [ "Projections", "Trading", "NFL News", "Account", "Protoblock" ]
    property var sectionTitlesIcons: [
        "qrc:/icons/ic_poll.png",
        "qrc:/icons/ic_timeline.png",
        "qrc:/icons/newspaper.png",
        "qrc:/icons/action_account_circle.png",
        "qrc:/icons/ic_help.png"
    ]


    property int  currentTabInFocus: 0

    property string pageSwitcher
    property string currentPage: sections[0][0]
    property int loginCardScale: 1

    theme {
        primaryColor: Colors.primaryColor
        accentColor: Colors.accentColor
        tabHighlightColor: Colors.accentColor
    }

    // Level One Trading
    property var levelTwo: [ "Trading"]

    //    ,"SeasonLongLandingPage", "WeeklyLandingPage"


    property var levelTwoIcons: [
        "qrc:/icons/newspaper.png"
    ]


    // Level Two
    property var levelOne: [ "Projections"]
    property var levelOneIcons: [
        "qrc:/icons/newspaper.png"
    ]

    //Level Three
    property var  levelThree: [
        "Roto World", "CBS" , "ESPN", "NFL","Twitter"
    ]
    property var levelThreeIcons: [
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_help.png",
    ]

    // Level Four
    property var levelFour: [ "Account" , "Import-Export", "Proto Chat", ]
    property var levelFourIcons: [
        "qrc:/icons/account_action_circle.png" ,
        "qrc:/icons/ic_sync.png",
        "qrc:/icons/ic_lightbulb.png",
    ]

    // Level Five
    property var levelFive: [ "Protoblock" , "Leaderboard", "Scoring System", "Contact Us"]
    property var levelFiveIcons: [
        "qrc:/icons/newspaper.png" ,
        "qrc:/icons/account_action_circle.png" ,
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_contact_mail.png" ,
    ]


    property string selectedComponent: sections[0][0]

    property var sectionLeftEnable: [ false, false, true, true, true]

    initialPage:  Material.TabbedPage {
        property bool expanded: true
        id: pageHelper
        title: "Protoblock 2017"
        selectedTabIndex: startindex
        onSelectedTabChanged: {
            title = sectionTitles[selectedTabIndex]
            var cp = sectionTitles[selectedTabIndex]
            rootLoader.source = Qt.resolvedUrl("qrc:/"+ cp.replace(/\s/g, "") + ".qml" )
            console.log(" onSelectedTabChanged " + selectedTabIndex)
//            navDrawer.enabled = sectionTitles[selectedTabIndex] === "Projections"
            expanded = sectionLeftEnable[selectedTabIndex];
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
                    pageHelper.selectedTabIndex = 3
                    pageHelper.title = "Account Settings"
                }
            }
//            ,Material.Action {
//                iconName: "qrc:/icons/action_settings.png"
//                name: "Settings"
//                hoverAnimation: true
//                onTriggered: {
//                    rootLoader.source  = "qrc:/Settings.qml"
//                    pageHelper.selectedTabIndex = 3
//                    pageHelper.title = "System Settings"
//                }
//            }
        ]
        backAction: navDrawer.action
        Material.NavigationDrawer {
            id: navDrawer
            enabled:{
                if ( ProtoScreen.formFactor === "phone" || ProtoScreen.formFactor === "tablet" || ProtoScreen.formFactor === "phablet" ){
                    true
                }else if (pageHelper.width < ProtoScreen.guToPx(120)){
                    true
                }
                else {
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
            width: (navDrawer.enabled === true) ? themeroot.width  :
                  pageHelper.width - (pageHelper.expanded === false ? 0.0 : ProtoScreen.guToPx(31.25))
            height: parent.height//navDrawer.enabled === true ? themeroot.height : navDrawer.height
            visible: status == Loader.Ready
            anchors.right: parent.right

            Component.onCompleted: {
                console.log(" matmain " + rootLoader.width)
            }
        }

        Material.ProgressCircle {
            id: actInd
            anchors.centerIn: rootLoader
            visible: rootLoader.status == Loader.Loading
        }

        Repeater {
            model: !navDrawer.enabled  ? sections : 0
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

    Material.Label {
        text: "Syncing... Block " + MiddleMan.blocknum
             + " of " + MiddleMan.height
        anchors.centerIn: parent
        font.pixelSize: ProtoScreen.font( ProtoScreen.XXLARGE)
        font.bold:  true
        visible: MiddleMan.liveSync === "Sync"
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
            fillDefaultModels(MiddleMan.theWeek)
        }
    }

    ListModel { id: teamModel}

    function fillDefaultModels(theweek){

        weekModel.clear()
        postionModel.clear()
        teamModel.clear();
        var positionArray = ["ALL","QB","RB","WR","TE","K","DEF"];
        for (var i in positionArray){
            postionModel.append({'text': positionArray[i] })
        }
        for (var ii = 0 ; ii < theweek+1; ii++){
            if(ii === 0 ){
                weekModel.append({"text" : "all weeks"})
            }
            else
            {
                weekModel.append({"text" : ii.toString() })
            }
        }

        var teamsArray =  ["ALL",
                           "ARI" ,
                           "ATL" ,
                           "BAL" ,
                           "BUF" ,
                           "CAR" ,
                           "CHI" ,
                           "CIN" ,
                           "CLE" ,
                           "DAL" ,
                           "DEN" ,
                           "DET" ,
                           "GB" ,
                           "HOU" ,
                           "IND" ,
                           "JAC" ,
                           "KC" ,
                           "MIA" ,
                           "MIN" ,
                           "NE" ,
                           "NO" ,
                           "NYG" ,
                           "NYJ" ,
                           "OAK" ,
                           "PHI" ,
                           "PIT" ,
                           "LAC" ,
                           "SEA" ,
                           "SF" ,
                           "LAR" ,
                           "TB" ,
                           "TEN" ,
                           "WAS"];

        for (var iii in teamsArray){
            teamModel.append({'text': teamsArray[iii] })
        }


    }


    /// DIALOGS
    Material.Dialog {
        height: parent.height / 2
        width: parent.width / 2
        id: usingNameDialog
        title: "Protoblock Player Name"
        Text{
            width: parent.width
            height: parent.height
            wrapMode: Text.WordWrap
            font.pixelSize:  ProtoScreen.font(ProtoScreen.NORMAL)
            text: msgString
        }
    }

    Material.Dialog {
        id: importexportDialog
        height: parent.height / 2
        width: parent.width / 2
        title: "Import/Export projections CSV"
        Text{
            width: parent.width
            height: parent.height
            wrapMode: Text.WordWrap
            font.pixelSize:  ProtoScreen.font(ProtoScreen.NORMAL)
            text: "using csv file: " + ImportLoader.filename
        }

        positiveButtonText: "Export"
        negativeButtonText: "Import"
        onAccepted: {
            ImportLoader.doExport(uname)
        }

        onRejected: {
            ImportLoader.doImport(uname)

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
                onClicked: {
                    if ( ProtoScreen.os === "osx" ) {
                        Qt.openUrlExternally("http://protoblock.com/Downloads/MacOS/64/protoblock.dmg")
                    }
                    else if ( ProtoScreen.os === "windows" ) {
                        Qt.openUrlExternally("http://protoblock.com/Downloads/Windows/64/protoblock.exe")
                    }
                    else if ( ProtoScreen.os === "ios"  ) {
                        Qt.openUrlExternally("https://itunes.apple.com/us/app/protoblock-2016/id1133758199");
                    }
                    else if ( ProtoScreen.os === "android") {
                        Qt.openUrlExternally("https://play.google.com/store/apps/details?id=org.proto.protoblock")
                    }
                    else
                        Qt.openUrlExternally("http://protoblock.com/template/downloads.html")
                }
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
        Material.Label{
            width: parent.width
            height: parent.height
            wrapMode: Text.WordWrap
            text: realRoot.helperTxt
            font.pixelSize:ProtoScreen.font( ProtoScreen.NORMAL)
            Component.onCompleted: {
                console.log (" Material.Label " + parent.width)

            }
        }
    }

    BusyIndicator {
        id: newnameInd
        running: MiddleMan.busySend
        anchors.centerIn: parent
    }

    Connections {
        target: MiddleMan


        onTheWeekChanged: {
            console.log( "onTheWeekChanged")
            fillDefaultModels(MiddleMan.theWeek)
        }

        onNameCheckGet: {
            console.log( "namehcek material main" + name + status)
            if(status === "true" ) {
                MiddleMan.signPlayer(name)
                if ( loginDialog.visible )
                    loginDialog.close()

                themeroot.reloadleaders = false
                rootLoader.source = start
                pageHelper.selectedTabIndex = startindex;
                rootLoader.showMaximized
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
            console.log(uname + " qml usingfantay name " + name)
            if ( uname !== name || uname === "") {
                uname = name
                msgString = "You are now playing as: " + name
                if( pageHelper.selectedTabIndex === 3 || loginDialog.visible === true){
                    rootLoader.source = start
                    pageHelper.selectedTabIndex = startindex;
                    usingNameDialog.open()
                }
                else
                    console.log(" no popup")
            }
            else console.log("ignoring usingfantasyname")
        }

        onImportSuccess: {
            console.log(passfail + "onImportSucess " + name )
            if ( passfail ) {
                msgString = name + " - Imported!"
                usingNameDialog.open()
            }
            else {
                importExportStatus = "Error: Import failed, please try again"
                myImportDialog.open()
            }
//            console.log(passfail + "onImportSucess " + name )
        }
    }


    function compairVersions(d){
        if (realRoot.version < d){
            //            console.log("there is a update")
            updateDialog.toggle()
        }else{

             console.log("There are NO UPDATES " + ProtoScreen.os)
        }
    }

//    // check for updates
    XmlListModel {
        id: updateMachine
        source: "http://protoblock.com/version-" + ProtoScreen.os + ".xml"
        query: "/updatemachine"
        XmlRole{name: "version";query: "version/string()"}
        XmlRole{name: "libs";query: "libs/string()"}
        XmlRole{name: "changelog";query: "changelog/string()"}
        onStatusChanged: {
            switch(status){
            case XmlListModel.Error :
                console.log("ERROR IN UPDATE MACHINE " + source)
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
