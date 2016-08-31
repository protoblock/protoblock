import QtQuick 2.0
import QtQuick.Controls 1.4
//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1

//import "../qml-components"
/*
Item {
    id: win
    height: parent.height
    width: parent.width
    Repeater {
        model: 10
        Rectangle {
            id: rect
            width: 50
            height: 50
            z: mouseArea.drag.active ||  mouseArea.pressed ? 2 : 1
            color: Qt.rgba(Math.random(), Math.random(), Math.random(), 1)
            x: Math.random() * (win.width / 2 - 100)
            y: Math.random() * (win.height - 100)
            property point beginDrag
            property bool caught: false
            border { width:2; color: "white" }
            radius: 5
            Drag.active: mouseArea.drag.active

            Text {
                anchors.centerIn: parent
                text: index
                color: "white"
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                drag.target: parent
                onPressed: {
                    rect.beginDrag = Qt.point(rect.x, rect.y);
                }
                onReleased: {
                    if(!rect.caught) {
                        backAnimX.from = rect.x;
                        backAnimX.to = beginDrag.x;
                        backAnimY.from = rect.y;
                        backAnimY.to = beginDrag.y;
                        backAnim.start()
                    }
                }

            }
            ParallelAnimation {
                id: backAnim
                SpringAnimation { id: backAnimX; target: rect; property: "x"; duration: 500; spring: 2; damping: 0.2 }
                SpringAnimation { id: backAnimY; target: rect; property: "y"; duration: 500; spring: 2; damping: 0.2 }
            }
        }
    }

    Rectangle {
        anchors {
            top: parent.top
            right:  parent.right
            bottom:  parent.bottom
        }
        width: parent.width / 2
        color: "gold"
        DropArea {
            anchors.fill: parent
            onEntered: drag.source.caught = true;
            onExited: drag.source.caught = false;
        }
    }
}
*/

SplitView {
    property alias scheduleView1: scheduleView1
      anchors.fill: parent
      orientation: Qt.Horizontal
//      width: parent.width

      Component.onCompleted: {
          lpv.releasedit.connect(releaseditMethod)
      }
      function releaseditMethod(fname) {
          ppt.addcolumn(fname)
          console.log(" releaseditMethod " )
      }
      Card {
          Layout.minimumWidth: parent.width * .10
          Layout.maximumWidth: parent.width * .50
//          Layout.fillWidth: true
          //          color: "lightblue"
          ScheduleView {
              id: scheduleView1
              anchors.fill: parent
          }
      }
      Card {
          Layout.minimumWidth: parent.width * .30
          Layout.maximumWidth: parent.width * .80
    //          color: "lightblue"
    //          Layout.fillWidth: true
          PlayerProjTable {
              id: ppt
              anchors.fill: parent
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
                    drag.source.caught = true;
                    myrec.color = Qt.binding( function() {
                       return "green"})

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
          id: rightr
          Layout.minimumWidth: parent.width * .20
          Layout.maximumWidth: parent.width * .60
          Layout.fillWidth: true
          LeaderProjView {
              id: lpv
//              dragtarget: "dropProxy"
              anchors.fill: parent
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



