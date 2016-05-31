import QtQuick 2.4
import ProRotoQml.Backend 1.0
import ProRotoQml.Models 1.0
import Material 1.0
import Material.ListItems 1.0 as ListItem
//import Material.Extras 1.0
Item {

    Rectangle{
        anchors.fill: parent
        TextField {
            id: searchBox
            placeholderText: "Search..."
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
            onCountChanged:  console.log(count)
            model: SortModel {
                id: sortModel
                source: pnlModel
                sortCaseSensitivity: Qt.CaseSensitive
                filterString: "*" + searchBox.text + "*"
                filterSyntax: SortModel.Wildcard
                filterCaseSensitivity: Qt.CaseSensitive
            }
            delegate:
                ListItem.Subtitled {
                text: fantasyName  + " GameId: "+ gameId
                subText: "Qty: " + qty
                         + " Price: " + price
                         + " PNL: " + pnl

                valueText: "Week: " + week
                maximumLineCount: 3
            }
        }
    }
    PnlModel{
        id: pnlModel
        gameId: demo.gameId
    }
}
