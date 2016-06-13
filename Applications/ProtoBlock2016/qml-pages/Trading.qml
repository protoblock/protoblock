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
        elevation: 5
        anchors.centerIn: parent

        Column{
            width: parent.width
            height: parent.height
            anchors.fill: parent
            spacing: ProtoScreen.guToPx(1.25)

            CardWithBanner{
                id: gstate
                width: parent.width / 1.07
                bannerText: "2016 Season Contracts. "
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Starting July"
            }

            CardWithBanner{
                width: parent.width / 1.07
                bannerText: "2016 Weekly Contracts. "
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Waiting 53 Man Rosters. "
            }

            CardWithBanner{
                width: parent.width / 1.07
                bannerText: "2017 Season Contracts. "
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Stay tuned for futures contracts"
            }
        }

    }
}
