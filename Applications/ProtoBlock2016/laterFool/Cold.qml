import QtQuick 2.4
import ProRotoQml.Backend 1.0
import ProRotoQml.Models 1.0
import Material 1.0
import Material.ListItems 1.0 as ListItem
Item {
    Rectangle{
        anchors.fill: parent
        TextField {
            id: searchBox
            placeholderText: "Search Players ..."
            inputMethodHints: Qt.ImhNoPredictiveText
            width: parent.width / 1.07
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 5
        }
        ListView{
            id: playerCard
            width: parent.width
            height: parent.height - searchBox.height
            anchors.top: searchBox.bottom
            model: SortModel {
                id: sortModel

                source: Backend.pnm
    //            sortByRoleName: "playersName"
                sortCaseSensitivity: Qt.CaseSensitive
                filterString: "*" + searchBox.text + "*"
                filterSyntax: SortModel.Wildcard
                filterCaseSensitivity: Qt.CaseSensitive
            }


            delegate:
                ListItem.Subtitled {
                text: model.firstName + " " +model.lastName
                subText: model.position + " For " + model.team + " Status " + model.playerStatus
                valueText: "Player Id: " + model.playerId
                maximumLineCount: 3
            }
        }
    }
}
