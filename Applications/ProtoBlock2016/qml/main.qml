import QtQuick 2.0
import QtQuick.Window 2.2 as Quick
import QtQuick.Dialogs 1.2

import ProRotoQml.Utils 1.0
import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0


import Material 1.0 as Material


Quick.Window {
    id: realRoot
    title: "Protoblock 2016 xxxxxx"
//    x: ( ProtoScreen.desktopWidth - ProtoScreen.availableWidth)
//    y: ( ProtoScreen.desktopHeight - ProtoScreen.availableHeight)
//    width: ProtoScreen.availableWidth
//           (Device.productType === "windows") ? 0 : ( ProtoScreen.desktopWidth - ProtoScreen.availableWidth)
//    height: ProtoScreen.availableHeight
//            (Device.productType === "windows") ? 0 : ( ProtoScreen.desktopHeight - ProtoScreen.availableHeight)

//    visibility: "FullScreen"
    maximumHeight: ProtoScreen.availableHeight
    maximumWidth: ProtoScreen.availableWidth

    property string theme: "Material"
    property string  uname
    property string  err
    property string currentTeamInFocus
    property string currentHomeTeam
    property string currentAwayTeam
    property string helperTxt
    property string helperHeader: "Help"

    property string msgString

    property string version: "0.1" //TODO

    property string playerInView

    // make this into c++
    property var speed2Name: function( bytesPerSecond ){
        if ( bytesPerSecond < 1000 )
            return bytesPerSecond + " B";
        if ( bytesPerSecond < 1024000 )
            return Math.round( bytesPerSecond / 1024 * 100 ) / 100 + " Kb"
        if ( bytesPerSecond < 1048576000 )
            return Math.round( bytesPerSecond / 1048576 * 100 ) / 100 + " Mb"
        else
            return Math.round( bytesPerSecond / 1073741824 * 100 ) / 100 + " Gb"
    }
    onThemeChanged:
        if(theme === "Material") {
            newLoader.source = "qrc:/MaterialMain.qml"
        }else{
            newLoader.source = "qrc:/"+theme+".qml"
        }
    visible: false
    Loader{
        id: newLoader
        width: realRoot.width
        height: realRoot.height
        anchors.fill: parent
        Component.onCompleted: {
            if(theme === "Material") {
                source = "qrc:/MaterialMain.qml"
            }else{
                source = "qrc:/"+theme+".qml"
            }
        }
    }

//    function getNames(){
//        var nL = MiddleMan.allNamesList()
//        for (var i = 0 ; i < nL.length ;i++ )
//        {
//            realRoot.leaders.append({"fName":nL[i], "balance": 0})
//        }
//    }


//    Button {
//        iconName: "av/play_arrow"
//        name: "Play"
//        onTriggered: {
//            for ( var i = 0; i < torrentModel.count; ++i ){
//                var res = torrentModel.data( i, TorrentModel.TorState );
//                if ( res == TorrentModel.Paused )
//                    torrentModel.setPause( i, false )
//            }
//        }
//    },

//    Button {
//        iconName: "av/pause"
//        name: "Pause"
//        onTriggered: {
//            for ( var i = 0; i < torrentModel.count; ++i ){
//                var res = torrentModel.data( i, TorrentModel.TorState );
//                if ( res == TorrentModel.Downloading || res == TorrentModel.Seeding )
//                    torrentModel.setPause( i, true )
//            }
//        }
//    }
//    Button{
//        id : addTorrentAction
//        name: "New Torrent"
//        iconName: "action/note_add"
//        onTriggered: addTorrentDialog.open()
//    }


//    AddTorrentDialog{
//        id : addTorrentDialog
//        onAccepted: torModel.addTorrent( torrentFileUrl, destinationFolder )
//    }

//    TorrentModel{
//        id: torModel
//        onCountChanged: console.log(count)
//    }

}
