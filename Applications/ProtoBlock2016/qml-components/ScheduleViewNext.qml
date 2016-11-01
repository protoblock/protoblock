import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import Material 1.0 as Material
//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQml.Models 2.2


Material.Card {
    id: topcn
    anchors.fill: parent
    elevation: 0
    radius: 0
    flat: true
    border.width: 0
    signal changed
    property var secsel: [["section"]]

    ListView {
        clip: true
        property bool first: true
//        anchors.fill: parent
        width: parent.width
        height: parent.height - ProtoScreen.guToPx(8)
        id: lv2
        model: MiddleMan.pNextWeekScheduleModel
        headerPositioning: ListView.OverlayHeader
        header : header
        delegate : gamedel
        section.delegate: sectionDelegate
        section.property: "time"
        section.criteria: ViewSection.FullString
        spacing: 0


    }

    Component {
        id: header
        Item {
            id: ih
            height: ProtoScreen.guToPx(8)
            width: parent.width
            property var widths: [3.0/11.0,2.0/11.0,2.0/11.0,4.0/11.0]
                RowLayout {
                    spacing: 0
                    anchors.fill: parent
                    Repeater {
                        model: [" Time "," Away "," Home "," Status "]
                        Item {
                            Layout.preferredWidth: (parent.width * ih.widths[index])
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            height: parent.height
                            Rectangle {
                                id: rec2
                                height: parent.height * .50
                                anchors.top: parent.top
                                width: parent.width
                            }

                            Material.Card{
                                anchors.bottom: parent.bottom
                                width: parent.width
                                border.color:"black"
                                backgroundColor: Colors.blue
                                height: parent.height * .50
                                Material.Label {
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
       }
    }

    Component {
        id: sectionDelegate
        Rectangle {
            width: parent.width
            height: t.height//ProtoScreen.guToPx(1.5);
            color: "#AFE1FF"//Qt.lighter(Colors.blue,1.20)
            Layout.fillHeight: true
            Layout.fillWidth: false

            Material.Label {
                id: t
                text: section
//                font.bold: true
                font.pixelSize: ProtoScreen.font(ProtoScreen.Tiny)
                verticalAlignment: Text.AlignVCenter
//                horizontalAlignment: Text.AlignHCenter
//                anchors.centerIn: parent
//                color:
//                anchors.horizontalCenterOffset:  -parent.width * .20
                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.fill: parent
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width * .20

                Component.onCompleted: {
                    secsel[section] = ""
                }
            }
        }
    }

    Component {
        id: gamedel
        Material.Card {
            id: dcard

            height: ProtoScreen.guToPx(4)
            width: parent.width
            property var widths: [3.0/11.0,2.0/11.0,2.0/11.0,4.0/11.0]


            elevation: 0
//            backgroundColor: "red" //Qt.darker("white",1.20)
            backgroundColor: "white"

        RowLayout {
            anchors.fill: parent
            spacing: 0
            id: rr

            Material.Label {
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
            Material.Label{
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
            Material.Label{
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

            Material.Label{
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
            Material.Label{
//                anchors.centerIn: parent
                text: "Scheduled"
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width * widths[3])
//                width: (parent.width * widths[3])
            }

//            Connections {
//                target: isl
//                onSelectionChanged: {
//                    elevation =  isl.isSelected(lv.model.index(index,0)) ? 5 : 0
//                    backgroundColor: (isl.isSelected(lv.model.index(index,0)) || !isl.hasSelection)  ? "white" : Qt.darker("white",1.20)
//                }
//            }

        }


    }

}
}
