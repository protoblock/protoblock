import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtMultimedia 5.5


import ProRotoQml.Files 1.0
import ProRotoQml.Utils 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItems
Item {
    id: pageHelper
    property alias title: headerTitle.text
    width: root.width
    height: root.height
    ListModel {
        id: sectionTitles
        ListElement { text: "Home"}
        ListElement { text: "Projections"}
        ListElement { text:"Trading"}
        ListElement { text:"NFL News"}
        ListElement { text:"Chat"}
        ListElement { text:"Account" }
    }
    property var  teamBackgrounds: []
    property var playerImages: [ ]
    property int menuLevel: 0

    function modelBack(){
//        menu.model = null
        menu.model = sectionTitles
        menuLevel = 0
    }

    function switchModel(ind){
        switch(ind){
        case 0 :
//            menu.model = null
            menu.model= levelOne
            break;
        case 1 :
//            menuModel = null
            menu.model = levelTwo
            break;
        case 2 :
//           menu.model = null
            menu.model = levelThree
            break;
        case 3 :
//            menu.model= null
            menu.model = levelFour
            break;
        case 4 :
//          menu.model = null
            menu.model = levelFive
            break;
        case 5 :
//          menu.model = null
            menu.model = levelSix
            break;
        }

        menuLevel = 2

//        console.log(menuLevel)

    }



    // Level One
    ListModel {
        id:levelOne
        ListElement{text: "Protoblock News" }
        ListElement{text:"About"}
        ListElement{text:"Contact Us"}
        ListElement{text:"FAQS"}
        ListElement{text:"Back" }
    }

//    property var levelOneIcons: [
//        "qrc:/icons/newspaper.png" ,
//        "qrc:/icons/ic_help.png",
//        "qrc:/icons/ic_contact_mail.png" ,
//        "qrc:/icons/ic_help.png"
//    ]
    // Level Two
    ListModel {
        id: levelTwo
        ListElement{text:"Projections"}
        ListElement{text:"Back" }
    }

    property var levelTwoIcons: [
        "qrc:/icons/newspaper.png"
    ]
    // Level Three News
    ListModel {
        id: levelThree
        ListElement{text: "SeasonLongLandingPage"}
        ListElement{text:"WeeklyLandingPage"}
        ListElement{text:"Back" }
    }

    property var levelThreeIcons: [
        "qrc:/icons/newspaper.png" ,
        "qrc:/icons/ic_help.png",
    ]
    //Level four
    ListModel {
        id: levelFour
        ListElement{text:  "News"}
        ListElement{text: "Tweet search" }
        ListElement{text: "CBS Search" }
        ListElement{text: "Espn Search"}
        ListElement{text: "Nfl Search" }
        ListElement{text: "Roto Search" }
        ListElement{text: "Back"}
    }

    property var levelFourIcons: [
        "qrc:/icons/newspaper.png" ,
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_contact_mail.png" ,
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_help.png",
        "qrc:/icons/ic_help.png"
    ]

    //Level Five
    ListModel {
        id:levelFive
        ListElement{text: "Chat"}
        ListElement{text: "Back"}
    }
    property var levelFiveIcons: [
        "qrc:/icons/newspaper.png" ,

    ]

    // Level Six
    ListModel {
        id:levelSix
        ListElement{text: "Account" }
        ListElement{text: "Import-Export" }
        ListElement{text: "FAQ" }
        ListElement{text: "Back" }
    }
    property var levelSixIcons: [
        "qrc:/icons/account_action_circle.png" ,
        "qrc:/icons/ic_sync.png",
        "qrc:/icons/ic_lightbulb.png"
    ]




    Image{
        id: background
        width: parent.width
        height: parent.height - header.height
        source: "qrc:/backgrounds/ARIF.bmp"
    }

    // Image on the right

    Image {
        id: pImages
        x: parent.width / 2
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width / 3
        height: parent.height / 1.1
        onSourceChanged: scale = 1.1
        Behavior on scale {

            ParallelAnimation{
                NumberAnimation{duration: 1200; from: 0 ; to: 1 ; easing.type:  Easing.OutBack }

                NumberAnimation {
                    target: pImages
                    property: "x"
                    from: parent.width
                    to: parent.width / 1.8
                    duration: 1200
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    Timer{
        id: playerPosTimer
        property int i: 0
        property int a: 0
        property int b: 0
        interval: 4000
        running: true
        repeat: true
        onTriggered: {
            // DOCME
            a = a+1
            if( i % a == 3) {
                background.source = teamBackgrounds[b]
                a = 0
                b = b+1
                if(b === teamBackgrounds.length ){
                    b = 0
                }
            }
            if (i < playerImages.length ){
                pImages.source = playerImages[i]
                i = i +1
            }
            else
            {
                i = 0
                pImages.source = playerImages[i]
                i = i +1
            }

        }
    }
    Rectangle{
        width:  parent.width / 3
        height : parent.height - (48*2 )
        color: "#80000000"
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.leftMargin: parent.width / 16


        ListView{
            id: menu
            width:  parent.width
            height: 64*6
            anchors.verticalCenter: parent.verticalCenter
            spacing: 4
            model: sectionTitles
            delegate:
                Image{
                source: "qrc:/menuButtonLong.png"
                width: parent.width
                height: 64
                Label {
                    text:qsTr("%1").arg( model.text )
                    width: parent.width - 15
                    elide: Text.ElideRight
                    height: 58
                    color: "white"
                    font.pixelSize: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    verticalAlignment: Text.AlignVCenter
                    smooth: true
//                    style: Text.Raised
                    font.bold: true

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
//                            console.log(menuModel)
                            if( menuLevel === 0 ){
                                switchModel(index)
                            }
                            else if (modelData === "Back"){
                                modelBack()
                            }
                            else if (menuLevel > 1 && modelData !== "Back")
                            {
                                var theFile = modelData
                                sliderLoader.source = Qt.resolvedUrl(theFile.replace(/\s/g, "") + ".qml" )
                                backButton.shown = true
                                pageTwo.shown = true
                                headerTitle = theFile
                                playerPosTimer.stop()
                            }
                        }
                    }
                }
            }
//            add: Transition {
//                NumberAnimation { properties: "opacity"; from: 1;to:0; duration: 1200 }
//            }
//            remove: Transition {
//                NumberAnimation { properties: "opacity"; from: 0;to:1; duration: 1200 }
//            }
        }
    }

    Rectangle{
        id: header
        height: 48
        width:  parent.width
        color: "#000000"
        Button{
            id: backButton
            property bool shown: false
            height: 48
            backgroundColor: "#44000000"
            elevation: 1
            text: "Back"
            y: shown === true ?  0 : (- header.height)
            anchors.left: parent.left
            anchors.leftMargin: 10
            Behavior on y {NumberAnimation{duration: 600; easing.type:  Easing.OutQuad}}
            onClicked: {
                pageTwo.shown = false
                // this causes all sorts of odd things to happen
                // Must be a bug upstream
                modelBack()
                // restart Timmer
                playerPosTimer.start()
                shown = false
            }
        }


        Text {
            id: headerTitle
            text: "Main Menu"
            color: "white"
            font.pixelSize: header.height / 1.7
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.width / 12
        }

        Text {
            id: headerUname
            text: uname + " Balance: 0"
            color: "white"
            font.pixelSize: header.height / 1.7
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: parent.width / 12
        }
    }

    Rectangle{
        id: footer
        width: parent.width
        height: 48
        color: "#000000"
        anchors.bottom: parent.bottom


        // the settings
        Image {
            id: logo
            fillMode: Image.PreserveAspectFit
            height: parent.height
            source: "qrc:/logoFinal.png"
            anchors.left: parent.left
            anchors.leftMargin: parent.width / 12
        }



        Rectangle{
            id: settingsButton
            color: "transparent"
            width: parent.width / 12
            height:   parent.height - 2
            border{
                width: 1
                color: "#33FFFFFF"
            }
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: parent.width / 12
            Text {
                id: footerSettings
                text: "Settings"
                color: "white"
                font.pixelSize: parent.height / 1.7
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: settingsMenu.shown === true ? settingsMenu.shown = false: settingsMenu.shown = true
            }
        }


        Rectangle{
            property bool shown: false
            width: settingsButton.width
            height: settingsButton.height * 2.5
            id: settingsMenu
            color: "#88000000"
            border.color: "#44FFFFFF"
            border.width: 1
            anchors.left: settingsButton.left
            y: shown ? parent.height - height - footer.height : parent.height
            Behavior on y {NumberAnimation{duration: 1200; easing.type: Easing.OutBack}}
            ListView{
                anchors.fill: settingsMenu
                model:["Account","User Settings"]
                delegate: Button{
                    backgroundColor: "#88000000"
                    textColor: "#44FFFFFF"
                    width: settingsButton.width - 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: settingsButton.height
                    text: modelData

                    onClicked: {

                    }
                }
            }
        }
    }



    // Backgrounds
    QmlDir{
        id: backDir
        // FIX THIS PATH
        dir: "/Users/satoshi/Desktop/fc/osx/ProRoto2016/assets/pinky/backgrounds"
        scanRecursively: true
        flags: QmlDir.Files
        iteratorFlag: QmlDir.Subdirectories
        Component.onCompleted:{
            setFiles(dir)
            for (var i = 0 ; i < files.length;i++){
//                console.log(files[i])
                teamBackgrounds.push("file://"+files[i])

            }
        }
    }




    // Player Portaits
    QmlDir{
        id: portDir
        // FIX THIS PATH
        dir: "/Users/satoshi/Desktop/fc/osx/ProRoto2016/assets/pinky/player_port"
        scanRecursively: true
        flags: QmlDir.Files
        iteratorFlag: QmlDir.Subdirectories
        Component.onCompleted:{
            setFiles(dir)
            for (var i = 0 ; i < files.length;i++){
//                console.log(files[i])
                playerImages.push("file://"+files[i])

            }
        }
    }






    // The Slider  AKA the slider!
    Rectangle {
        id: pageTwo
        property bool shown: false
        height: parent.height - (footer.height+ header.height)
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: header.height
        x: shown === true ?  0 : parent.width
        Behavior on x {NumberAnimation{duration: 1200 ; easing.type: Easing.OutBack}}
        color: "black"
        Image{
            id: nextPageBackground
            anchors.fill: parent
            source: "qrc:/football.jpg"
        }
        Loader{
            id: sliderLoader
            anchors.fill: parent
        }
    }

    property int currentTrack : 0
    function getNewTrack() {
        if (currentTrack === trackList.length){
            currentTrack = 0
        }else{
            currentTrack = currentTrack +1
        }
        player.source = trackList[currentTrack]
        player.play()

        // FIXME make media objects that work
        var strMedia = trackList[currentTrack]
        var cutLast = strMedia.substring(strMedia.lastIndexOf("/")+1)
//        console.log(cutLast)
        var removeExt = cutLast.substring(0,cutLast.lastIndexOf("."))
//        console.log(removeExt)

        trackName.text = "Artist: NFL Films Presents" + "\nSong: "+ removeExt.replace(/-/g," ")
        trackNameTimerOne.start()
    }

    Audio {
        id: player
        onError: console.log("Media Player Error " +  errorString)
        onStatusChanged: {
            if (status === Audio.EndOfMedia){
                getNewTrack();
            }
        }
    }


    property var trackList: []
    // Player Portaits
    QmlDir{
        id: autoDir
        // FIX THIS PATH
        dir: "/Users/satoshi/Desktop/fc/osx/ProRoto2016/pinky-Tunes"
        scanRecursively: true
        flags: QmlDir.Files
        iteratorFlag: QmlDir.Subdirectories
        Component.onCompleted:{
            setFiles(dir)
            for (var i = 0 ; i < files.length;i++){
//                console.log(files[i])
                trackList.push("file://"+files[i])
            }
            getNewTrack();
        }
    }



    Rectangle{
        id: trackNameRec
        property bool shown: false
        color: "#88000000"
        border.color: "#44FFFFFF"
        height:shown ?64: 0
        width: shown ? height*4 : 0
        Behavior on width {NumberAnimation{duration: 600; easing.type: Easing.OutQuad}}
        anchors.left: parent.left
        anchors.leftMargin: parent.width / 22
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height / 18
        Text {
            id: trackName
            anchors.fill: parent
            color: "white"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            opacity: trackNameRec.shown ? 1 : 0
        }
    }
    Timer{
        id: trackNameTimerOne
        interval: 1000
        repeat: false
        onTriggered: {
            trackNameRec.shown = true
            trackNameTimerTwo.start()
        }
    }
    Timer{
        id: trackNameTimerTwo
        interval: 5000
        repeat: false
        onTriggered: {
            trackNameRec.shown = false
        }
    }
}
