import QtQuick 2.0

import ProRotoQml.Backend 1.0
import ProRotoQml.Models 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItem
import Material.Extras 1.0

import "QChart.js" as Charts
import "Utils.js" as Utils

Item{
    property string playerInFocus
    property string  playerInFocusId

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
//                playerStatsModel.week =  selectedIndex +1
            }
        }
    }

    TextField{
        id: searchBar
        width: parent.width / 1.07
        anchors.horizontalCenter: parent.horizontalCenter
        placeholderText: "Search .... "
        inputMethodHints: Qt.ImhNoPredictiveText
        text: demo.leaderInView
        anchors.top: weekPickerRow.bottom
        anchors.topMargin: 10
    }
    ListView{
        id: awardData
        clip: true
        width:  parent.width
        height: parent.height -( searchBar.height + weekPickerRow.height + 10  )
        anchors.top: searchBar.bottom
        anchors.topMargin: 5
        model: SortModel {
            id: sortModel
            source: awardsModel
            sortByRoleName: "playersName"
            sortCaseSensitivity: Qt.CaseSensitive
            filterString: "*" + searchBar.text + "*"
            filterSyntax: SortModel.Wildcard
            filterCaseSensitivity: Qt.CaseSensitive
        }
        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 1200 }
                NumberAnimation { properties: "x"; to: parent.width; duration: 1200; easing.type:Easing.OutBack  }
            }
        }
        delegate:
            ListItem.Subtitled {
            text: model.fantasyName + " "
                  + "awarded " + model.award + ", for projection(" + model.projection +"). "
                  + ((model.pnl === 0) ? "" : ("Profit of " + model.pnl + " from " + model.settleQty + " contracts (" + model.avgPrice + "). "))

            subText: model.playersName + " (" + model.pos + ") " + model.team
                     + " " + model.season
                     + " wk" + model.week
                     + ((model.team===model.home) ? (" vs " + model.away ): (" @ " + model.home))
                     + " result: " + model.result
            valueText: model.award + model.pnl
            maximumLineCount: 3
            onClicked: {
                playerInFocus = model.fantasyName
                playerInFocusId = model.playerId

                Utils.awardAward = model.award
                Utils.awardProjedction = model.projection
                playerInfo.proje = Utils.awardProjedction
                playerInfo.awa = Utils.awardAward




                playerInfo.open()
            }
        }
    }
    AwardsModel{
        id:awardsModel
        awardType: AwardsModel.FillWeek
        week: weekPicker.selectedIndex+ 1
    }

    Dialog {
        id: playerInfo
        title: playerInFocus
        property int proje
        property int awa
        onOpened: {
            chart.height = awardData.height / 1.3
            chart.chartData = [{ value: Utils.awardAward, color: Theme.primaryColor }, { value: Utils.awardProjedction, color: Theme.accentColor } ]

            avgChart.height = awardData.height / 1.3
            avgChart.chartData = [{ value: Utils.awardAward, color: Theme.primaryColor }, { value: Utils.awardProjedction, color: Theme.accentColor } ]

            chart.repaint()
            avgChart.repaint()
           }

        Row{
            Chart{
                id: avgChart;
                width: height
                chartAnimated: true;
                chartAnimationEasing: Easing.OutQuint;
                chartAnimationDuration: 1200;
                chartType: Charts.ChartType.DOUGHNUT;
                Rectangle{
                    width: chart.width / 4
                    height: width
                    color: "transparent"
                    anchors.centerIn: chart
                    Column{
                        anchors.fill: parent
                        Row{width: parent.width ; height: parent.height / 2; spacing: 2
                            Rectangle{width: projText.paintedHeight; height: width ; color: Theme.accentColor}
                            Text { id:projText; text: "Projection: " + playerInfo.proje }
                        }
                        Row{width: parent.width ; height: parent.height / 2; spacing: 2
                            Rectangle{width: projText.paintedHeight; height: width ; color: Theme.primaryColor}
                            Text { text: "Award: " + playerInfo.awa }
                        }
                    }
                }
            }


        Chart{
            id: chart;
            width: height
            chartAnimated: true;
            chartAnimationEasing: Easing.OutQuint;
            chartAnimationDuration: 1200;
            chartType: Charts.ChartType.DOUGHNUT;
            Rectangle{
                width: chart.width / 4
                height: width
                color: "transparent"
                anchors.centerIn: chart
                Column{
                    anchors.fill: parent
                    Row{width: parent.width ; height: parent.height / 2; spacing: 2
                        Rectangle{width: projText.paintedHeight; height: width ; color: Theme.accentColor}
                        Text { text: "Projection: " + playerInfo.proje }
                    }
                    Row{width: parent.width ; height: parent.height / 2; spacing: 2
                        Rectangle{width: projText.paintedHeight; height: width ; color: Theme.primaryColor}
                        Text { text: "Award: " + playerInfo.awa }
                    }
                }
            }
        }
        }
//        positiveButtonText: "Close";

    }
}
