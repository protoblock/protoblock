import QtQuick 2.4
import ProRotoQml.Theme 1.0
import ProRotoQml.Backend 1.0

import Material 1.0

Item {
    id: gameDelRoot
    property bool inFocus
    property string awayTeam
    property int awayTeamPoints
    property string homeTeam
    property int homeTeamPoints
    property int week
    property string gameTime
    signal clicked();
    implicitHeight: 64
    //    color: "black"
    Card{
        width: gameDelRoot.width
        height: gameDelRoot.width
        rotation: 90
        border{
            color: "black"
            width: 1
        }

        MouseArea{
            anchors.fill: parent
            onClicked: gameDelRoot.clicked()
        }
    }


    /*!
      *  homeTeam
      */
    Rectangle{
        id: hT
        width:  height
        height: parent.height // / 1.07
        border.color: "black"
        border.width: 3
        anchors.left: parent.left
        anchors.verticalCenter: gameDelRoot.verticalCenter

        gradient: Gradient{
            GradientStop{position: 0;color: TeamInfo.getPrimaryAt(homeTeam)}
            GradientStop{position: 1;color: TeamInfo.getSecondaryAt(homeTeam)}
        }
        Image {
            id: hI
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            source: {
                getTeamIcon(homeTeam)
            }
        }
        Text {
            anchors.left: hI.right
            anchors.leftMargin: 12
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height / 2
            text: homeTeam//TeamMap.getTeamLongName(homeTeam)
            //            color: "white"
            font.bold: true
        }


    }

    /*!
  * Away Team
  */
    Rectangle{
        id: aT
        width:  height
        height: parent.height /// 1.07
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        gradient: Gradient{
            GradientStop{position: 0;color: TeamInfo.getPrimaryAt(awayTeam)}
            GradientStop{position: 1;color: TeamInfo.getSecondaryAt(awayTeam)}
        }
        Image {
            id: aI
            anchors.fill: parent
            source: {
                getTeamIcon(awayTeam)
            }
        }
        Text {
            anchors.right: aI.left
            anchors.rightMargin: 12
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height / 2
            text: awayTeam //TeamMap.getTeamLongName(awayTeam)
            //            color: "white"
            font.bold: true
        }
    }
    Text {
        text: gameStatus //gameTime
        anchors.horizontalCenter: parent.horizontalCenter
        height:     parent.height
        //        color: "white"
        font.pixelSize:12 //parent.width / 2
        verticalAlignment: Text.AlignVCenter
    }
    function getTeamIcon(team)
    {
        var j = team.toString()
        var tt = j.toLowerCase()
        var k  = "qrc:/"+ tt + ".png"
        return k
    }
}
