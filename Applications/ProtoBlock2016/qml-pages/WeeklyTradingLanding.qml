import QtQuick 2.0
import QtQuick.Controls 1.4

import Material 1.0
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems

import ProRotoQml.Sql 1.0
import ProRotoQml.Theme 1.0

Item {
    property string  lastQuery
        width : 20
        Component.onCompleted:{
            teamPicker.currentIndex = 0
            pPicker.currentIndex = 1
        }
            Rectangle{
            id: pickers
            width:  parent.width
            height: teamPicker.expanded ? teamPicker.expandedHeight : 48
            color: "transparent"

            ComboBox{
                id: teamPicker
                width: parent.width / 2
                height: parent.height
                model:  [ currentHomeTeam , currentAwayTeam ]
                onCurrentIndexChanged:{
                    resetModel()
                }
            }

            ComboBox{
                id: pPicker
                height: parent.height
                width: parent.width / 2
                model: postionModel
                anchors.right: parent.right
                onCurrentIndexChanged:{
                    resetModel()
                }
            }

        }

    ListView{
        id: tradingListView
        width: parent.width
        height:  parent.height - pickers.height
        anchors.top: pickers.bottom
        anchors.topMargin: pickers.height > ProtoScreen.guToPx(6) ? pickers.height :0
        clip: true
        spacing: 3
        model: weekPlayersModel
        delegate: lightClientDel
    }



    Component{
        id: lightClientDel
        ListItems.Subtitled{
            elevation: 5
            text: model.FirstName + " " + model.LastName + " #" +model.Number
            subText: "Height:  " + model.Height + " Weight: " + model.Weight
                     +  " College: "  + model.College +  " Experience: " + model.Experience
                     + " years "
                     + model.BirthDateString  +" (" + model.Age + ") "
            action: Image{
                height: parent.height
                width : height
                fillMode: Image.PreserveAspectFit
                source: model.PhotoUrl
            }
            valueText: "ADP: " + model.AverageDraftPosition
            maximumLineCount: 4
            onClicked: fillSinglePlayer(model.PlayerID  )
        }
    }

    QmlSqlQueryModel{
        id: weekPlayersModel
        connectionName: "protoblock"
        queryString:{
            "SELECT * FROM tfprod_fantasydata WHERE FantasyPosition=\'"
                    + pPicker.currentText
                    + "\' AND Team=\'"
                    + teamPicker.currentText
                    + "\' ORDER BY DepthOrder; "
        }
    }


    QmlSqlQueryModel{
        id: singleWeekPlayersModel
        connectionName: "protoblock"
    }
       Repeater {
        id:  tradingCard
        property bool shown: false
        width:shown === true  ?   parent.width / 1.07 : 0
        height: shown === true  ? parent.height / 1.07 : 0
        delegate:
            PlayersCard{
            id: pCard
            property bool shown: tradingCard.shown
            width:shown === true  ?   parent.width / 1.07 : 0
            height: shown === true  ? parent.height / 1.07 : 0
            scale: shown === true ?  1: 0
            anchors.centerIn: parent
            sLong: false
            Behavior on scale{NumberAnimation{duration: 600; easing.type: Easing.OutBounce}}
            Row{
                height: pCard.height /5
                width: pCard.width / 1.07
                anchors.bottom: pCard.bottom
                Button{
                    elevation: 5
                    height: parent.height
                    width: parent.width / 3
                    text:"Buy"
                    onClicked: pCard.buy = true
                }
                Button{
                    elevation: 5
                    text:"Sell"
                    width: parent.width / 3
                    height: parent.height
                    onClicked: pCard.buy = false
                }
                Button{
                    elevation: 5
                    text:"Back"
                    height: parent.height
                    width: parent.width / 3
                    onClicked: {
                        tradingCard.shown = false
                    }
                }
            }
        }



    }
    function fillSinglePlayer(pId){
        console.log("here comes the pId " + pId)
        lastQuery = weekPlayersModel.queryString
        singleWeekPlayersModel.queryString = ""
        singleWeekPlayersModel.queryString ="SELECT * FROM tfprod_fantasydata WHERE PlayerID='"
                + pId
                +"';"

        console.log(singleWeekPlayersModel.queryString)
        singleWeekPlayersModel.exec()
        tradingCard.model = singleWeekPlayersModel
        tradingCard.shown = true
    }



    function resetModel(){
        weekPlayersModel.queryString = ""
        weekPlayersModel.queryString = "SELECT * FROM tfprod_fantasydata WHERE FantasyPosition=\'"
                + pPicker.currentText
                + "\' AND Team=\'"
                + teamPicker.currentText
                + "\' ORDER BY CASE
                WHEN DepthOrder IS NULL THEN 1
                WHEN DepthOrder = ''    THEN 2
                ELSE 3
                END DESC,
                DepthOrder ASC ; "

        weekPlayersModel.exec()
    }

}
