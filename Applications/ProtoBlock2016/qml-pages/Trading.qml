import QtQuick 2.0
import QtQuick 2.0

import Material.ListItems 1.0 as ListItems
import Material 1.0

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0

Item {
    Card{
        width: parent.width / 1.07
        height:     parent.height / 1.07
        anchors.centerIn: parent
        elevation: 1
        Banner{
            id: ban
            text: "Trades "
            color: "white"
            backgroundColor: realRoot.theme ===  "Pinky" ? "black" : root.theme.primaryColor
            width: parent.width
            height: ProtoScreen.guToPx(6)
        }
        ListView{
            id: leaderboard
            width: parent.width - 5
            anchors.top: ban.bottom
            height: parent.height - ban.height
            clip: true
            model:  90
            delegate:
                ListItems.Subtitled{
                elevation: 2
                width: parent.width
                text: "Player: " +  modelData  + " Status: "
                subText: "Amount " + Math.floor((Math.random() * 30) + 1);
                action: Image{
                    height: parent.height
                    width : height
                    fillMode: Image.PreserveAspectFit
                    source: index%3 == 0 ? "qrc:/icons/local_atm.png" : "qrc:/icons/ic_poll.png"
                }
                valueText: {
                    if (index%3 == 0)
                            "<b>Ask Size (Sell)</b> " + Math.floor((Math.random() * 40) + 1);
                        else
                            "<b>BidSize (Buy)</b> "  + Math.floor((Math.random() * 40) + 1);
                }
            }
        }
        Scrollbar{flickableItem: leaderboard }
    }
}
