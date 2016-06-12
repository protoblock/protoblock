import QtQuick 2.4

import Material 1.0
import Material.ListItems 1.0 as ListItems

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
Item {
    Component.onCompleted: {
         if ( !root.reloadhome )
              root.reloadhome = true
          else
              MiddleMan.allNamesGet()
    }
    Image {
        id: logo
        source: "qrc:/logoFinal.png"
        fillMode: Image.PreserveAspectFit
        width: parent.width / 1.07
        height: parent.height / 6
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height / 22
    }
    Label {
        id: welcomeTxt
        anchors.top: logo.bottom
        anchors.topMargin:  paintedHeight
        width: parent.width / 1.07
        font.pixelSize: Qt.platform.os === "android" ? 32 : 22
        font.family: "Roboto"
        color: realRoot.theme ===  "Pinky" ? "white" : "black"
        horizontalAlignment: Text.AlignHCenter
        text: "Welcome  To Protoblock"
        wrapMode: Text.WordWrap
    }
    Column{
        id:buttons
        width: parent.width
        height: parent.height - (welcomeTxt.paintedHeight - logo.height)
        spacing: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: welcomeTxt.bottom
        anchors.topMargin: 10

        Label {
            width: parent.width / 1.07
            font.pixelSize: Qt.platform.os === "android" ? 32 : 22
            font.family: "Roboto"
            horizontalAlignment: Text.AlignHCenter
            text: "Tell your Friends"
            color: realRoot.theme ===  "Pinky" ? "white" : "black"
            wrapMode: Text.WordWrap
        }


        Card{
            width: parent.width / 1.07
            height:     parent.height / 2
            elevation: 1
            anchors.horizontalCenter: parent.horizontalCenter
            Banner{
                id: ban
                text: "LeaderBoard"
                color: "white"
                backgroundColor: realRoot.theme ===  "Pinky" ? "black" : root.theme.primaryColor
                width: parent.width
                height: 48
            }
            ListView{
                id: leaderboard
                width: parent.width - 5
                anchors.top: ban.bottom
                height: parent.height - ban.height
                clip: true
                model:   MiddleMan.allNamesList()
                delegate:
                    ListItems.Subtitled{
                    elevation: 2
                    width: parent.width
                    text: "FantasyName: " +  modelData
                    action: Image{
                        height: parent.height
                        width : height
                        fillMode: Image.PreserveAspectFit
                        source:  "qrc:/icons/action_account_circle.png"
                    }
                    valueText: "Balance : 0"
                }
            }

            Scrollbar{flickableItem: leaderboard }
            ProgressCircle {
                id: fNameInd
                anchors.centerIn: leaderboard
                visible: MiddleMan.fetchingLeaders  === true ?  true : false
            }
        }
    }

    Connections {
        target: MiddleMan
        onLeaderBoardchanged: {
            leaderboard.model = MiddleMan.allNamesList()
        }
    }
}
