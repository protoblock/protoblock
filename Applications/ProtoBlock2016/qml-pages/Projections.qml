import QtQuick 2.0
import QtQuick.Controls 1.4
//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1


Item {
    id: topi
    property int week: 1

    Component.onCompleted: {
//         pageHelper.title = "Projections 2016 Week" + week
    }

        // spacer
    Rectangle{width: ProtoScreen.guToPx(.125); height: ProtoScreen.guToPx(1);color: "transparent"}

    Card {
        id: topcard
        width: parent.width * .99
        height: parent.height
        anchors{
            top: parent.top
            topMargin:ProtoScreen.guToPx(2)
            bottomMargin:ProtoScreen.guToPx(1)
            horizontalCenter: parent.horizontalCenter
            margins: ProtoScreen.guToPx(.125)
        }
        Banner {
            backgroundColor: "white"
            color: themeroot.theme.primaryColor
            anchrosHType: "center"
            text: MiddleMan.seasonString + " 2016 - Week " + MiddleMan.theWeek + " - " + MiddleMan.liveSync;
            id: cBan
            width: parent.width
            elevation: 0
        }
        SystemPalette { id: pal }

        SplitView {
            handleDelegate: Item {
//                width: ProtoScreen.guToPx(.5)
//                implicitWidth: rec.width + rec3.width + rec2.width
                height: parent.height-cBan.height
//                width: styleData.hovered ? ProtoScreen.guToPx(.85) : ProtoScreen.guToPx(.3)
                anchors.margins: 0
                    Rectangle {
                        border.width: 0
                        id: rec
    //                    width: styleData.hovered ?
                        width: styleData.hovered ? ProtoScreen.guToPx(.25) : ProtoScreen.guToPx(.1)
    //                                             : ProtoScreen.guToPx(.375)
                        color: styleData.hovered ? "black" : Qt.darker(pal.window, 1.5)
                        height: parent.height
                        anchors.right: rec3.left

    //                    MouseArea {
    //                        hoverEnabled: true
    //                        anchors.fill: parent
    //                        onEntered: {
    //                            console.log(" entered ")
    //                            rec.color = "red"
    //                            cursorShape = Qt.SizeHorCursor
    //                        }
    //                    }

                   }
                    Rectangle {
                        border.width: 0
                        id: rec3
                        width: styleData.hovered ? ProtoScreen.guToPx(.35) : ProtoScreen.guToPx(.1)

//                        width: ProtoScreen.guToPx(.1)//styleData.hovered ? ProtoScreen.guToPx(.125) : ProtoScreen.guToPx(.375)
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: styleData.hovered ? "white" : Qt.darker(pal.window, 1.5)
                    }
                    Rectangle {
                       border.width: 0
                       id: rec2
                       width: styleData.hovered ? ProtoScreen.guToPx(.25) : ProtoScreen.guToPx(.1)

//                       width: ProtoScreen.guToPx(.1)//styleData.hovered ? ProtoScreen.guToPx(.125) : ProtoScreen.guToPx(.375)
                       height: parent.height
                       anchors.left: rec3.right
                       color: styleData.hovered ? "black" : Qt.darker(pal.window, 1.5)
                    }

            }
            property alias scheduleView1: scheduleView1
        //    anchors.margins: ProtoScreen.guToPx(1)
              anchors.top: cBan.bottom
              anchors.topMargin: ProtoScreen.guToPx(.25)
              orientation: Qt.Horizontal
              width: parent.width
              height: parent.height

              Component.onCompleted: {
                  lpv.releasedit.connect(releaseditMethod)
              }
              function releaseditMethod(fname) {
                  ppt.addcolumn(fname)
                  console.log(" releaseditMethod " )
              }
              Card {
                  anchors.leftMargin: 10
                  Layout.minimumWidth: parent.width * .10
                  Layout.maximumWidth: parent.width * .30
        //          Layout.fillWidth: true
                  //          color: "lightblue"
                  ScheduleView {
                      id: scheduleView1

                  }
              }
              Card {

        //          width: parent.width *.50
                  Layout.minimumWidth: parent.width * .30
                  Layout.maximumWidth: parent.width * .80
            //          color: "lightblue"
                      Layout.fillWidth: true
                  PlayerProjTable {
                      id: ppt
                  }

                  Rectangle {
                    id: myrec
                    anchors {
                        top: parent.top
                        right:  parent.right
                        bottom:  parent.bottom
                    }
                    width: parent.width
                    color: "transparent"
                    DropArea {
                        anchors.fill: parent
                        onEntered: {
                            console.log(" entered ")
                            drag.source.caught = (ppt.ccount > 10) ? false : true
                            myrec.color = Qt.binding( function() {
                               if( ppt.ccount > 10) return "red";
                               else
                                return "green";
                            })

                            myrec.opacity = Qt.binding(function() {
                                return .20})

                            ppt.indrop(drag.source.fname)

                        }
                        onExited: {
                            console.log(" exit ")
                            drag.source.caught = false;
                            myrec.color = Qt.binding( function() {
                               return "transparent"})

                            myrec.opacity = Qt.binding(function() {
                                return 0})

                            ppt.donedrop()
                        }
                    }
                }

              }
        //      Rectangle {
        //          id: centerItem
        //          width: parent.width * .60
        //          Layout.maximumWidth: parent.width * .80
        //          color: "lightgray"
        //          Text {
        //              text: "View 2"
        //              anchors.centerIn: parent
        //          }
        //      }
              Card {
                  anchors.leftMargin: 10

                  id: rightr
                  Layout.minimumWidth: parent.width * .20
                  Layout.maximumWidth: parent.width * .60
        //          Layout.fillWidth: true
                  LeaderProjView {
                      id: lpv
        //              dragtarget: "dropProxy"
        //              anchors.fill: parent
                  }

              }
          }

    }
}

//DropArea {
//    id: thisdragTarget

//    property string colorKey
//    property alias dropProxy: thisdragTarget

//    anchors.fill: parent
//    anchors.centerIn: parent
//    keys: [ "red" ]

//    Rectangle {
//        id: dropRectangle

//        anchors.fill: parent
//        color: "transparent"

//        states: [
//            State {
//                when: thisdragTarget.containsDrag
//                PropertyChanges {
//                    target: tile
//                    color: "grey"
//                }
//            }
//        ]
//    }
//}



