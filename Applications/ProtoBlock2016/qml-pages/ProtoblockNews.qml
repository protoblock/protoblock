import QtQuick 2.4

import Material 1.0
import Material.ListItems 1.0 as ListItems

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
Item {
    Flickable{
        height: parent.height
        width: parent.width
        contentHeight: parent.height * 4
        interactive: true
        boundsBehavior: Flickable.StopAtBounds
        Image {
            id: logo
            source: "qrc:/logoFinal.png"
            fillMode: Image.PreserveAspectFit
            width: parent.width / 1.07
            anchors.horizontalCenter: parent.horizontalCenterx
            anchors.top: parent.top
            anchors.topMargin: ProtoScreen.guToPx(1)
        }
        Label {
            id: welcomeTxt
            anchors.top: logo.bottom
            anchors.topMargin: ProtoScreen.guToPx(2)
            width: parent.width / 1.07
            font.pixelSize: ProtoScreen.font( ProtoScreen.MEDIUM )
            font.family: "Roboto"
            color: realRoot.theme ===  "Pinky" ? "white" : "black"
            horizontalAlignment: Text.AlignHCenter
            text: "2016 Enrollment Open Now ! Tell your friends to claim their names.\n Below is a news feed of protoblocks news"
            wrapMode: Text.WordWrap
        }
        Column{
            id:buttons
            width: parent.width
            height: parent.height - (welcomeTxt.paintedHeight - logo.height)
            spacing: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: welcomeTxt.bottom


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
                text: "Protoblock players who disagree on their projections can trade with each other. The player with the higher projection is the buyers, and the player with the lower projection is the seller. The trade occur at some point in-between. Who ever is ultimately correct will receive the others points."
            }



            //    Card{
            //        width: parent.width / 1.07
            //        height: ProtoScreen.guToPx(20)
            //        anchors.horizontalCenter: parent.horizontalCenter
            //        Column{
            //            width: parent.width
            //            height: parent.height
            //            spacing: 1
            //        Banner{
            //                width: parent.width
            //                text: "PROTOBLOCK 2016"
            //                backgroundColor:  themeroot.theme.primaryColor
            //            }
            //            Label{
            //                width: parent.width / 1.07
            //                font.pixelSize: ProtoScreen.formFactor === "phone" ? ProtoScreen.guToPx(4):ProtoScreen.guToPx(2.75)
            //                font.family: "Roboto"
            //                horizontalAlignment: Text.AlignHCenter
            //                color: realRoot.theme ===  "Pinky" ? "white" : "black"
            //                wrapMode: Text.WordWrap
            //                text: "Protoblock is new game based directly on NFL PPR projections.  It distinguishes skilled experts while eliminating luck factors.  The game is fair, open and decentralized in a way that makes each Protoblock Player an owner."
            //            }
            //        }
            //    }
        }
    }
}
