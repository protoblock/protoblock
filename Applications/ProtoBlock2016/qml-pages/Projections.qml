import QtQuick 2.0
import QtQuick.Controls 1.4
//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0

Item {
    Component.onCompleted: {
         if ( !themeroot.reloadleaders )
              themeroot.reloadleaders = true
          else
              MiddleMan.allNamesGet()
    }


    CardWithBanner{
        id: gstate
        width: parent.width / 1.07
        bannerText: "Season: 2016 Week: 0 "
        text: "Game Status: Waiting 53 Man Roster \n2016 Projection Game To Begin on Sept 3"
        anchors{
            top: parent.top
            topMargin:ProtoScreen.guToPx(.5)
            horizontalCenter: parent.horizontalCenter
        }
    }

    Card{
        width: parent.width / 1.07
        height: rootLoader.height / 1.07 - (gstate.height + ban.height)
        elevation: 1
        anchors{
            topMargin: ProtoScreen.guToPx(1)
            top: gstate.bottom
            horizontalCenter: parent.horizontalCenter
        }
        Banner{
            id: ban
            text: "2016 Leaderboard"
            color: "white"
            backgroundColor: realRoot.theme ===  "Pinky" ? "black" : themeroot.theme.primaryColor
            width: parent.width
            height: ProtoScreen.guToPx(6)
        }
        ListView{
            id: leaderboard
            width: parent.width - 5
            anchors.top: ban.bottom
            height: rootLoader.height - (ban.height + gstate.height)
            clip: true
            model:   MiddleMan.allNamesList()
            delegate:
                ListItems.Subtitled{
                elevation: 2
                backgroundColor: realRoot.uname ===  modelData ? Colors.amber : "white"
                width: parent.width
                text: /*"FantasyName: " +*/  modelData
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


    Connections {
        target: MiddleMan
        onLeaderBoardchanged: {
            leaderboard.model = MiddleMan.allNamesList()
        }
    }

}
    //    property string cHT
//    property string cAT
//    SplitView{
//        anchors.fill: parent
//        orientation: Qt.Horizontal
//        ListView{
//            id: weekTable
//            width: parent.width / 5
//            height: parent.height
//            model:weekModel
//            delegate:         ListItems.Game {
//                elevation: 5
//                text:  model.HomeTeam + " VS " + model.AwayTeam
//                subText: {
//                    model.StadiumDetails_Name
//                    //                       model.Date
//                    //                    Qt.formatDateTime(time, "dddd MMMM yy h:mm ap")
//                }
//                secondaryItem: Image {
//                    height:  Unit.dp(32)
//                    width: height
//                    source: "qrc:/"+ AwayTeam + ".PNG"
//                }
//                action: Image {
//                    height:  Unit.dp(32)
//                    width: height
//                    source:"qrc:/"+ HomeTeam + ".PNG"

//                }
//                onClicked:{
//                    cHT = model.HomeTeam
//                    cAT = model.AwayTeam
//                    weekPlayersModel.exec()
//                }
//            }
//        }


//        TableView{
//            width: parent.width - (weekTable*2)
//            height: parent.height
//            model:weekPlayersModel
//            TableViewColumn{
//                role: "Name"
//                title: "Name"
//            }
//            TableViewColumn{
//                role: "Team"
//                title: "Team"
//            }
//            TableViewColumn{
//                role: "LastName"
//                title: "Last Name"
//            }
//            TableViewColumn{
//                role: "Position"
//                title: "Position"
//            }
//            TableViewColumn{
//                role: "AverageDraftPosition"
//                title: "ADP"
//            }
//            TableViewColumn{
//                role: "CurrentStatus"
//                title: "Current Status"
//            }
//        }

//        TableView{
//            id: leaderBoardTable
//            width: parent.width / 5
//            height: parent.height
//            model:weekModel
//            TableViewColumn{
//                role: "HomeTeam"
//                title: "Home Team"
//            }
//            TableViewColumn{
//                role: "AwayTeam"
//                title: "Away Team"
//            }
//        }
//    }



//    QmlSqlQueryModel{
//        id: weekModel
//        connectionName: "protoblock"
//        queryString:"SELECT * FROM Schedules where Week='1';"
//        onQueryStringChanged: console.log(" Summon Query "+  queryString)
//        Component.onCompleted: exec()
//    }
//    QmlSqlQueryModel{
//        id: weekPlayersModel
//        connectionName: "protoblock"
//        queryString:{
//            "SELECT * FROM tfprod_fantasydata WHERE Team = '"+cHT
//                    + "\' OR Team=\'"
//                    + cAT
//                    + "\' ORDER BY DepthOrder; "
//        }
//    }
//}
