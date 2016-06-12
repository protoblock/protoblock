import QtQuick 2.0
import QtQuick.Controls 1.4
import ProRotoQml.Sql 1.0
import Material 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0

import "qrc:/Utils.js" as Utils
Item {
    id: weekRoot
    property string weekInFocus: "1"
    Component.onCompleted: weekModel.exec()
    onWeekInFocusChanged: {
        console.log(weekInFocus)
        weekModel.queryString = ""
        weekModel.queryString =  "SELECT * FROM Schedules WHERE Week='" + weekRoot.weekInFocus + "';"
        weekModel.exec()

    }

    ListView{
        id: weeksView
        width: parent.width / 1.07
        height: parent.height - ( weekPicker.height + ProtoScreen.guToPx(2.5) )
        clip: true
        spacing: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: weekPicker.bottom
        anchors.topMargin: weekPicker.expanded === false  ? 10 :  weekPicker.expandedHeight
        onCountChanged: console.log("sql count " + count)
        model: weekModel
        delegate: pnlZeroDel
    }
    ComboBox{
        id: weekPicker
        width: parent.width / 1.07
        height:  48
        model: Utils.weeks
        currentIndex: 0
        anchors.horizontalCenter: parent.horizontalCenter
        onCurrentTextChanged:  {
            weekRoot.weekInFocus =  currentText
        }
    }
    Component{
        id: pnlZeroDel
        ListItems.Game {
                    elevation: 5
                   text:  model.HomeTeam + " VS " + model.AwayTeam
                   subText: {
                       model.StadiumDetails_Name
//                       model.Date
//                    Qt.formatDateTime(time, "dddd MMMM yy h:mm ap")
                   }
                   secondaryItem: Image {
                       height:  Unit.dp(32)
                       width: height
                       source: "qrc:/"+ AwayTeam + ".PNG"
                   }
                   action: Image {
                       height:  Unit.dp(32)
                       width: height
                       source:"qrc:/"+ HomeTeam + ".PNG"

                   }
                   onClicked:{
                    currentHomeTeam = model.HomeTeam
                    currentAwayTeam = model.AwayTeam
                    rootLoader.source = "qrc:/WeeklyTradingLanding.qml"
                   }
               }
    }

    QmlSqlQueryModel{
        id: weekModel
        connectionName: "protoblock"
        queryString:"SELECT * FROM Schedules where Week='"+ weekInFocus+ "';"
        onQueryStringChanged: console.log(" Summon Query "+  queryString)
    }
}
