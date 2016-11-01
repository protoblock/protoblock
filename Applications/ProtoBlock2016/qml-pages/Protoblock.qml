import QtQuick 2.4

import Material 1.0
import Material.ListItems 1.0 as ListItems
import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
Item {
    Flickable{
        height: parent.height
        width: parent.width
        contentHeight: parent.height * 4
        interactive: true
        boundsBehavior: Flickable.StopAtBounds


        Label {
            id: welcomeTxt
//            anchors.top: logo.bottom
            anchors.topMargin: ProtoScreen.guToPx(2)
            width: parent.width / 1.07
            font.pixelSize: ProtoScreen.font( ProtoScreen.MEDIUM )
            font.family: "Roboto"
            color: realRoot.theme ===  "Pinky" ? "white" : "black"
            horizontalAlignment: Text.AlignHCenter
            text: "2016 Rest Of Way trading avilable now! Claim your name, get in the game."
            wrapMode: Text.WordWrap
        }

        RowLayout {
            width: parent.width
            height: parent.height

            Card {
                width: parent.width * .50
                height: parent.height
                Column{
                    id: buttons
                    width: parent.width
                    height: parent.height // (welcomeTxt.paintedHeight - logo.height)
                    spacing: 5
                    anchors.horizontalCenter: parent.horizontalCenter
//                    anchors.top: welcomeTxt.bottom


                    CardWithBanner{
                        width: parent.width / 1.07
                        bannerText: "Protoblock 2016"
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
                        bannerText: "2017 Season Long Contracts "
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Hedge dynasty risk, with  future season long 2017 contracts. Contact us for more info. contact@protoblock.com"
                    }

                }
            }

            Card {
                width: parent.width * .50
                height: parent.height
                Column{
                    id: aboutCol
                    width: parent.width
                    height: parent.height
                    //            anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 10

                    Label{
                        font.family: "Roboto"
                        font.pixelSize: Qt.platform.os === "android" ? 48 : 32
                        font.bold: true
                        text: "  New Era"
                    }
                    ThinDivider{}
                    Label{
                        font.family: "Roboto"
                        font.pixelSize: Qt.platform.os === "android" ? 32 : 22
                        width: parent.width /  1.07
                        wrapMode: Text.WordWrap
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Protoblock is the first blockchain gaming company for fantasy football. A new era of fantasy football began with the launch of the first ever blockchain based fantasy football game for 2015. The upcoming 2016 season will take this one step farther with our new mobile apps, and much, much more."
                    }

                    Label{
                        font.family: "Roboto"
                        font.pixelSize: Qt.platform.os === "android" ? 48 : 32
                        font.bold: true
                        text: "  Our Mission"
                    }
                    ThinDivider{}

                    Label{
                        font.family: "Roboto"
                        font.pixelSize: Qt.platform.os === "android" ? 32 : 22
                        width: parent.width /  1.07
                        wrapMode: Text.WordWrap
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Protoblock is on a mission to create a new fantasy economy, were traditional fantasy football skill is valued above all. We combine our passion for fantasy football with our software development expertise.\n
    At Protoblock, we believe fantasy football is a skill, and our experts deserve to be recognized and rewarded like sports or movie stars."
                    }
                    //Fantasy Football players mimic the role of a real NFL general managers.
                    //50 million people play fantasy.
                    //Fantasy generates more revenue than the entire NFL."
                }
            }
        }
    }
}

