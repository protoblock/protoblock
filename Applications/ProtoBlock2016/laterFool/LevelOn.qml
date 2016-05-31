import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import ProRotoQml.Models 1.0
import ProRotoQml.Backend 1.0
import ProRotoQml.Theme 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItem

import "Utils.js"as Utils
Item{
    id: sGamesPage
    Rectangle{
        anchors.fill: parent
        color: "transparent"
        Row{
            id: weekPickerRow
            width: parent.width / 1.07
            height: weekPicker.height + 10
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            Label{
                width: parent.width - weekPicker.width
                height:   parent.height
                verticalAlignment: Text.AlignVCenter
                text: "Week"
                font.bold: true
            }
            MenuField{
                id: weekPicker
                width: parent.width / 1.07
                model: Utils.weeks
                selectedIndex:  0
                onSelectedIndexChanged:  {
                    demo.weekInFocus =  selectedIndex +1
                }
            }
        }


        Card{
            id: scheduledGamesView
            width: parent.width / 4
            height: parent.height - (weekPicker.height + 10 )
            elevation:  searchGames.focus ? 5 : 0
            anchors.top: weekPickerRow.bottom
            anchors.left: parent.left
            anchors.leftMargin: 5
            TextField {
                id: searchGames
                placeholderText: "Search Games"
                inputMethodHints: Qt.ImhNoPredictiveText
                width: parent.width / 1.07
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 5
            }

            ListView {
                id: scheduledG
                width: parent.width / 1.03
                height: parent.height  - (searchGames.height + 10 )
                anchors.top: searchGames.bottom;
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 2
                clip: true
                model: SortModel {
                    source: scheduledGamesModel
                    sortCaseSensitivity: Qt.CaseInsensitive
                    filterString: "*" + searchGames.text + "*"
                    filterSyntax: SortModel.Wildcard
                    filterCaseSensitivity: Qt.CaseInsensitive
                   Component.onCompleted:  sortAgain('time', Qt.AscendingOrder)
                }
                delegate: ListItem.Game {
                    width: scheduledG.width / 1.07
                    text:  model.homeTeam + " VS " + model.awayTeam
                    subText: model.time
                    secondaryItem: Image {
                        height:  dp(32)
                        width: height
                        source: Utils.getTeamIcon(awayTeam)
                    }
                    action: Image {
                        height:  dp(32)
                        width: height
                        source: Utils.getTeamIcon(homeTeam)
                    }
                    onClicked:  demo.gameId = model.gameId
                }
            }
        }

        Card{
            width: parent.width - (scheduledGamesView.width + 20 )
            height: scheduledGamesView.height
            anchors.top: scheduledGamesView.top
            anchors.left: scheduledGamesView.right
            anchors.leftMargin: 5
            elevation:  searchBox.focus ? 5 : 0
            ListItem.Standard{
                id: statsViewHeader
                width: parent.width
                height: parent.height / 8
                elevation: 1
                textColor: "white"
                text: Backend.getTeamsByGameId(demo.gameId)
                backgroundColor: Theme.primaryColor
            }

            TextField {
                id: searchBox
                placeholderText: "Search..."
                inputMethodHints: Qt.ImhNoPredictiveText
                width: parent.width / 1.07
                anchors.top: statsViewHeader.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }
            TableView{
                id: statsView
                frameVisible: false
                sortIndicatorVisible: true
                sortIndicatorOrder: Qt.AscendingOrder
                onSortIndicatorColumnChanged: {
                    proxyModel.sortAgain(getColumn(sortIndicatorColumn).role, Qt.DescendingOrder)
                }
                width: parent.width / 1.07
                height: parent.height - (statsViewHeader.height + searchBox.height + 25 )
                anchors.top: searchBox.bottom;
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
                model: SortModel {
                    id: proxyModel
                    source: playerStatsModel.count() > 0 ? playerStatsModel : null

                    sortOrder: statsView.sortIndicatorOrder
                    sortCaseSensitivity: Qt.CaseInsensitive
                    sortRole: statsView.count > 0 ? statsView.getColumn(statsView.sortIndicatorColumn).role : ""

                    filterString: "*" + searchBox.text + "*"
                    filterSyntax: SortModel.Wildcard
                    filterCaseSensitivity: Qt.CaseInsensitive
                }


                style: TableViewStyle{
                    frame: Rectangle{border.color: "#333"}
                    headerDelegate:Rectangle{
                        color:  Theme.primaryColor
                        height: textItem.implicitHeight * 1.2
                        border.color: "#333"
                        Label{
                            id: textItem
                            text: styleData.value
                            width: parent.width
                            height: parent.height
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            color: "white"
                        }
                    }
                }

                TableViewColumn{
                    title: "Players Name"
                    role: "playerName"
                }
                TableViewColumn{
                    title: "Pass Yards"
                    role:  "passyds"
                }
                TableViewColumn{
                    title: "Pass TD"
                    role:  "passtd"
                }
                TableViewColumn{
                    title: "Rush Yards"
                    role:  "rushyds"
                }
                TableViewColumn{
                    title: "Rush TD"
                    role:  "rushtd"
                }
                TableViewColumn{
                    title: "Rec"
                    role:  "rec"
                }
                TableViewColumn{
                    title: "Rec Yards"
                    role:  "recyds"
                }
                TableViewColumn{
                    title: "Rec TD"
                    role:  "rectd"
                }
            }
        }


        PlayerStatsModel{
            id: playerStatsModel
            gameId:{
                var gId = demo.gameId
                var g = gId.toString();
                g
            }
        }
        ScheduledGamesModel{
            id: scheduledGamesModel
            week: demo.weekInFocus
        }
    }
}
