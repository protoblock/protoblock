import QtQuick 2.0
import QtQuick.Controls 1.4
//import ProRotoQml.Sql 1.0
import Material.ListItems 1.0 as ListItems
import Material 1.0
import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1



SplitView {
      anchors.fill: parent
      orientation: Qt.Horizontal
//      width: parent.width
      Card {
          width: parent.width * .20
          Layout.maximumWidth: parent.width * .50
//          color: "lightblue"
          ScheduleView {
              anchors.fill: parent
          }
      }
      Card {
          width: parent.width * .60
          Layout.maximumWidth: parent.width * .80
//          color: "lightblue"

          PlayerProjTable {
              anchors.fill: parent
          }

          DropArea {
              id: dragTarget

              property string colorKey
              property alias dropProxy: dragTarget

              anchors.fill: parent
              anchors.centerIn: parent
              keys: [ "red" ]

              Rectangle {
                  id: dropRectangle

                  anchors.fill: parent
                  color: "transparent"

                  states: [
                      State {
                          when: dragTarget.containsDrag
                          PropertyChanges {
                              target: dropRectangle
                              color: "grey"
                          }
                      }
                  ]
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
      Rectangle {
          id: rightp
          width: parent.width * .20
          Layout.maximumWidth: parent.width * .50
          MouseArea {
              id: mouseArea

//              anchors.fill: parent
              anchors.centerIn: parent
              width: parent.width * .30
              height: parent.height / 2.0
              drag.target: tile

              onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : dragTarget

              Rectangle {
                  id: tile

                  anchors.fill: parent
                  anchors.verticalCenter: parent.verticalCenter
                  anchors.horizontalCenter: parent.horizontalCenter

                  color: "red"

                  Drag.keys: [ "red" ]
                  Drag.active: mouseArea.drag.active
//                  Drag.hotSpot.x: parent.x
//                  Drag.hotSpot.y: 32
                  states: State {
                      when: mouseArea.drag.active
                      ParentChange { target: tile; parent: rightp }
                      AnchorChanges { target: tile; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
                  }

              }
          }
      }
  }
