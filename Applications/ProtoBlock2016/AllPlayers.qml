import QtQuick 2.4
import ProRotoQml.Backend 1.0
import ProRotoQml.Theme 1.0
import "qrc:/Common/"
import QtQuick.Controls 1.4
import "Common"
PageHelper{
    title: "Player Browser"
    width: rootLoader.width
    height: rootLoader.height

    property int bidSize
    property int playerId

    BackendApi{}
    object:
        Rectangle{
        width: rootLoader.width
        height: rootLoader.height
        color: "transparent"
        ComboBox{
            id: posPicker
            width: parent.width / 1.07 / 3
            model: 23
            anchors.right: parent.right
            onCurrentIndexChanged: {

            }
        }
        ComboBox{
            id: teampicker
            width: parent.width / 1.07 / 3
            model: 23
            onCurrentIndexChanged: {

            }
        }
        GridView{
            id: playerCard
            anchors.top: teampicker.bottom
            width: parent.width - teampicker.height
            height: parent.height
            clip: false
            onCountChanged: console.log(count)
            cellWidth:  parent.width / 3.2
            cellHeight:  parent.height / 1.8
            model: playerModel
            anchors.horizontalCenter: parent.horizontalCenter
            Component.onCompleted: console.log(count)
            delegate:
                Card{
                width: playerCard.cellWidth / 1.07
                height:  playerCard.cellHeight / 1.07
                elevation: 06
                property int  playerId

                Rectangle{
                    id: playerTeamColor
                    width: parent.width
                    height: parent.height / 2.4
                    color: "#9B2743"
                    gradient: Gradient{
                        GradientStop{position: 1; color: TeamInfo.getPrimaryAt(team) }
                        GradientStop{position: 0; color: TeamInfo.getSecondaryAt(team) }
                    }
                }



                Row{
                    height: 40
                    spacing: 5
                    anchors{
                        bottom: parent.bottom
                        bottomMargin: parent.height / 2
                        right: parent.right
                        rightMargin: parent.width / 33
                    }
                        Rectangle{
                        width: 24
                        height: width
                        radius: 360
                        color: playerStatus === "Active" ? "green" : "red"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Text{
                        height:    parent.height
                        text: playerStatus // === "A"  ?  "Active"  :  "InActive"
                        color: playerStatus === "Active" ? Colors.blueGrey : "red"
                        font.pixelSize: 28
                        verticalAlignment: Text.AlignVCenter
                    }
                }





                // New component look upfrom md1
                    Grid{
                        width: parent.width / 3
                        height: parent.height / 2.2
                        columns: 3
                        rows: 2
                        spacing: 10
                        horizontalItemAlignment: Grid.AlignHCenter
                        verticalItemAlignment: Grid.AlignVCenter
                        anchors{
                            top: parent.top
                            topMargin: 10
                            left: parent.left
                            leftMargin: parent.width / 33
                        }
                        Text {
                            color: "white"
                            font.pixelSize: Unit.dp(38)
                            //                            function getSingleInfo(id,tName){
                            Component.onCompleted:  {
                                var t =getSingleInfo(PlayerId , "price")
                                if ( t=== ""){
                                    text = "0"
                                }else {
                                    text = t
                                }
                            }
                        }
                        Text {
                            text: qsTr("9")
                            color: "white"
                            font.pixelSize: Unit.dp(38)
                            Component.onCompleted:  {
                                var t =getSingleInfo(PlayerId , "volume")
                                if ( t=== ""){
                                    text = "0"
                                }else {
                                    text = t
                                }
                            }
                        }
                        Text {
                            text: qsTr("9")
                            color: "white"
                            font.pixelSize: Unit.dp(38)
                            Component.onCompleted:  {
                                var t =getSingleInfo(PlayerID , "change")
                                if ( t=== ""){
                                    text = "0"
                                }else {
                                    text = t
                                }
                            }
                        }
                        Text {
                            text: qsTr("price")
                            color: "white"
                            font.pixelSize: Unit.dp(10)
                        }
                        Text {
                            text: qsTr("volume")
                            color: "white"
                            font.pixelSize: Unit.dp(10)
                        }
                        Text {
                            text: qsTr("change")
                            color: "white"
                            font.pixelSize: Unit.dp(10)
                        }
                    }

                // The Players card
                Rectangle{
                    id: playerPhoto
                    width: height
                    height:  100
                    color: "transparent"
                    anchors{
                        top: playerTeamColor.bottom
                        topMargin: -(height / 2)
                        left: parent.left
                        leftMargin: parent.width / 17
                    }
                    RoundImage{
                        source: PhotoUrl
                        width: height
                        height:  100
                        anchors.centerIn: parent
                    }
                }
                Rectangle{
                    id: teamPhoto
                    width: height
                    height:  100
                    radius: 20
                    color: "transparent"
                    anchors{
                        top: parent.top
                        topMargin: parent.height / 22
                        right: parent.right
                        rightMargin: parent.width / 17
                    }
                    Image{
                        source: {
                            var j = Team.toLowerCase()
                            var k  = j + ".png"
                            k
                        }
                        smooth: true
                        width: height
                        height:  98
                        anchors.centerIn: parent
                    }
                }

                Column{
                    width: parent.width / 2 - (parent.width / 37)
                    height:    parent.height / 2 - (playerPhoto.height/ 3.6)
                    spacing: 0
                    anchors.bottom:  parent.bottom
                    anchors.bottomMargin: Unit.dp(16)
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width / 37
                    Text {
                        id: currentName
                        text: firstName + " " + lastName
                        color: Palette.text
                        font.bold: true
                        font.pixelSize: Unit.dp(32)
                    }
                    Rectangle{
                        height: 2
                        width: parent.width
                        color: Colors.grey
                    }


                    Row{
                        width: parent.width
                        height:  currentName.paintedHeight
                        spacing: 10
                        Text{
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                            text: "#"+Number +"  " + Position
                            color: Colors.blueGrey
                        }
                        Rectangle{ width: 1; height: parent.height/1.7; color: Colors.grey ;opacity: .8;anchors.verticalCenter: parent.verticalCenter }

                        Text {
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                            text: Height + ", " + Weight + " lbs"
                            color: Colors.blueGrey
                        }
                        Rectangle{ width: 1; height: parent.height/1.7; color: Colors.grey ;opacity: .8;anchors.verticalCenter: parent.verticalCenter }
                        Text {
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                            text: team
                            color: Colors.blueGrey
                        }
                    }
                    Text {
                        text: BirthDateString + "  (Age: "+ Age+")"
                        color: Colors.blueGrey
                    }
                    Text {
                        text: "Drafted " +CollegeDraftYear + " Rnd: " + CollegeDraftRound + ",  " + CollegeDraftPick +" By "+ CollegeDraftTeam
                        color: Colors.blueGrey
                    }
                    Text{
                        text: "Year In League: " + Experience
                        color: Colors.blueGrey
                    }
                    Text{
                        text: "College: " + College
                        color: Colors.blueGrey
                    }


                }
                //NEXT
                Card{
                    width: height
                    height: 80
                    backgroundColor: TeamInfo.getPrimaryAt(team)
                    radius: 360
                    elevation: 20
                    anchors{
                        bottom: parent.bottom
                        bottomMargin: parent.height / 15
                        right:  parent.right
                        rightMargin: parent.width / 10
                    }

                    Image{
                        width: height
                        height: 80
                        source: "https://raw.githubusercontent.com/google/material-design-icons/master/av/drawable-xxxhdpi/ic_play_arrow_white_48dp.png"
                        anchors.centerIn: parent
                    }
                }
            }
        }
    }


PlayersModel{
    id: playerModel
Component.onCompleted: {
    console.log("tyo ") ;
    initModel()}
}
}
