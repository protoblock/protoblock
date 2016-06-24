import QtQuick 2.4

import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItems

Item {
    Flickable{
        height: parent.height
        width: parent.width
        contentHeight: parent.height * 2
        interactive: true
        boundsBehavior: Flickable.StopAtBounds
        Image {
            id: logo
            source: "qrc:/logoFinal.png"
            fillMode: Image.PreserveAspectFit
            width: parent.width / 1.07
            height: parent.height / 6
            anchors.horizontalCenter: parent.horizontalCenterx
        }
        Label {
            id: welcomeTxt
            anchors.top: logo.bottom
            anchors.topMargin:  paintedHeight
            width: parent.width / 1.07
            font.pixelSize: ProtoScreen.formFactor === "phone" ? ProtoScreen.guToPx(4):ProtoScreen.guToPx(2.75)
            font.family: "Roboto"
            color: realRoot.theme ===  "Pinky" ? "white" : "black"
            horizontalAlignment: Text.AlignHCenter
            text: "2016 Enrollment Open Now - tell your friends to claim their names"
            wrapMode: Text.WordWrap
        }
        Column{
            id:buttons
            width: parent.width
            height: parent.height - (welcomeTxt.paintedHeight - logo.height)
            spacing: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: welcomeTxt.bottom
            anchors.topMargin: 10

            CardWithBanner{
                width: parent.width / 1.07
                bannerText: "BETA STARTS IN JULY"
                anchors.horizontalCenter: parent.horizontalCenter
                text: "With a user account you’re ready for the rollout/updates. We want your early feedback to help make the game interface the best it can be in time for the 53-man roster cuts and Week 1.
Please use integrated Chat tab to share feedback or questions."
            }

            CardWithBanner{
                width: parent.width / 1.07
                bannerText: "PROTOBLOCK 2016"
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Protoblock is new game based directly on NFL PPR projections.  It distinguishes skilled experts while eliminating luck factors.  The game is fair, open and decentralized in a way that makes each Protoblock Player an owner."
            }

            CardWithBanner{
                width: parent.width / 1.07
                bannerText: "HOW DO YOU PLAY"
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Each week of the season Protoblock Players make projections for each/any player in the NFL.  At the close of each NFL game ‘fantasy-bits’, the in-game currency, are generated and distributed by the blockchain protocol in proportion to the FFP scoring and among the most accurate projections.  Those with the most fantasy-bits rise the Overall Leaderboard.  New comprehensive Leaderboards data automatically generated, with new deep information and potential to reveal unique expert niche skill for teams, players, or positions."
            }


            CardWithBanner{
                width: parent.width / 1.07
                bannerText: "PROTOBLOCK at FSTA"
                anchors.horizontalCenter: parent.horizontalCenter
                text: "We are happy to announce the new Protoblock 2016 game and launch enrollment at the annual Fantasy Sports Trade Association n New York this June 13-14.   All new accounts during the FSTA 2016 Summer Conference are entered to win a 2016 main event entry. Choose from FFPC, FFWC or NFCC.

Follow @protoblock for winner and general Protoblock news/info"
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
            //                backgroundColor:  root.theme.primaryColor
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

