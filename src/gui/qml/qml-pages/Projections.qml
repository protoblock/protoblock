import QtQml.Models 2.2
import QtQml 2.2
import QtQuick 2.5
import QtQuick.Controls 1.4
import Material.ListItems 1.0 as ListItems
import Material 1.0
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.3


Item {
    id: topi
    property int week: (MiddleMan.theWeek === 0 && MiddleMan.theSeason === 0) || !stack || !stack.currentItem ? 0 :
                          (stack.currentItem.objectName === "prevWeekS" ? MiddleMan.thePrevWeek : (stack.currentItem.objectName === "nextWeekS" ? MiddleMan.theNextWeek : MiddleMan.theWeek))
    property string seasontext: !stack.currentItem ? "!stack" : (
                (stack.currentItem.objectName === "prevWeekS" ? "NFL Season " + MiddleMan.thePrevSeason
                                                              : (stack.currentItem.objectName === "nextWeekS" ? "NFL Season " + MiddleMan.theNextSeason
                                                              : MiddleMan.seasonString + " " + MiddleMan.theSeason))
                                                                    + " - Week ")

    property string liveorresult: MiddleMan.liveSync === "Sync" || !stack || !stack.currentItem ? "Sync" :
                                       (stack.currentItem.objectName === "prevWeekS" ? "Result" :
                                         (stack.currentItem.objectName === "nextWeekS" ? "Schedule" : MiddleMan.liveSync))

    Component.onCompleted: {
//        console.log(" proj wisth" + parent.width + " 2 " + rootLoader.width + " 3 " + themeroot.width + " 4 " + realRoot.width + " 5 " + pageHelper.width)
    }



    Card {
        radius: 0
        flat: true

        id: topcard
        width: parent.width
        height: parent.height
        anchors{
            top: parent.top
//                topMargin:ProtoScreen.guToPx(2)
//                bottomMargin:ProtoScreen.guToPx(1)
            horizontalCenter: parent.horizontalCenter
//                margins: ProtoScreen.guToPx(.125)
        }
        backgroundColor: "white"

        Rectangle {
            height: cBan.height - ProtoScreen.guToPx(1)
            width: ProtoScreen.guToPx(6)
            anchors.right: cBan.left
            anchors.rightMargin: ProtoScreen.guToPx(4)
            color: "gray" // Theme.light.textColor//themeroot.theme.secondaryColor
            radius: 2
            anchors.verticalCenter: cBan.verticalCenter


            IconButton {
//                anchors.right: cBan.left
//                Layout.fillHeight: true
//                Layout.fillWidth: false
    //                Layout.preferredWidth: ProtoScreen.guToPx(12)

                id: left
    //                width: ProtoScreen.guToPx(6)
    //                height: parent.height

                color: "white"
                anchors.centerIn: parent
                size: ProtoScreen.guToPx(5)

                enabled:  MiddleMan.liveSync !== "Sync" && stack && stack.currentItem && (MiddleMan.thePrevWeek > 1 || MiddleMan.thePrevSeason > 2014)

                action: Action {
                    iconName: "awesome/caret_left"
    //                    hoverAnimation: true
//                        name: "Week" + string(MiddleMan.thePrevWeek-1) //+ (stack.currentItem.objectName === "pptS" && MiddleMan.thisWeekPrev )
//                                        ? MiddleMan.theWeek : MiddleMan.thePrevWeek-1
                }
                onClicked : {
                    console.log( "left" + stack.currentItem.objectName)
                    if ( stack.currentItem.objectName === "pptS" ) {
                        if ( MiddleMan.thisWeekPrev)
                            MiddleMan.setPrevWeekData(MiddleMan.theWeek,MiddleMan.theSeason)
                        else
                            MiddleMan.setPrevWeekData(MiddleMan.theWeek-1,MiddleMan.theSeason)
                        stack.pop();
                    }
                    else if ( stack.currentItem.objectName === "nextWeekS" ) {
                        if ( MiddleMan.theNextWeek-1 ===  MiddleMan.theWeek)
                            stack.pop();
                        else
                            MiddleMan.setNextWeekData(MiddleMan.theNextWeek-1 )
                    }
                    else
                        MiddleMan.setPrevWeekData(MiddleMan.thePrevWeek-1,MiddleMan.thePrevSeason)
                }
            }
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: seasontext + week + " " + liveorresult
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

        Rectangle {
            height: cBan.height - ProtoScreen.guToPx(1)
            width: ProtoScreen.guToPx(6)
            anchors.left: cBan.right
            anchors.leftMargin: ProtoScreen.guToPx(4)
            color: "gray" // Theme.light.textColor//themeroot.theme.secondaryColor
            radius: 2
            anchors.verticalCenter: cBan.verticalCenter


            IconButton {
//                anchors.right: cBan.left
//                Layout.fillHeight: true
//                Layout.fillWidth: false
    //                Layout.preferredWidth: ProtoScreen.guToPx(12)

                id: right
    //                width: ProtoScreen.guToPx(6)
    //                height: parent.height

                color: "white"
                anchors.centerIn: parent
                size: ProtoScreen.guToPx(5)

                action: Action {
                    iconName: "awesome/caret_right"
    //                    hoverAnimation: true
                    tooltip: "Next Week"
                }
                enabled: MiddleMan.liveSync !== "Sync" && stack && stack.currentItem &&
                         (
                             (stack.currentItem.objectName === "prevWeekS") ||
                             (stack.currentItem.objectName === "pptS" &&
                              MiddleMan.theWeek < MiddleMan.lastWeekForSeason(MiddleMan.theSeason)) ||
                             (stack.currentItem.objectName === "nextWeekS" &&
                              (MiddleMan.theNextWeek < MiddleMan.lastWeekForSeason(MiddleMan.theNextSeason) ||
                               MiddleMan.theSeason > MiddleMan.theNextSeason))
                         )

//                enabled:  MiddleMan.liveSync !== "Sync" && stack && stack.currentItem && MiddleMan.thePrevWeek > 1
                onClicked : {
                    console.log( "right" + stack.currentItem.objectName + prevWeekS)
                    if ( stack.currentItem.objectName === "prevWeekS") {
                        if ( (!MiddleMan.thisWeekPrev && MiddleMan.thePrevWeek === MiddleMan.theWeek-1)
                                ||
                             (MiddleMan.thisWeekPrev && MiddleMan.thePrevWeek === MiddleMan.theWeek)
                                ||
                             (  MiddleMan.thePrevWeek === MiddleMan.lastWeekForSeason(MiddleMan.thePrevSeason) &&
                                MiddleMan.thePrevSeason === MiddleMan.theSeason-1 &&
                                (MiddleMan.theWeek < 1 || (MiddleMan.theWeek === 1 && !MiddleMan.thisWeekPrev ))
                             )
                                ||
                             (  MiddleMan.thePrevWeek === MiddleMan.lastWeekForSeason(MiddleMan.thePrevSeason) &&
                                MiddleMan.thePrevSeason === MiddleMan.theSeason &&
                                MiddleMan.theWeek > MiddleMan.lastWeekForSeason(MiddleMan.theSeason)
                             )
                            )
                            stack.push({item: pptS, properties:{objectName:"pptS"}});
                        else {
                            MiddleMan.setPrevWeekData(MiddleMan.thePrevWeek+1,MiddleMan.thePrevSeason)
                        }
                    }
                    else if ( stack.currentItem.objectName === "pptS" ) {
                        stack.push({item: nextWeekS, properties:{objectName:"nextWeekS"}})
                        MiddleMan.setNextWeekData(MiddleMan.theWeek+1);
                    }
                    else if ( stack.currentItem.objectName === "nextWeekS"  )
                        MiddleMan.setNextWeekData(MiddleMan.theNextWeek+1)
                }
            }
        }

        SystemPalette { id: pal }

        StackView {
            anchors.top: cBan.bottom
            anchors.topMargin: ProtoScreen.guToPx(.25)
            width: parent.width
            height: parent.height - cBan.height
            id: stack
            initialItem: pptS

            Component.onCompleted: {
               stack.push({item: prevWeekS, properties:{objectName:"prevWeekS"}})
               stack.push({item: pptS, properties:{objectName:"pptS"}})

               console.log( " stack " + width)
            }

//            Component.onStatusChanged: {
//                console.log( " stack  onStatusChanged" )
//            }

            Item {
                id: pptS
                width: parent.width
                height: parent.height
                SplitView {
                    orientation: Qt.Horizontal
                    handleDelegate: handeldel
                    width: parent.width
                    height: parent.height
                    Component.onCompleted: {
                        lpv.releasedit.connect(releaseditMethod)
                        console.log( " split " + width)
                    }
                    function releaseditMethod(fname) {
                        ppt.addcolumn(fname)
                        console.log(" releaseditMethod " )
                    }
                    Card {
                        id: schedr
                        anchors.leftMargin: 10
                        Layout.minimumWidth: parent.width * .10
                        Layout.maximumWidth: parent.width * .30

                        ScheduleView {
                            id: scheduleView1
                        }
                    }
                    Card {
                        Layout.minimumWidth: parent.width * .30
                        Layout.maximumWidth: parent.width * .80
                        Layout.fillWidth: true

                        id: projr
                        PlayerProjTable {
                            id: ppt
//                            who: "ppt"
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
                    Card {
                        anchors.leftMargin: 10
                        id: rightr
                        Layout.minimumWidth: parent.width * .30
                        Layout.maximumWidth: parent.width * .60
                        LeaderProjView {
                            id: lpv
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
                        id: sched2
                        anchors.leftMargin: 10
                        Layout.minimumWidth: parent.width * 0
                        Layout.maximumWidth: parent.width * .30

                        ScheduleViewPrev {
                            id: scheduleView2
                        }
                    }
                    Card {
                        id: proj2
                        Layout.minimumWidth: parent.width * .30
                        Layout.maximumWidth: parent.width * 1
                        Layout.fillWidth: true
                        PlayerResultTable {}
                    }

                    Card {
                        anchors.leftMargin: 10
                        id: rightr2
                        Layout.minimumWidth: parent.width * 0
                        Layout.maximumWidth: parent.width * .70
                        LeaderAwardView {
                            id: lav
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
                        id: sched3
                        anchors.leftMargin: 10
                        Layout.minimumWidth: parent.width * .20
                        Layout.maximumWidth: parent.width * .40

                        ScheduleViewNext {
                            id: scheduleView3
                        }
                    }
                    Card {
                        id: proj3
                        Layout.minimumWidth: parent.width * .30
                        Layout.maximumWidth: parent.width * .50
                        Layout.fillWidth: true
                    }
                    Card {
                        anchors.leftMargin: 10
                        id: rightr3
                        Layout.minimumWidth: parent.width * .30
                        Layout.maximumWidth: parent.width * .80
                        LeaderProjView {
                            id: lpv3
                        }
                    }
                }
            }
        }
    }

    Component {
        id: handeldel

        Item {
            height: parent.height//-cBan.height
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





