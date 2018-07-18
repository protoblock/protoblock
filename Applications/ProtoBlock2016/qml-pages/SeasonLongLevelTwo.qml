import QtQuick 2.0
import QtQuick.Controls 1.4

import ProRotoQml.Sql 1.0
import Material 1.0
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import "Utils.js" as Utils
Item {
    property string  lastQuery
    Component.onCompleted: singleWeekPlayersModel.exec()
    ListItems.BaseListItem{
        id: weekPlayerPickers
        width: parent.width
            MaterialComboBox{
                id: posPicker
                height: parent.height
                width: parent.width
                model: postionModel
                currentIndex: 1
                onCurrentIndexChanged:{
                    resetModel()
                }
            }
    }
    ListView{
        id: tradingListView
        width: parent.width
        height: parent.height - weekPlayerPickers.height
        anchors.top: weekPlayerPickers.bottom
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
            sLong: true
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
                + currentTeamInFocus
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





    function fillDefaultModels(){
        var positionArray = ["all positions","QB","RB","WR","TE","K","DEF"];
        for (var i in positionArray){
            postionModel.append({'text': positionArray[i] })
        }
        for (var ii = 0 ; ii < 17; ii++){
            if(ii === 0 ){
                weekModel.append({"text" : "all weeks"})
            }
            else
            {
                weekModel.append({"text" : ii.toString() })
            }
        }
    }

    ListModel{id: postionModel}
    ListModel{id: weekModel}


}
