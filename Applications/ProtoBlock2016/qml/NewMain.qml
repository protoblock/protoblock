import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2

import ProRotoQml.Utils 1.0
import ProRotoQml.Protoblock 1.0
import ProRotoQml.Torrent 1.0
import Material 1.0 as Material
//import Material.Componets 0.3
//import Material.Dialog 0.1
Window {
    id: realRoot
    title: "Protoblock 2016 "
    width: Device.productType === "osx"||Device.productType === "win32" ? 1200  :  Screen.width
    height: Device.productType === "osx"||Device.productType === "win32" ? 1220  :  Screen.height
    property string theme: "Material"
    property string  uname
    onUnameChanged: console.log("USING NEW UNAME " + uname)
    property string  err
    property string currentTeamInFocus
    property string currentHomeTeam
    property string currentAwayTeam
    //    property string uname: "NULL"
//    property variant leaders: ListModel{}
    property string msgString

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
            newLoader.source = "qrc:/main.qml"
        }else{
            newLoader.source = "qrc:/"+theme+".qml"
        }
    visible: false
    Loader{
        id: newLoader
        anchors.fill: parent
        Component.onCompleted: {
            if(theme === "Material") {
                source = "qrc:/main.qml"
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


    AddTorrentDialog{
        id : addTorrentDialog
        onAccepted: torModel.addTorrent( torrentFileUrl, destinationFolder )
    }

    TorrentModel{
        id: torModel
        onCountChanged: console.log(count)
    }
}
