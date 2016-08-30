import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
import ProRotoQml.Protoblock 1.0


Item {
    id: topi

    property string dragtarget: "dropProxy"

    anchors.fill: parent

    ComboBox {
        model: 16
        enabled: true
        currentIndex: 0
        onCurrentTextChanged: {
//           MiddleMan.pProjectionsViewFilterProxyModel.setPos(currentText)
        }
        id: cb
    }

    Item {
        anchors.top: cb.bottom
        width: parent.width
        height: parent.height - cb.height

        TableView {
            id: tv
            Component.onCompleted: {
                resizeColumnsToContents()
            }

            highlightOnFocus:   false
            anchors.fill: parent

            selectionMode: SelectionMode.SingleSelection

//            anchors.top: parent.top
//            width: parent.width
//            height: parent.heigh//t - tv0.height
            model: MiddleMan.pFantasyNameBalModel
//            sortIndicatorVisible: true
//            sortIndicatorOrder: Qt.AscendingOrder
//            onSortIndicatorColumnChanged: {
//    //            accountErrorDialog.show();
//    //            MiddleMan.pProjectionsViewFilterProxyModel.sort(sortIndicatorColumn, sortIndicatorOrder)
//                MiddleMan.pProjectionsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
//            }

//            onSortIndicatorOrderChanged: {
//    //            MiddleMan.pProjectionsViewFilterProxyModel.sort(sortIndicatorColumn, sortIndicatorOrder)

//                MiddleMan.pProjectionsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
//            }

            headerDelegate:  Item {
//                implicitWidth: textItem2.implicitWidth / 2.0
            width: parent.width
//            height: ProtoScreen.guToPx(3)

//            Rectangle {
//                id: rec
//                height: parent.height * .60
//                width: parent.width
//                color: "white"
//                anchors.top: parent.top
//            }

            Card {
                width: parent.width
                height: parent.height * .40
                backgroundColor: themeroot.theme.primaryColor
                anchors.bottom: parent.bottom
                radius: 0
                border.color:
                    styleData.column  === tv.sortIndicatorColumn ? "green" : "black"


                Label{
                    id: textItem2
                    text: " " + styleData.value + " "
                    anchors.fill: parent
    //                width: parent.width
    //                height: parent.height * .40
    //                anchors.bottom: parent.bottom
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }
            }
            }

            frameVisible: true
            TableViewColumn {
                role: "name"
                title: "xFantasy Name"
                horizontalAlignment : Text.AlignHCenter



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


                delegate: Item {
                    id: win
                    anchors.fill: parent
                    height: tx.height
                    Rectangle {
                        id: rect
                        height: parent.height
                        width: parent.width
//                        anchors.fill: parent
//                        anchors.centerIn: parent;
                        z: mouseArea.drag.active ||  mouseArea.pressed ? -20 : -10
                        color: "Grey"
                        property point beginDrag
                        property bool caught: false
                        border { width:1; color: "white" }
                        Drag.active: mouseArea.drag.active

                        Label {
                            id: tx
//                            anchors.fill: parent
                            anchors.centerIn: parent
                            text: styleData.value
//                            style: Text.Raised
                            color: "white"
                            font.pixelSize: ProtoScreen.font(ProtoScreen.TINY)
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
//                                rect.x += mouseX - startX
//                                rect.y += mouseY - startY
//                                startX = mouseX
//                                startY = mouseY
                            }

                            onPressed: {
                                startX = mouseX
                                startY = mouseY
                                console.log(" mouse pressed")
                                rect.beginDrag = Qt.point(rect.x, rect.y);
                            }

                            onReleased: {
                                 console.log(" mouse released   " + rect.caught)
                                 if(rect.caught) {
                                    backAnimX.duration = 4000;
                                    backAnimY.duration = 4000;
                                     backAnimX.spring = .1;
                                     backAnimY.spring = .1;
                                     rect.color = Qt.binding( function() {
                                        return "transparent"
                                     })
                                     tx.color = Qt.binding( function() {
                                        return "grey"
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
