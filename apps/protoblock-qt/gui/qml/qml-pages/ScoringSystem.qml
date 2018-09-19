import QtQuick 2.4

import Material 1.0
import Material.ListItems 1.0 as ListItems
import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQml.Models 2.2

import ProtoblockQml.Theme 1.0
Item {
    Flickable{
        height: parent.height
        width: parent.width
        contentHeight: parent.height * 2
        interactive: true
        boundsBehavior: Flickable.StopAtBounds


        Card {
            id: topcard
            width: parent.width * .50
            height: parent.height
            anchors.top: parent.top
            anchors.right: parent.horizontalCenter
            anchors.rightMargin:  ProtoScreen.guToPx(.5)

                ListModel {
                    id: scoringmodel

                    ListElement {
                        name: "Passing Yards	"
                        unitpoints: 	.05
                        unitdes: "points per yard"
                        team: "Offense"
                    }

                    ListElement {
                        name: "Passing Touchdown"
                        unitpoints: 	4
                        unitdes: "points"
                        team: "Offense"
                    }
                    ListElement {
                        name: "Passing Interception"
                        unitpoints: 	-1
                        unitdes: "points"
                        team: "Offense"

                    }
                    ListElement {
                        name: "Rushing Yards	"
                        unitpoints: 	.1
                        unitdes: "points per yard"
                        team: "Offense"

                    }
                    ListElement {
                        name: "Rushing Touchdowns	"
                        unitpoints: 	6
                        unitdes: "points"
                        team: "Offense"

                    }
                    ListElement {
                        name: "Receiving Yards	"
                        unitpoints: 	.1
                        unitdes: "points per yard"
                        team: "Offense"

                    }
                    ListElement {
                        name: "Receiving Touchdowns	"
                        unitpoints: 	6
                        unitdes: "points"
                        team: "Offense"

                    }

                    ListElement {
                        name: "Reception"
                        unitpoints: 	1
                        unitdes: "point per reception"
                        team: "Offense"

                    }

                    ListElement {
                        name: "Two point conversion"
                        unitpoints: 	2
                        unitdes: "points - passer, runner or receiver"
                        team: "Offense"

                    }

                    ListElement {
                        name: "Point After Conversion"
                        unitpoints: 	1
                        unitdes: "point per PAT"
                        team: "Kicker"

                    }

                    ListElement {
                        name: "Field Goal"
                        unitpoints: 	3
                        unitdes: "points. + .1 points each yard above 30"
                        team: "Kicker"

                    }

                    ListElement {
                        name: "Sack"
                        unitpoints: 	1
                        unitdes: "point per sack"
                        team: "Defense"

                    }

                    ListElement {
                        name: "Takeaway"
                        unitpoints: 	2
                        unitdes: "interception,  fumbles and special teams"
                        team: "Defense"
                    }

                    ListElement {
                        name: "Defensive Touchdown"
                        unitpoints: 	6
                        unitdes: "points"
                        team: "Defense"
                    }

                    ListElement {
                        name: "Safety"
                        unitpoints: 	5
                        unitdes: "points per safety"
                        team: "Defense"

                    }
                    ListElement {
                        name: "Shutout"
                        unitpoints: 	12
                        unitdes: "points for 0 points allowed"
                        team: "Defense"

                    }

                    ListElement {
                        name: "1-6 Points Allowed"
                        unitpoints: 	10
                        unitdes: "points for 6 points allowed"
                        team: "Defense"

                    }
                    ListElement {
                        name: "7-10 points"
                        unitpoints: 	8
                        unitdes: "points for 10 points allowed"
                        team: "Defense"
                    }

                }
                ListView {
//                        anchors.top: scoring.bottom
//                        anchors.topMargin: ProtoScreen.guToPx(2)
                    anchors.fill: parent
                    header:  Label{
                        id: lefttitle
//                        font.family: "Roboto"
                        font.pixelSize: ProtoScreen.font(ProtoScreen.MEDIUM)
                        width: parent.width
                        wrapMode: Text.WordWrap
                        text: "Protoblock PPR Scoring"
                        font.bold: true
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.margins: ProtoScreen.guToPx(2)
                        height: ProtoScreen.guToPx(8)
                    }
                    model: scoringmodel
                    section.property: "team"
                    section.criteria: ViewSection.FullString
                    spacing: 0
                    section {
                            property: "team"
                            criteria: ViewSection.FullString
                            delegate: Banner  {
                                elevation: 0
                                height: ProtoScreen.guToPx(3)
                                backgroundColor:  themeroot.theme.primaryColor
//                                    width: parent.width
                                text: section
                                fontSize: ProtoScreen.font(ProtoScreen.NORMAL)
                            }
                      }

                    delegate :  ListItems.Subtitled  {
                        text: name
                        valueText: unitpoints + " " + unitdes
//                        Layout.fillWidth: true
                        height: ProtoScreen.guToPx(3)
                        width: parent.width
                    }
                }
        }

        Card {
            id: rcard
            width: parent.width * .50
            height: parent.height
            anchors.top: parent.top
            anchors.left: parent.horizontalCenter
            anchors.leftMargin:  ProtoScreen.guToPx(.5)

            ListModel {
                 id: mathmodel
                 ListElement {
                         textstep:  'Let R equal actual results and Pn equal the projections made by each player. Take the difference of the projection from the results d(p). and then get the average difference Ď.'
                         imagestep: ':/math1.png'
                     }
                 ListElement {
                         textstep:  ' Filter out projections that are below average or are 100% or more off the mark F(d).'
                         imagestep: ':/math2.png'
                     }

                 ListElement {
                         textstep: 'Calculate unitpayout X, which will distribute more coins to the better predictions.'
                         imagestep: ':/math3.png'
                     }

                 ListElement {
                         textstep: 'Finally, the award function A(p) determines how many fantasy points are awarded for each projection, this is multiplied by 100 for fantasybits'
                         imagestep: ':/math4.png'
                     }

                 ListElement {
                         textstep: 'Any points leftover, L, due to bad or no projections get distributed to the block signer .'
                         imagestep: ':/math5.png'
                     }
             }

            ListView {
                anchors.fill: parent
                header:  Label{
                    id: lefttitle2
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: 'Fantasybit (ƑɃ) Distribution Algorithim '
                    font.pixelSize: ProtoScreen.font(ProtoScreen.MEDIUM)
                    font.family: fontfamFB

                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.margins: ProtoScreen.guToPx(2)
                    height: ProtoScreen.guToPx(8)
                }
                model: mathmodel
                delegate: Column {
                    anchors.left: parent.left
                    anchors.leftMargin: ProtoScreen.guToPx(4)
                  Label {
                      width: rcard.width * .90
                              font.pixelSize :ProtoScreen.font(ProtoScreen.NORMAL)
                                text: textstep
                                wrapMode: Text.WordWrap
//                                anchors.h: parent.horizontalCenter
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter

                                anchors.leftMargin: ProtoScreen.guToPx(4)
                  }
                  Image {
                           anchors.horizontalCenter: parent.horizontalCenter
                           source: "qrc" + imagestep;
                           width: rcard.width * .50
                           height: ProtoScreen.guToPx(8)
                           fillMode: Image.PreserveAspectFit
                  }
                }
            }
        }
    }
}
//                        text:  +" " + model.lastName +  " ("+ model.pos + ") - Week " + model.week
//                        subText: "Projection: " + model.projection + " Result: " + model.result
//                        elevation: 2
//                        valueText: "Award: " + model.award
//                        // FIXME set this as a platform.os android
//                        action: Image {
//                         source: "qrc:/" + model.team+".PNG"
//                            width: ProtoScreen.guToPx(4)
//                            height:  width
//                        }


