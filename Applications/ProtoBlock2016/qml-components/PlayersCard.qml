import QtQuick 2.4
//import ProRotoQml.Backend 1.0
import ProRotoQml.Theme 1.0
import Material 1.0

Item {
    id: player
    width: 20
    height: 20
    property bool buy: false
    property bool sLong: false
//    property variant model
//    property int playerId

//    ListView{
//        width: player.width
//        height: player.height
//        model: player.model
//        delegate:
            Card{
            width: player.width / 1.07
            height:  player.height / 1.07
            elevation: 10
            Rectangle{
                id: playerTeamColor
                width: parent.width
                height: parent.height / 2.4
                color: "#9B2743"
                gradient: Gradient{
                    GradientStop{position: 1; color: TeamInfo.getPrimaryAt(model.Team) }
                    GradientStop{position: 0; color: TeamInfo.getSecondaryAt(model.Team) }
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
                    color: model.Active === "true" ? "green" : "red"
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text{
                    height: parent.height
                    text: model.Active === "true"  ?  "Active"  :  "InActive"
                    color: model.Active === "true" ? Colors.blueGrey : "red"
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
                Image{
                    source: model.PhotoUrl
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
                        "qrc:/"+model.Team+".PNG"
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
                    id: playersName
                    text: model.FirstName + " " + model.LastName
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
                    height:  playersName.paintedHeight
                    spacing: 10
                    Text{
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        text: "#"+Number +"  " + model.Position
                        color: Colors.blueGrey
                    }
                    Rectangle{ width: 1; height: parent.height/1.7; color: Colors.grey ;opacity: .8;anchors.verticalCenter: parent.verticalCenter }

                    Text {
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        text: model.Height + ", " + model.Weight + " lbs"
                        color: Colors.blueGrey
                    }
                    Rectangle{ width: 1; height: parent.height/1.7; color: Colors.grey ;opacity: .8;anchors.verticalCenter: parent.verticalCenter }
                    Text {
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        text: model.Team
                        color: Colors.blueGrey
                    }
                }
                Text {
                    text: model.BirthDateString + "  (Age: "+ model.Age+")"
                    color: Colors.blueGrey
                }
                Text {
                    text: "Drafted " +model.CollegeDraftYear + " Rnd: " + model.CollegeDraftRound + ",  " + model.CollegeDraftPick +" By "+ model.CollegeDraftTeam
                    color: Colors.blueGrey
                }
                Text{
                    text: "Year In League: " + model.Experience
                    color: Colors.blueGrey
                }
                Text{
                    text: "College: " + model.College
                    color: Colors.blueGrey
                }
            }
        }
//    }



      TradingCard{
        id: tradingCard
        width:  playerTeamColor.width / 1.3
        height:  playerTeamColor.height
        anchors.horizontalCenter: parent.horizontalCenter
        playerName: playersName.text
        buying: buy
        seaesonLong: sLong
      }
}





