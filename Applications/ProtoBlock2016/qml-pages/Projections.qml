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
          width: parent.width * .20
          Layout.maximumWidth: parent.width * .50
          Text {
              text: "View 3"
              anchors.centerIn: parent
          }
      }
  }
