import QtQuick 2.4
import ProRotoQml.Theme 1.0
import QtQuick.Controls 1.4
import Material 1.0
Item{
    id: playersCard
    property int  playerid
    property string roster_status: ""
    property string photoUrl: ""
    property string team: ""
    property string first: ""
    property string last: ""
    property string birthDateString: ""
    property string age: ""
    property string position: ""
    property string playersHeight: ""
    property string playersWeight: ""
    property string college: ""
    property string collegeDraftYear: ""
    property string collegeDraftRound: ""
    property string collegeDraftPick: ""
    property string collegeDraftTeam: ""
    property string experience: ""

    // for markets
    property int bid
    property int bidSize
    property int ask
    property int askSize
    property int volume
    property int change
    property int lastPrice


    property int cellWidth
    property int cellHeight

    Card{
        width: playersCard.cellWidth / 1.07
        height:  playersCard.cellHeight / 1.07
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
                opacity: roster_status === "" ? 0 :1
                color: roster_status === "A" ? "green" : "red"
                anchors.verticalCenter: parent.verticalCenter
            }
            Text{
                height:    parent.height
                text: roster_status === "A"  ?  "Active"  :  "InActive"
                color: roster_status === "A" ? Colors.blueGrey : "red"
                font.pixelSize: 28
                opacity: roster_status === "" ? 0 :1
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
                font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
                text:  lastPrice
            }
            Text {
                text: volume
                color: "white"
                font.pixelSize:ProtoScreen.font(ProtoScreen.LARGE )
            }
            Text {
                text: change
                color: "white"
                font.pixelSize:ProtoScreen.font(ProtoScreen.LARGE )
            }
            Text {
                text: qsTr("Last")
                color: "white"
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            }
            Text {
                text: qsTr("Volume")
                color: "white"
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            }
            Text {
                text: qsTr("Change")
                color: "white"
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
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
                source: photoUrl
                width: height
                height:  ProtoScreen.guToPx(6)
                anchors.centerIn: parent
            }
        }
        Rectangle{
            id: teamPhoto
            width: height
            height:  ProtoScreen.guToPx(8)
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
                    var j = team
                    var k  = j + ".PNG"
                    k
                }
                smooth: true
                width: height
                height:  ProtoScreen.guToPx(12.3)
                anchors.centerIn: parent
            }
        }

        Column{
            width: parent.width / 2 - (parent.width / 37)
            height:    parent.height / 2 - (playerPhoto.height/ 3.6)
            spacing: 0
            anchors.bottom:  parent.bottom
            anchors.bottomMargin: ProtoScreen.guToPx(2)
            anchors.left: parent.left
            anchors.leftMargin: parent.width / 37
            Text {
                id: currentName
                text: first + " " + last
                color: Palette.text
                font.bold: true
                font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
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
                    text: position
                    color: Colors.blueGrey
                    visible: position === "" ? false: true
                }
                Rectangle{ width: 1; height: parent.height/1.7; color: Colors.grey ;opacity: .8;anchors.verticalCenter: parent.verticalCenter }

                Text {
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    text: playersHeight+ ", " + playersWeight + " lbs"
                    color: Colors.blueGrey
                    visible: playersHeight === "" ? false: true
                }
                Rectangle{ width: 1; height: parent.height/1.7; color: Colors.grey ;opacity: .8;anchors.verticalCenter: parent.verticalCenter }
                Text {
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    text: team
                    color: Colors.blueGrey
                    visible: team === "" ? false: true
                }
            }
            Text {
                text: "BirthDateString " + "(Age: "  + age + ")"
                color: Colors.blueGrey
                visible: age === "" ? false: true
            }
            Text {
                text: "Drafted " +collegeDraftYear + " Rnd: " + collegeDraftRound + ",  " + collegeDraftPick +" By "+ collegeDraftTeam
                visible: collegeDraftYear === "" ? false: true
                color: Colors.blueGrey
            }
            Text{
                text: "Year In League: " + experience
                color: Colors.blueGrey
                visible: experience === "" ? false: true
            }
            Text{
                text: "College: " + college
                visible: college === "" ? false: true
                color: Colors.blueGrey
            }
        }
    }
}
