import QtQuick 2.0
import ProRotoQml.Backend 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Material.ListItems 1.0 as ListItem
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
            model: 18
            selectedIndex:  demo.weekInFocus
            onSelectedIndexChanged:  demo.weekInFocus = selectedIndex
        }
    }

    ListView {
        id: scheduledGamesView
        width: parent.width
        height: parent.height - (weekPickerRow.height+ 10 )
        spacing: 8
        clip: true
        anchors.top: weekPickerRow.bottom
        model: scheduledGamesModel
        delegate: ListItem.Game {
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
            onClicked: demo.gameId = model.gameId
        }

        ScheduledGamesModel{
            id: scheduledGamesModel
            week: demo.weekInFocus
        }
    }
}
