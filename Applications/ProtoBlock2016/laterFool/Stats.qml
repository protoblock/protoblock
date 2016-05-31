import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import ProRotoQml.Backend 1.0
import ProRotoQml.Models 1.0

import Material 1.0

import "Utils.js" as Utils
Item {
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
                playerStatsModel.week =  selectedIndex +1
            }
        }
    }


    TextField{
        id: searchStatsBar
        width: parent.width / 1.07
        placeholderText: "Search Players.... "
        inputMethodHints: Qt.ImhNoPredictiveText
        anchors.top: weekPickerRow.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
    }

    TableView{
        id: statsView
        width: parent.width
        height: parent.height - (searchStatsBar.height +weekPickerRow.height + 10 )
        anchors.top: searchStatsBar.bottom
        anchors.topMargin: 5

        frameVisible: false
        sortIndicatorVisible: true
        sortIndicatorOrder: Qt.AscendingOrder
        onSortIndicatorColumnChanged: {
            proxyModel.sortAgain(getColumn(sortIndicatorColumn).role, Qt.DescendingOrder)
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
        model: SortModel {
            id: proxyModel
            source:playerStatsModel

            sortOrder: statsView.sortIndicatorOrder
            sortCaseSensitivity: Qt.CaseInsensitive
            sortRole: statsView.count > 0 ? statsView.getColumn(statsView.sortIndicatorColumn).role : ""

            filterString: "*" + searchStatsBar.text + "*"
            filterSyntax: SortModel.Wildcard
            filterCaseSensitivity: Qt.CaseInsensitive
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

    PlayerStatsModel{
        id: playerStatsModel
        viewType: PlayerStatsModel.SortWeek
        week: weekPicker.selectedIndex +1
    }
}
