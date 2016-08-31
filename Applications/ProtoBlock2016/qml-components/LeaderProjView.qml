import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
import ProRotoQml.Protoblock 1.0


Item {
    id: topi

    signal releasedit(string fname)
    property string dragtarget: "dropProxy"

    anchors.fill: parent
    anchors.margins: ProtoScreen.guToPx(1)
    Item {
//        anchors.top: cb.bottom
        width: parent.width
        height: parent.height //- cb.height

        TableView {
            id: tv
            Component.onCompleted: {
                resizeColumnsToContents()
            }

            highlightOnFocus:   false
            anchors.fill: parent

            selectionMode: SelectionMode.NoSelection
            model: MiddleMan.pFantasyNameBalModel
            sortIndicatorVisible: true
            sortIndicatorOrder: Qt.AscendingOrder
            onSortIndicatorColumnChanged: {
//                MiddleMan.pFantasyNameBalModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            onSortIndicatorOrderChanged: {
//                MiddleMan.pFantasyNameBalModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }



            headerDelegate:  Rectangle {
                width: parent.width
                height: ProtoScreen.guToPx(6)
                color: "white"
//                anchors.fill: parent
                Card {
                    width: parent.width
                    height: parent.height * .40
                    backgroundColor: themeroot.theme.primaryColor
                    anchors.bottom: parent.bottom
                    radius: 0
                    border.color:
                        styleData.column  === tv.sortIndicatorColumn ? "green" : "black"

                    Label {
                        id: textItem2
                        text: " " + styleData.value + " "
                        anchors.fill: parent
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        elide: Text.ElideRight
                        wrapMode: Text.WordWrap
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: "white"
                    }
                }
            }

            frameVisible: false

            //                width: tv.width / 4.0
            //                    width: tv.width / 4.0
            //                    height: ProtoScreen.guToPx(6)
            //                        height: ProtoScreen.guToPx(3)
            //                        width: win.width * .75
            //Qt.rgba(Math.random(), Math.random(), Math.random(), 1)
//                    x: Math.random() * (win.width / 2 - 100)
//                    y: Math.random() * (win.height - 100)
            //                        radius: 1
            //                        anchors.fill: parent


            TableViewColumn {
                role: "name"
                title: "xFantasy Name"
                horizontalAlignment : Text.AlignHCenter

                delegate: Item {
                    id: win
                    anchors.fill: parent
                    height: tx.height
                    //                        anchors.fill: parent
                    //                        anchors.centerIn: parent;
                    Rectangle {
                        id: rect
                        height: parent.height
                        width: parent.width
//                        z: mouseArea.drag.active || mouseArea.pressed ? -20 : -10
                        color: "grey"
                        property point beginDrag
                        property bool caught: false
                        property string fname: styleData.value
                        border { width:1; color: "white" }
                        Drag.active: mouseArea.drag.active

                        //                            anchors.fill: parent
                        //                            style: Text.Raised
                        Label {
                            id: tx
                            anchors.centerIn: parent
                            text: styleData.value
                            color: "white"
                            font.bold: true
                            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            drag.target: rect
                            onClicked: { console.log(" mouse click")}
                            onHoveredChanged: {console.log(" mouse hover")}

                            property int startX
                            property int startY

                            onPositionChanged: {
                                console.log(" mouse onPositionChanged   " + rect.caught)
//                                                                rect.x += mouseX - startX
//                                                                rect.y += mouseY - startY
//                                                                startX = mouseX
//                                                                startY = mouseY
                            }

                            onPressed: {
                                startX = mouseX
                                startY = mouseY
                                console.log(" mouse pressed")
                                rect.beginDrag = Qt.point(rect.x, rect.y);
                            }

                            //                                 MiddleMan.newProjColumn(tx.text)

                            onReleased: {
                                 console.log(" mouse released   " + rect.caught)
                                 if(rect.caught) {
                                    lpv.releasedit(styleData.value)
                                    backAnimX.duration = 4000;
                                    backAnimY.duration = 4000;
                                    backAnimX.spring = .1;
                                    backAnimY.spring = .1;
                                    rect.color = Qt.binding( function() {
                                        return "transparent"
                                    })
                                     tx.color = Qt.binding( function() {
                                        return "black"
                                     })
                                 }
                                 backAnimX.from = rect.x;
                                 backAnimX.to = rect.beginDrag.x;
                                 backAnimY.from = rect.y;
                                 backAnimY.to = rect.beginDrag.y;
                                 backAnim.start()
                            }
                        }

                        ParallelAnimation {
                            id: backAnim
                            SpringAnimation { id: backAnimX; target: rect; property: "x"; duration: 500; spring: 2; damping: 0.2 }
                            SpringAnimation { id: backAnimY; target: rect; property: "y"; duration: 500; spring: 2; damping: 0.2 }
                        }
                    }
                }
            }
            TableViewColumn{
                role: "stake"
                title: "2016 Rank"
                horizontalAlignment : Text.AlignHCenter

            }
            TableViewColumn{
                role: "bits"
                title: "Wk 1 rank"
                horizontalAlignment : Text.AlignHCenter

            }
            TableViewColumn{
                role: "index"
                title: "#"
                horizontalAlignment : Text.AlignHCenter
            }
            TableViewColumn{
                role: "time"
                title: "Recent"
            }
        }
    }
}


//                    Item {
//                    anchors.fill: parent
//                    id: rootr

//                    MouseArea {
//                        id: mouseArea

//                        anchors.fill: parent
//                        anchors.centerIn: parent
//                        drag.target: dragTarget

//                        onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : rootr

//                        Rectangle {
//                            id: tile

//                            anchors.fill: parent
//                            anchors.verticalCenter: parent.verticalCenter
//                            anchors.horizontalCenter: parent.horizontalCenter

//                            color: "green"

//                            Drag.keys: [ "red" ]
//                            Drag.active: mouseArea.drag.active
//                            states: State {
//                                when: mouseArea.drag.active
//                                ParentChange { target: tile; parent: dragTarget }
//                                AnchorChanges { target: tile; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
//                                PropertyChanges {
//                                    target: tile
//                                    color: "transparent"
//                                }
//                            }

//                        }
//                    }
//                }

//            }
