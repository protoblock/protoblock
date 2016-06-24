import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.0
import QtMultimedia 5.5

import ProRotoQml.Files 1.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Theme 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItems

Item {
    id: pageHelper
    property alias title: headerTitle.text
    width: Screen.width
    height: Screen.height
    property var  teamBackgrounds: []
    property var playerImages: [ ]
    property int menuLevel: 0

    function modelBack(){
        menu.model = null
        menu.model = sectionTitles
        menuLevel = 0
    }

    function switchModel(ind){
        switch(ind){
        case 0 :
            menu.model = null
            menu.model= levelOne
            break;
        case 1 :
            menu.model = null
            menu.model = levelTwo
            break;
        case 2 :
            menu.model = null
            menu.model = levelThree
            break;
        case 3 :
            menu.model= null
            menu.model = levelFour
            break;
        case 4 :
            menu.model = null
            menu.model = levelSix
            break;
        case 5 :
            menu.model = null
            menu.model = levelSix
            break;
        }
        menuLevel = 2
    }


    LevelOneMenuModel{id: levelOne}
    LevelTwoMenuModel{id: levelTwo}
    LevelThreeMenuModel{id: levelThree}
    LevelFourMenuModel{id: levelFour}
    LevelFiveMenuModel{id: levelFive}
    LevelSixMenuModel{id: levelSix}
    SectionTitleMenuModel{id: sectionTitles}


    Image{
        id: background
        width: parent.width
        height: parent.height - header.height
        source: "qrc:/backgrounds/ARIF.bmp"
    }

    // Image on the right

    Image {
        id: pImages2
        x: pImages.x
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width / 3
        height: parent.height / 1.1
        source:  pImages.source
        onXChanged:{
            if( x === (parent.width / 1.8) ){
                seAnimation.start()
            }
        }
        ParallelAnimation{
            id: seAnimation
            NumberAnimation{ target: pImages2; property: "opacity"; from : 1 ; to: 0; duration: 1200; }
            NumberAnimation{ target: pImages2; property: "scale"; from : 1 ; to: 2; duration: 1200; }
        }
    }
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
                NumberAnimation { target: pImages; property: "x"; from: parent.width; to: parent.width / 1.8;
                    duration: 1200; easing.type: Easing.InOutQuad; }

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
        height : parent.height - ProtoScreen.guToPx(12)
        color: "#80000000"
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.leftMargin: parent.width / 16
        ListView{
            id: menu
            width:  parent.width
            height:ProtoScreen.guToPx(48)
            anchors.verticalCenter: parent.verticalCenter
            spacing: 4
            model: sectionTitles
            delegate:
                Image{
                source: "qrc:/menuButtonLong.png"
                width: parent.width
                height: ProtoScreen.guToPx(8)
                Label {
                    text:qsTr("%1").arg( model.text )
                    width: parent.width - 15
                    elide: Text.ElideRight
                    height:ProtoScreen.guToPx(8)
                    color: "white"
                    font.pixelSize: ProtoScreen.guToPx(6)
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    verticalAlignment: Text.AlignVCenter
                    style: Text.Raised
                    font.bold: true

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            console.log(index)
                            if( menuLevel === 0 ){
                                switchModel(index)
                            }
                            else if (model.text === "Back"){
                                modelBack()
                            }
                            else if (menuLevel > 1 && model.text !== "Back")
                            {
                                var theFile = model.text
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
            add: Transition {
                NumberAnimation { properties: "opacity"; from: 1;to:0; duration: 1200 }
            }
            remove: Transition {
                NumberAnimation { properties: "opacity"; from: 0;to:1; duration: 1200 }
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


    Rectangle{
        id: header
        height: ProtoScreen.guToPx(6)
        width:  parent.width
        color: "#000000"
        Button{
            id: backButton
            property bool shown: false
            height: ProtoScreen.guToPx(6)
            backgroundColor: "#44000000"
            elevation: 1
            text: "Back"
            y: shown === true ?  0 : (- header.height)
            anchors.left: parent.left
            anchors.leftMargin: ProtoScreen.guToPx(1.25)
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
            text: realRoot.uname + " Balance: 0"
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
        height: ProtoScreen.guToPx(6)
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
            id: settingsMenu
            property bool shown: false
            width: settingsButton.width
            height: settingsButton.height * 2.5
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
                        var cSpace = modelData
                        sliderLoader.source = Qt.resolvedUrl("qrc:/" +cSpace.replace(/\s/g,"")+ ".qml")
                        pageTwo.shown = true
                        backButton.shown = true
                        settingsMenu.shown = false
                    }
                }
            }
        }
    }



//    // Backgrounds
    QmlDir{
        id: backDir
        // FIX THIS PATH
        dir: "/Users/satoshi/Desktop/fc/osx/ProRoto2016_NOSQL/assets/pinky/backgrounds"
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
        dir: "/Users/satoshi/Desktop/fc/osx/ProRoto2016_NOSQL/assets/pinky/player_port"
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
        dir: "/Users/satoshi/Desktop/fc/osx/ProRoto2016_NOSQL/pinky-Tunes"
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
        height:shown ? ProtoScreen.guToPx(8): 0
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
