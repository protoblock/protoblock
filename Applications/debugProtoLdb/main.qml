import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import ProRotoQml.Models 1.0
import ProRotoQml.Backend 1.0
import ProRotoQml.Theme 1.0

import "Utils.js" as Utils
import "Common"
Window {
    id: window
    visible: true
    width: Screen.width
    height: Screen.height -10 ;
    color: Colors.white

    property string  currentBlock
    property string headerTexts
    property string currentHash
    property string currentType
    property variant lastHash:[]
    property string lastMessage
    onLastMessageChanged: {
        console.log(lastMessage)
    }

    property variant leaves: []
    property bool  isLeaves: false
    ListModel{id: treeModel}


    Component.onCompleted: {
        comboTimer.start()
    }

    Rectangle{
        width: window.width
        height: window.height
        color:"transparent"
        gradient: Gradient{
            GradientStop{position: 1; color: "#44FFFFFF"}
            GradientStop{position: 0; color: "#66000000"}
        }





        // CONTENT
        SplitView{
            id: mainView
            orientation: Qt.Horizontal
            height: window.height - (topBar.height + header.height + 4 )
            width: parent.width
            anchors{
                left: parent.left
                leftMargin: 12
                right: parent.right
                rightMargin: 12
                bottom: parent.bottom
            }

            // HISTORY
            Rectangle{
                id: historyView
                width: parent.width / 5
                height: parent.height
                color: "transparent"
                radius: 5
                border{width: 3; color: "#77000000"}
                gradient: Gradient{
                    GradientStop{position: 1; color: "#44FFFFFF"}
                    GradientStop{position: 0; color: "#66000000"}
                }
                Rectangle{
                    id: clearRec
                    width: parent.width
                    height:    clearButton.height + 20
                    color: "#33FFFFFF"

                    Button{
                        id: clearButton
                        width: parent.width / 1.07
                        anchors.centerIn: parent
                        text: "Clear"
                        onClicked: {
                            treeModel.clear()
                        }
                    }
                }
                ListView {
                    width: parent.width
                    height: parent.height - (clearButton.height+5)
                    anchors.top: clearRec.bottom
                    anchors.topMargin: 15
                    spacing: 10
                    model: treeModel
                    delegate:
                        Card{
                        width: parent.width / 1.07
                        height: 24
                        anchors.horizontalCenter: parent.horizontalCenter
                        elevation: 2
                        radius: 5
                        Text {
                            id: theInfo
                            text: message
                            anchors{
                                margins: 10
                                fill: parent
                            }
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            color: Colors.black
                            wrapMode: Text.WrapAnywhere
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                var isMerk = message.substring(message.lastIndexOf("->")+2)
                                if (isMerk !== "MerkleTree"){
                                    treeModel.append({ 'message' : message , 'hash' : hash })
                                    drillModel.clear()
                                    key.text = hash
                                    stye.currentText = message
                                    LdbHashReaderTool.getDebugStr(hash,message)
                                    lastMessage = message
                                }else{
                                    //                        treeModel.append({ 'message' : message , 'hash' : hash })
                                    drillModel.clear()
                                    key.text = hash
                                    stye.currentText = isMerk
                                    LdbHashReaderTool.getDebugStr(hash,isMerk)
                                    lastHash = isMerk
                                }
                            }
                        }
                    }
                }
            }//HISTORY

            //NEXT
            Rectangle{
                id: nextRec
                width: parent.width / 3.2
                height:  clearButton.height
                color: "transparent"
                radius: 5
                border{width: 3; color: "#77000000"}
                gradient: Gradient{
                    GradientStop{position: 0; color: "#44FFFFFF"}
                    GradientStop{position: 1; color: "#66000000"}
                }
                TextField{
                    id: searchBar
                    width: parent.width / 1.07
                    anchors.horizontalCenter: parent.horizontalCenter
                    placeholderText: "Search .... "
                    anchors.top: parent.top
                    anchors.topMargin: 10
                }
                ScrollView{
                    id: scrollDebugStr
                    width: parent.width
                    height:     parent.height - ( searchBar.height + 10)
                    anchors.top: searchBar.bottom
                    anchors.topMargin: 10


                    Behavior on x{NumberAnimation{duration: 1200 ; easing.type: Easing.OutBack}}
                    GridView {
                        id: debugstr
                        width: scrollDebugStr.width
                        anchors.margins: 20
                        anchors.centerIn: parent
                        height: parent.height
                        cellWidth:  width /3
                        cellHeight: height / 4
                        remove: Transition {
                            ParallelAnimation{
                                NumberAnimation { property: "opacity";from: 1 ; to: 0; duration: 1200 }
                                NumberAnimation{property: "scale";from: 1; to:0; duration:1200; easing.type: Easing.InBack}
                            }
                        }
                       add: Transition {
                            ParallelAnimation{
                                NumberAnimation { property: "opacity";from:0; to: 1; duration: 1200 }
                                NumberAnimation{property: "scale";from:0; to:1; duration:1200; easing.type: Easing.OutBack}
                            }
                        }
                        model: SortModel {
                            source: drillModel
                            sortCaseSensitivity: Qt.CaseSensitive
                            filterString: "*" + searchBar.text + "*"
                            filterSyntax: SortModel.Wildcard
                            filterCaseSensitivity: Qt.CaseSensitive
                        }

                        delegate: Card{
                            id: nextDel
                            elevation: 0
                            width: debugstr.cellWidth - 10
                            height: debugstr.cellHeight -10
                            backgroundColor:  theValue.length  > 40 ? Colors.blueGrey : Colors.green
                            radius: theValue.length  > 40 ? 5 : 360
                            Behavior on scale{NumberAnimation{duration: 1200 ; easing.type: Easing.OutBack}}
                            Component.onCompleted: {
                                leaves.length = 0
                                for (var i = 0 ; i < drillModel.count ; i ++){
                                    var checkStr = drillModel.get(i).theKey
                                    if (checkStr === "leaves"){
                                        leaves.push(drillModel.get(i).theValue)
                                    }
                                }
                            }

                            Text {
                                id: thesInfo
                                text: {
                                    if (theValue.length  < 40 ){
                                        "<b>" +  theKey  +" </b> <br />"
                                                + theValue
                                    }
                                    else if (theValue.length  > 40){
                                        "<b>"+ theKey + "</b>"
                                    }
                                }

                                anchors{
                                    margins: 10
                                    fill: parent
                                }
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                color: Colors.white
                                wrapMode: Text.WrapAnywhere
                            }

                            MouseArea{
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: {
                                    if( theValue.length > 40 ){
                                        nextDel.elevation = nextDel.elevation + 10
                                        nextDel.scale = nextDel.scale + .03
                                    }
                                }
                                onExited: {
                                    nextDel.elevation = 0
                                    nextDel.scale = 1
                                }




                                onClicked:{
                                    if (theValue.length > 40 ){
                                        var k =   Utils.getTypeFromText(theKey)
                                        var v = theValue.replace(/"/g, "");
                                        if (theKey === "prev"){
                                            drillModel.clear()
                                            LdbHashReaderTool.getDebugStr(v,stye.currentText)
                                            lastMessage = theKey
                                            return;
                                        }else if (theKey !== 'prev'){
                                            if (k === "MerkleTree"){
                                                treeModel.append({ 'message' : theKey +"->"+ k , 'hash' : v })
                                                lastMessage =  " MERKLE  " +  theKey


                                                // THIS RETURNS THE MERKEL TYPE
                                                var theNextMerkle = Utils.getMerkleFromText(theKey)

                                                drillModel.clear()
                                                key.text = v
                                                stye.currentText = k
                                                LdbHashReaderTool.getDebugStr(v,k)
                                                stye.currentText =  theNextMerkle
                                            }
                                            else if (k !== "MerkleTree")
                                            {
                                                lastMessage = theKey
                                                drillModel.clear()
                                                treeModel.append({ 'message' :  k , 'hash' : v })
                                                key.text = v
                                                stye.currentText = k
                                                LdbHashReaderTool.getDebugStr(v,k)


                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }//GRID-NEXT
            }//NEXT


            Rectangle{
                color: "transparent"
                radius: 5
                width: window.width - (nextRec.width - historyView)
                height: nextRec.height
                border{width: 3; color: "#77000000"}
                gradient: Gradient{
                    GradientStop{position: 0; color: "#44FFFFFF"}
                    GradientStop{position: 1; color: "#66000000"}
                }
                DebugLog{
                    id: tArea
                    anchors{
                        fill: parent
                        margins: 20
                        centerIn: parent
                    }
                    Behavior on x{NumberAnimation{duration: 1200 ; easing.type: Easing.OutBack}}
                }
            }
        }//END OF CONTANT




        TopBar{
            id: header
            headerText: window.headerTexts
            model: historyModel
        }

        Rectangle{
            id: topBar
            width: parent.width
            height:  header.height
            color: "transparent"
            anchors{
                top: header.bottom
            }
            Column{
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5
                TextField {
                    id: key
                    width: parent.width / 1.07
                    height:     parent.height / 2
                    text: "blockhead"
                    placeholderText: "hashid key"
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                    }
                    onAccepted:{
                        aboutToChange( currentHash , currentType )
                        currentHash = key.text
                        currentType = stye.currentText
                        LdbHashReaderTool.getDebugStr( currentHash , currentType )
                        lastMessage = currentType
                    }
//                    onTextChanged: {
//                        var someText = s
////                        tye.currentText
//                                     tArea.append("\n\n TEXT AREA CHANGED  \n\n"
//                                     + "\n ComboBox:   " + someText
//                                     +  "\n\nHash TextArea Changed:  "+ text
//                                     +"\n --------------------------------------------------------------------------------\n"
//                                     )
//                    }
                }
                Row{
                    width: parent.width / 1.07
                    height:  parent.height / 2.2
                    spacing: 10
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                    }
                    SearchBar {
                        id: stye
                        width: parent.width / 2
                        height: 32
                        focus: false
                        onCurrentTextChanged: {
                            currentType = currentText
                            window.headerTexts= currentText
                        }
                    }
                    SimpleButton {
                        id: butt
                        width: parent.width / 2
                        text: "Read Hash"
                        textColor: "#333333"
                        borderWidth: 1
                        height: 32
                        glowColor: "#22333333"
                        glowEnabledByMouse: true
                        onClicked:{
                            currentHash = key.text
                            currentType = stye.currentText
                            LdbHashReaderTool.getDebugStr(currentHash, currentType)
                            lastMessage = currentType
                        }
                    }
                }
            }
        }


        ListModel{id: historyModel}
        ListModel{id: drillModel ;onRowsAboutToBeRemoved:  searchBar.text = ""}


        BlockMeta {
            id: bmeta
            Component.onCompleted: {
                console.log( "Total Blocks " + blocknum )
            }

        }

        //FIXME add a progession indicator and a background splash

        Connections {
            target: LdbHashReaderTool
            onGotDebugStr: {
                tArea.append("\n--------------------------------------------------------------------------------\n")
                tArea.append(stye.currentText)
                tArea.append("\n")
                tArea.append(LdbHashReaderTool.debugstring)

                if(isLeaves !== true ){
                    drillModel.clear();
                    var sToArray = LdbHashReaderTool.debugstring.split("\n")
                    for (var i = 0 ; i < sToArray.length ; i++ ){

                        var sToMapArray = sToArray[i].split(": ");
                        if (sToMapArray.length === 1){
                            drillModel.append({
                                                  "theKey" : sToMapArray[0].replace(/}|{/g,""),
                                                  "theValue" : sToMapArray[0].replace(/}|{/g,"")

                                              })
                        }else{
                            drillModel.append({
                                                  "theKey" : sToMapArray[0].replace(/}|{/g,""),
                                                  "theValue" : sToMapArray[1].replace(/}|{/g,"")

                                              })
                        }
                    }
                    sToArray.length = 0
                    sToMapArray.length = 0
                    // Next leaves
                }else{
                    var sToArray = LdbHashReaderTool.debugstring.split("\n")
                    for (var i = 0 ; i < sToArray.length ; i++ ){

                        var sToMapArray = sToArray[i].split(": ");
                        if (sToMapArray.length === 1){
                            drillModel.append({
                                                  "theKey" : sToMapArray[0].replace(/{|}/g,""),
                                                  "theValue" : "",  // sToMapArray[0].replace(/}|{/g,""),
                                                  "all": LdbHashReaderTool.debugstring
                                              })
                        }else{
                            drillModel.append({
                                                  "theKey" : sToMapArray[0].replace(/}|{/g,""),
                                                  "theValue" : sToMapArray[1].replace(/{|}/g,""),
                                                  "all": LdbHashReaderTool.debugstring
                                              })
                        }
                    }
                    drillModel.append({
                                          'theKey' :  "END OF Merel"
                                          ,'The Value' :  'Plop'
                                      })
                    sToArray.length = 0
                    sToMapArray.length = 0
                }
            }
        }

        // racer on the ComboBox
        Timer{
            id: comboTimer
            interval: 200
            running: false
            repeat: false
            onTriggered: {
                drillModel.clear()
                isLeaves = false
                stye.currentText = "pbstate"
                currentHash = LdbHashReaderTool.jumpToPbStateId()
                key.text = currentHash
                window.currentType = stye.currentText
                LdbHashReaderTool.getDebugStr(currentHash,currentType)
                lastMessage = stye.currentText
                treeModel.append({ 'message' : currentType , 'hash' : currentHash })
            }
        }

    }
}
