import QtQuick 2.4
import ProRotoQml.Backend 1.0
import ProRotoQml.Theme 1.0

import "Common"
Item {
    id: player
    title: "Player Browser"
    width: 600
    height: 300
    property variant model
    property int playerId
    Card{
        width: parent.width / 1.07
        height:  parent.height / 1.07
        elevation: 06
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
                text: playerStatus === "Active"  ?  "Active"  :  "InActive"
                color: playerStatus === "Active" ? Colors.blueGrey : "red"
                font.pixelSize: 28
                verticalAlignment: Text.AlignVCenter
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
                    var j = team.toLowerCase()
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
    }
}
