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
    property int week: stack.currentItem.objectName === "prevWeekS" ? MiddleMan.thePrevWeek : MiddleMan.theWeek
    property string seasontext: MiddleMan.seasonString + " 2016 - Week "
    property string liveorresult: stack.currentItem.objectName === "prevWeekS" ? "Result" : MiddleMan.liveSync

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

//        RowLayout {
//            id: cBan
//            height: ProtoScreen.guToPx(6)
//            width: parent.width
//            Layout.alignment: Qt.AlignHCenter
        IconButton {
            anchors.right: cBan.left
            Layout.fillHeight: true
            Layout.fillWidth: false
//                Layout.preferredWidth: ProtoScreen.guToPx(12)

            id: left
//                width: ProtoScreen.guToPx(6)
//                height: parent.height


            size: ProtoScreen.guToPx(5)

            enabled: stack || stack.currentItem.objectName === "pptS" || MiddleMan.thePrevWeek > 1

            action: Action {
                iconName: "awesome/caret_left"
//                    hoverAnimation: true
                tooltip: "Previous"
            }
            onClicked : {
                console.log( "left" + stack.currentItem.objectName)
                if ( stack.currentItem.objectName === "pptS" ) {
                    stack.pop();
                    MiddleMan.setPrevWeekData(MiddleMan.theWeek-1)
                }
                else if ( stack.currentItem.objectName === "nextWeekS" )
                    stack.pop();
                else
                    MiddleMan.setPrevWeekData(MiddleMan.thePrevWeek-1)
            }
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: seasontext + week + " - " + liveorresult
//                                            (stack && stack.currentItem.objectName === "prevWeekS") ?
//                                                (MiddleMan.thePrevWeek + " - Results") :
//                                                (MiddleMan.theWeek + " - " + MiddleMan.liveSync);

            font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
            color: themeroot.theme.primaryColor
            Layout.fillHeight: true
            Layout.fillWidth: false
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            id: cBan
        }
        IconButton {
            anchors.left: cBan.right
            Layout.fillHeight: true
            Layout.fillWidth: false
//                Layout.preferredWidth: ProtoScreen.guToPx(12)

            id: right
//                width: ProtoScreen.guToPx(6)
//                height: parent.height


            size: ProtoScreen.guToPx(5)

            action: Action {
                iconName: "awesome/caret_right"
//                    hoverAnimation: true
                tooltip: "Previous"
            }
            enabled: stack && stack.currentItem.objectName === "prevWeekS"
            onClicked : {
                console.log( "right" + stack.currentItem.objectName + prevWeekS)
                if ( stack.currentItem.objectName === "prevWeekS") {
                    if ( MiddleMan.thePrevWeek === MiddleMan.theWeek-1)
                        stack.push({item: pptS, properties:{objectName:"pptS"}});
                    else
                        MiddleMan.setPrevWeekData(MiddleMan.thePrevWeek+1)
                }
                else if ( stack.currentItem.objectName === "pptS" )
                    stack.push({item: nextWeekS, properties:{objectName:"nextWeekS"}})
            }
        }

        SystemPalette { id: pal }

        SplitView {
            handleDelegate: handeldel

            property alias scheduleView1: scheduleView1
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

            Item {
                id: leftitem
                Layout.minimumWidth: parent.width * .40
                Layout.maximumWidth: parent.width * .80
                Layout.fillWidth: true
                StackView {
//                    width: parent.width
//                    height: parent.height
                    id: stack
                    //  initialItem: ppt
                    anchors.fill: parent

                    Component.onCompleted: {
                       stack.push({item: prevWeekS, properties:{objectName:"prevWeekS"}})
                       stack.push({item: pptS, properties:{objectName:"pptS"}})
                    }

                    Item {
                        id: pptS
                        SplitView {
                            orientation: Qt.Horizontal
                            handleDelegate: handeldel
                            width: parent.width
                            height: parent.height
                            Card {
                                anchors.leftMargin: 10
                                Layout.minimumWidth: parent.width * .20
                                Layout.maximumWidth: parent.width * .40

                                ScheduleView {
                                    id: scheduleView1
                                }
                            }
                            Card {
                                Layout.minimumWidth: parent.width * .50
                                Layout.maximumWidth: parent.width * .90
                                Layout.fillWidth: true

                                PlayerProjTable {
                                    id: ppt
                                    who: "ppt"
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
                        }
                    }
                    Item {
                        id: prevWeekS
                        width: parent.width
                        height: parent.height
                        SplitView {
                            orientation: Qt.Horizontal
                            handleDelegate: handeldel
                            width: parent.width
                            height: parent.height
                            Card {
                                anchors.leftMargin: 10
                                Layout.minimumWidth: parent.width * .20
                                Layout.maximumWidth: parent.width * .40

                                ScheduleViewPrev {
                                    id: scheduleView2
                                }
                            }
                            Card {
                                Layout.minimumWidth: parent.width * .50
                                Layout.maximumWidth: parent.width * .90
                                Layout.fillWidth: true

                                PlayerResultTable {
                                    id: prevPPT
                                    who: "prev"
                                }
                           }
                        }
                    }
                    Item {
                        id: nextWeekS
                        SplitView {
                            orientation: Qt.Horizontal
                            handleDelegate: handeldel
                            width: parent.width
                            height: parent.height
                            Card {
                                anchors.leftMargin: 10
                                Layout.minimumWidth: parent.width * .10
                                Layout.maximumWidth: parent.width * .30

                                ScheduleView {
                                    id: scheduleView3
                                }
                            }
                            Card {
                                Layout.minimumWidth: parent.width * .30
                                Layout.maximumWidth: parent.width * .80
                                Layout.fillWidth: true
                                ScheduleView {
                                    id: scheduleView4
                                }
                            }
                        }
                    }
                }
            }
            Card {
                anchors.leftMargin: 10
                id: rightr
                Layout.minimumWidth: parent.width * .20
                Layout.maximumWidth: parent.width * .60
                LeaderProjView {
                    id: lpv
                }
            }
        }
    }

//                StackView {
//                    id: stack
////                    initialItem: ppt
//                    anchors.fill: parent

//                    PlayerProjTable {
//                        id: ppt
//                        who: "ppt"
//                    }

//                    PlayerProjTable {
//                        id: prevWeek
//                        who: "prevWeek"
//                    }

//                    PlayerProjTable {
//                        id: nextWeek
//                        who: "nextWeek"
//                    }

//                    Component.onCompleted: {
//                       stack.push({item: prevWeek})
//                       stack.push({item: ppt})
//                    }
//                }
//                }
//            }

    Component {
        id: handeldel

        Item {
            height: parent.height-cBan.height
            anchors.margins: 0
            Rectangle {
                border.width: 0
                id: rec
                width: styleData.hovered ? ProtoScreen.guToPx(.25) : ProtoScreen.guToPx(.1)
                color: styleData.hovered ? "black" : Qt.darker(pal.window, 1.5)
                height: parent.height
                anchors.right: rec3.left
            }
            Rectangle {
                border.width: 0
                id: rec3
                width: styleData.hovered ? ProtoScreen.guToPx(.35) : ProtoScreen.guToPx(.1)

                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                color: styleData.hovered ? "white" : Qt.darker(pal.window, 1.5)
            }
            Rectangle {
               border.width: 0
               id: rec2
               width: styleData.hovered ? ProtoScreen.guToPx(.25) : ProtoScreen.guToPx(.1)

               height: parent.height
               anchors.left: rec3.right
               color: styleData.hovered ? "black" : Qt.darker(pal.window, 1.5)
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