//                    Rectangle {
//                    height: ProtoScreen.guToPx(3)
//                    Text {
//                        text: textstep
//                        font { family: 'Helvetica Neue'; pixelSize: ProtoScreen.font(ProtoScreen.SMALL) }
//                    }
//                }
//            }

//            }

//            Text {
//            text: '<script src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>'
//                textFormat: Text.RichText
//            }

//            Text {
//                text: 'Let R equal actual results and Pn equal the projections made by each player. Take the difference of the projection from the results d(p). and then get the average difference Ď.'
//                font { family: 'Helvetica Neue'; pixelSize: ProtoScreen.font(ProtoScreen.SMALL) }
//            }




//        }

//        Card {
//            width: parent.width * .50
//            height: parent.height
//            Column{
//                id: right
//                width: parent.width
//                height: parent.height // (welcomeTxt.paintedHeight - logo.height)
//                spacing: 5
//                anchors.horizontalCenter: parent.horizontalCenter

//                Label {
//                    id: scoringsystem
//                    anchors.topMargin: ProtoScreen.guToPx(2)
//                    width: parent.width
//                    font.pixelSize: ProtoScreen.font( ProtoScreen.MEDIUM )
//                    font.family: "Roboto"
//                    horizontalAlignment: Text.AlignHCenter
//                    text: "ProtoblockDistribution Algorithin"
//                    wrapMode: Text.WordWrap
//                }
//              }
//        }
