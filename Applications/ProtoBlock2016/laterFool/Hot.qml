import QtQuick 2.0
import ProRotoQml.Backend 1.0

Item {
    ListView{
        id:pnlData
        width: parent.width
        height: parent.height
        model: PnlNewModel
        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 1200 }
                NumberAnimation { properties: "x"; to: parent.width; duration: 1200; easing.type:Easing.OutBack  }
            }
        }
        delegate: Rectangle{
            width: parent.width
            height: 200
            color:"transparent"
            border{
                width: 1
                color: "black"
            }
            Text{
                text:{
                    "FantasyName " +     model.fantasyName+"\n" +
                            "PlayerId: "  +   model.playerId+"\n" +
                            "Qty"  +  model.qty +"\n" +
                            "Price: "    +model.price +"\n" +
                            "PNL: " + model.pnl + "\n" +
                            "Week: "  + model.week+"\n" +
                            "Game Id: "+   model.gameId
                }
                anchors.centerIn: parent
            }
        }
    }

}
