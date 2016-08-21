import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Material 1.0
import ProRotoQml.Protoblock 1.0
import  ProRotoQml.Theme 1.0


Card {
//    property alias scheduleModel: lv.model
//    width: parent.width
//    height: parent.height
    anchors.margins: ProtoScreen.guToPx(1)
    anchors.fill: parent
//    width: parent.width
//    height: parent.height
//    border.color: "grey"
    elevation: 0
    ListView {
        anchors.fill: parent
        width: parent.width
//        height: parent.height
        id: lv
        model: MiddleMan.pWeeklyScheduleModel
        headerPositioning: ListView.OverlayHeader
        header: header
        delegate: gamedel
//        Text {
//                text: time + away + " @ " + home
//                height: ProtoScreen.guToPx(4)
//                width: parent.width
//            }

//            gamedel
//        focus: true
        section.delegate: sectionDelegate
        section.property: "time"
        section.criteria: ViewSection.FullString
        spacing: 0
    }

    Component {
         id: header
        RowLayout {
            spacing: 0
            height: ProtoScreen.guToPx(4)
            width: parent.width
            property var widths: [3.0/11.0,2.0/11.0,2.0/11.0,4.0/11.0]

            Repeater{
                model: [" Time "," Away "," Home ","  Status  "]
                Card{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: (parent.width * widths[index])
                    border.color:"black"
                    backgroundColor: Colors.blue
                    Label{
                        anchors.centerIn: parent
                        text: modelData
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }

    Component {
        id: sectionDelegate

        Rectangle {
            width: ListView.view.width
            height: ProtoScreen.guToPx(.25);
            color: Colors.blue
        }
    }
    Component {
        id: gamedel
        Card {
            height: ProtoScreen.guToPx(4)
            width: parent.width

//            anchors.topMargin: (ListView.section === ListView.previousSection) ?
//                                   10 : -2;
//            anchors.bottomMargin: (ListView.section === ListView.nextSection) ?
//                                   10 : -2;

//            anchors.fill: parent
            border.color: "transparent"
            border.width: 0

            elevation: 0
            property var widths: [3.0/11.0,2.0/11.0,2.0/11.0,4.0/11.0]


        RowLayout {
            anchors.fill: parent
            spacing: 0


            Label{
//                anchors.centerIn: parent
                text: time
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width * widths[0])
//                width: (parent.width * widths[0])
            }
            Label{
//                anchors.centerIn: parent
                text: away
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width * widths[1] * .90)
//                width: (parent.width * widths[1])
                ColorAnimation on color { to: TeamInfo.getPrimaryAt(away); duration: 10000 }
                font.bold: true
            }
            Label{
//                anchors.centerIn: parent
                text: "@"
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width * widths[1] * .20)
//                width: (parent.width * widths[1])
            }

            Label{
//                anchors.centerIn: parent
                text: home
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
//                Layout.fillHeight: true
//                Layout.fillWidth: false
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width * widths[2] * .90)
                ColorAnimation on color { to: TeamInfo.getPrimaryAt(home); duration: 10000 }
                font.bold: true
//                width: (parent.width * widths[2])
            }
            Label{
//                anchors.centerIn: parent
                text: status
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width * widths[3])
//                width: (parent.width * widths[3])
            }

        }
     }
//        Label {
//            text: time
//            Layout.preferredWidth: (parent.width / 4.0)
//        }
//        Label {
//            text: away
//            Layout.preferredWidth: (parent.width / 4.0)
//        }
//        Label {
//            text: home
//            Layout.preferredWidth: (parent.width / 4.0)
//        }
//        Label {
//            text: status
//            Layout.preferredWidth: (parent.width / 4.0)
//        }
//            }
//        }
//    }
//        Item {
////            elevation: marketDepthCard.elevation
//            height: ProtoScreen.guToPx(4)
//            width: parent.width

//            anchors.fill: parent
////            border.color: "black"
//            RowLayout {
//                id: rowLayout1
//                anchors.fill: parent
//                spacing: 0
//                width: parent.width
//                height: parent.height

//                Label {
//                    text: time
////                    Layout.fillHeight: true
////                    Layout.fillWidth: false
////                    Layout.preferredWidth: (parent.width / 4.0)
//                    width: (parent.width / 4.0)

//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                }

//                Label {
//                    text: away
////                    Layout.fillHeight: true
////                    Layout.fillWidth: false
////                    Layout.preferredWidth: (parent.width / 4.0)
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    width: (parent.width / 4.0)
//                }

//                Label {
//                    text: home
////                    Layout.fillHeight: true
////                    Layout.fillWidth: false
////                    Layout.preferredWidth: (parent.width / 4.0)
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    width: (parent.width / 4.0)
//                }

//                Label {
//                    text: status
////                    Layout.fillHeight: true
////                    Layout.fillWidth: false
////                    Layout.preferredWidth: (parent.width / 4.0)
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    width: (parent.width / 4.0)
//                }
//            }
//        }

}
}
