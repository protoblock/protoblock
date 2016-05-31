import QtQuick 2.5
import ProRotoQml.Backend 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Models 1.0

import Material.ListItems 1.0 as ListItem
import Material 1.0

import "Utils.js" as Utils
import "Common"
Item{
    id: rootLeaderBoard
    TextField{
        id: searchBar
        width: parent.width / 1.07
        anchors.horizontalCenter: parent.horizontalCenter
        placeholderText: "Search .... "
        inputMethodHints: Qt.ImhNoPredictiveText
        text: demo.leaderInView
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    ListView {
        id: welcomeView
        width: parent.width
        height: parent.height - searchBar.height
        anchors.top: searchBar.bottom
        anchors.topMargin: 10
        clip: true
        spacing: 8
        model: SortModel {
            id: sortModel
            source: leaderBoardModel
            sortCaseSensitivity: Qt.CaseInsensitive
            filterString: "*" + searchBar.text + "*"
            filterSyntax: SortModel.Wildcard
            filterCaseSensitivity: Qt.CaseInsensitive
            Component.onCompleted: {
                sortModel.sortAgain('skillBal' , Qt.DescendingOrder)}
        }
        highlightMoveDuration: 200
        delegate: ListItem.Subtitled {
            text: fantasyName
            subText: "<b>State Balance:  </b> " + stateBal  +  "<b>  Skill Balance:  </b> " +skillBal
            onClicked: {
                levelTwoLeader.playersName = fantasyName
                levelTwoLeader.stake = skillBal
                levelTwoLeader.bits = stateBal
                levelTwoLeader.shown = true
            }
            action: Icon {
                anchors.centerIn: parent
                name: "action_account_circle"
                size: dp(32)
            }
        }
        keyNavigationWraps: true
        highlightFollowsCurrentItem: true
        Keys.onReturnPressed: itemClicked(currentIndex)
    }
    LeaderBoardModel{id: leaderBoardModel}




    // LevelTwo of leader board
    Card{
        id: levelTwoLeader
        property bool shown: false
        property string playersName: demo.leaderInView
        property string playersId
        property string stake
        property string bits
        width: parent.width
        height: parent.height
        x: levelTwoLeader.shown ?  0 :  rootLeaderBoard.width
        Behavior on x {NumberAnimation{duration: 1200; easing.type: Easing.OutBack}}
        ListItem.Subtitled{
            id: levelTwoHeader
            width: parent.width
            height: parent.height / 8
            text: "Fantasy Name: " + levelTwoLeader.playersName
            subText: "<b>State Balance:  </b> " + levelTwoLeader.bits  +  "<b>  Skill Balance:  </b> " +levelTwoLeader.stake
            action: Icon {
                anchors.centerIn: parent
                name: "content_forward"
                rotation: 180
                size: dp(32)
            }
            MouseArea{
                anchors.fill: parent
                onClicked: levelTwoLeader.shown = false
            }
        }


        Card{
            id: weeksList
            width: parent.width
            height: levelTwoHeader.height / 2
            //            color: "gr"
            elevation: 1
            anchors.top: levelTwoHeader.bottom
            anchors.topMargin: 5
            Row{
                width: parent.width / 1.07
                height:     parent.height - 5
                anchors.centerIn: parent
                Repeater{
                    id: weekHelper
                    anchors.fill: parent
                    model: Utils.weeks.length
                    Button{
                        width: parent.width / 16
                        text: Utils.weeks[index]
                        elevation: parseInt(text,10)=== awardModel.week ?  5 : 0
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: awardModel.week = parseInt(text,10)
                    }
                }
            }
        }

        ListView{

            width: parent.width
            height:  parent.height - (weeksList.height + levelTwoHeader.height + 10 );
            anchors.top: weeksList.bottom
            anchors.topMargin: 5
            clip: true
            model: awardModel
            delegate: ListItem.Subtitled{
                text: model.playersName
                subText: "Projection "
                         + model.projection
                         + " Week " + model.week + " "
                         + " Game "  + Backend.getTeamsByGameId(model.gameId)
                         + " Award "
                         + model.award
                maximumLineCount: 3
            }
        }

        AwardsModel{
            id: awardModel
            awardType: AwardsModel.FNameAndWeek
            fname: levelTwoLeader.playersName
            week:16
        }
    }// end drill Fantasy Player
}
