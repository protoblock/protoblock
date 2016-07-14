import QtQuick 2.0
import QtQuick.Controls 1.4
//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0


Item {
    Card {
        width: parent.width
        height: parent.height
        elevation: 0
        anchors.centerIn: parent

        Column{
            width: parent.width
            height: parent.height
            anchors.fill: parent
            spacing: ProtoScreen.guToPx(1.25)

            CardWithBanner{
                id: gstate
                width: parent.width / 1.07
                bannerText: "2016 Season Contracts"
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Starting July"
            }

            CardWithBanner{
                width: parent.width / 1.07
                bannerText: "2016 Weekly Contracts "
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Waiting 53 Man Rosters"
            }

            CardWithBanner{
                width: parent.width / 1.07
                bannerText: "2017 Season Long Contracts "
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Stay tuned for futures contracts "
            }
        }

    }
}








//Item {










//    Label {
//        rotation: -45
//        text: qsTr("Comming Soon")
//        color: "#40000000"
//        anchors.centerIn: parent
//        font.pixelSize: ProtoScreen.font( ProtoScreen.XXLARGE)
//        font.bold:  true
//    }
//}







////        Banner{
////            id: ban
////            text: "Trades "
////            color: "white"
////            backgroundColor: realRoot.theme ===  "Pinky" ? "black" : themeroot.theme.primaryColor
////            width: parent.width
//////            height: ProtoScreen.guToPx(6)
////        }

////        ListView{
////            id: leaderboard
////            width: parent.width - 5
////            anchors.top: ban.bottom
////            height: parent.height - ban.height
////            clip: true
////            model:  90
////            delegate:
////                ListItems.Subtitled{
////                property int playerId: Math.floor((Math.random() * 665) + 1 )
////                elevation: 2
////                width: parent.width
////                text: "Player: " + playerId
////                subText: "Buyer:  " + " Seller"
////                action: Image{
////                    height: parent.height
////                    width : height
////                    fillMode: Image.PreserveAspectFit
////                    source: index%3 == 0 ? "qrc:/icons/local_atm.png" : "qrc:/icons/ic_poll.png"
////                }
////                valueText:{ Math.floor((Math.random() * 40) + 1) + "<b> @ </b>  " + Math.floor((Math.random() * 40) + 1);
////                }
////            }
////        }
////        Scrollbar{flickableItem: leaderboard }
////    }
////}
