import QtQuick 2.0
import QtQuick.Controls 1.4
import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems

Item {
    property string cHT
    property string cAT
    SplitView{
        anchors.fill: parent
        orientation: Qt.Horizontal
        ListView{
            id: weekTable
            width: parent.width / 5
            height: parent.height
            model:weekModel
            delegate:         ListItems.Game {
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
                    cHT = model.HomeTeam
                    cAT = model.AwayTeam
                    weekPlayersModel.exec()
                }
            }
        }


        TableView{
            width: parent.width - (weekTable*2)
            height: parent.height
            model:weekPlayersModel
            TableViewColumn{
                role: "Name"
                title: "Name"
            }
            TableViewColumn{
                role: "Team"
                title: "Team"
            }
            TableViewColumn{
                role: "LastName"
                title: "Last Name"
            }
            TableViewColumn{
                role: "Position"
                title: "Position"
            }
            TableViewColumn{
                role: "AverageDraftPosition"
                title: "ADP"
            }
            TableViewColumn{
                role: "CurrentStatus"
                title: "Current Status"
            }
        }

        TableView{
            id: leaderBoardTable
            width: parent.width / 5
            height: parent.height
            model:weekModel
            TableViewColumn{
                role: "HomeTeam"
                title: "Home Team"
            }
            TableViewColumn{
                role: "AwayTeam"
                title: "Away Team"
            }
        }
    }



    QmlSqlQueryModel{
        id: weekModel
        connectionName: "protoblock"
        queryString:"SELECT * FROM Schedules where Week='1';"
        onQueryStringChanged: console.log(" Summon Query "+  queryString)
        Component.onCompleted: exec()
    }
    QmlSqlQueryModel{
        id: weekPlayersModel
        connectionName: "protoblock"
        queryString:{
            "SELECT * FROM tfprod_fantasydata WHERE Team = '"+cHT
                    + "\' OR Team=\'"
                    + cAT
                    + "\' ORDER BY DepthOrder; "
        }
    }

}
