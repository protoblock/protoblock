import QtQuick 2.4

import Material 1.0
import Material.ListItems 1.0 as ListItems
import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import ProRotoQml.Theme 1.0

//import ProRotoQml.Protoblock 1.0
Item {
    Flickable{
        height: parent.height
        width: parent.width
        contentHeight: parent.height * 1.25
        interactive: true
        boundsBehavior: Flickable.StopAtBounds

            Card {
                width: parent.width * .90
                height: parent.height

                Label {
                    id: welcomeTxt
//                    anchors.topMargin: ProtoScreen.guToPx(2)
                    width: parent.width
                    font.pixelSize: ProtoScreen.font( ProtoScreen.MEDIUM )
                    font.family: "Roboto"
                    color: realRoot.theme ===  "Pinky" ? "white" : "black"
                    horizontalAlignment: Text.AlignHCenter
                    text: MiddleMan.controlMessage//"Companion Protoblock APP now available on iTunes and Google Play! "
                    wrapMode: Text.WordWrap
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: ProtoScreen.guToPx(2)
                    anchors.bottomMargin: ProtoScreen.guToPx(1)
                    onLinkActivated: Qt.openUrlExternally(link)
                }
                Column{
                    id: buttons
                    width: parent.width
                    height: parent.height - welcomeTxt.height
                    spacing: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: welcomeTxt.bottom
                    anchors.topMargin: ProtoScreen.guToPx(2)


                    CardWithBanner{
                        width: parent.width / 1.07
                        bannerText: "Professional Roto Blockchain "
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Protoblock is new game based directly on NFL PPR projections.  It distinguishes skilled experts while eliminating luck factors.  The game is fair, open and decentralized in a way that makes each Protoblock Player an owner."
                    }

                    CardWithBanner{
                        width: parent.width / 1.07
                        bannerText: "Projections Game"
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Each week of the season Protoblock Players make projections on NFL players PPR results. At the close of each NFL game, the total PPR points scored are calculated and run through the Blockchain protocol, which awards game points to the Protoblock players with the most accurate projections. Those with the most points rise  up the Leaderboard. New comprehensive Leaderboards are automatically generated, with potential to reveal unique expert niche skill in projecting specific teams, players or positions"
                    }


                    CardWithBanner{
                        width: parent.width / 1.07
                        bannerText: "Trading Game"
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Protoblock players who disagree on their projections can trade with each other. The player with the higher projection is the buyer, and the player with the lower projection is the seller. The trade occur at some point in-between. Who ever is ultimately correct will receive the others points."
                    }

                    CardWithBanner{
                        width: parent.width / 1.07
                        bannerText: theqmlseason + " Season Long Contracts "
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Hedge dynasty risk, with  future season long " + theqmlseason + " contracts. Contact us for more info. contact@protoblock.com"
                    }

                }
            }

    }
}

