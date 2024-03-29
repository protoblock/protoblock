import QtQml.Models 2.2
import QtQml 2.2
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Material 1.0 as Material
import ProRotoQml.Theme 1.0

Item {
    id: topc
    anchors.fill: parent
    signal changed
    property var isl: MiddleMan.pPrevQItemSelectionModel
    property var secsel: [["section"]]

    Material.Card {
        anchors.fill: parent
        elevation: 0
        radius: 0
        flat: true
        border.width: 0

        ListView {
            clip: true
            property bool first: true
            width: parent.width
            height: parent.height //- ProtoScreen.guToPx(8)
            id: lv
            model: MiddleMan.pPreviousWeekScheduleModel
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

                Rectangle {
                    color: "transparent"
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height * .5;
                    anchors.top: parent.top

                    Material.Label {
                        anchors.centerIn: parent
                        text: "Week " + week + " Schedule";
                        font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                        color: "green"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                RowLayout {
                    spacing: 0
                    anchors.fill: parent
                    ExclusiveGroup { id: tabPositionGroup }
                    Repeater {
                        model: [" Time "," Away "," Home "," Status "]
                        Item {
                            Layout.preferredWidth: (parent.width * ih.widths[index])
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            height: parent.height

                            Material.Card{
                                anchors.bottom: parent.bottom
                                width: parent.width
                                border.color:"black"
                                backgroundColor: themeroot.theme.primaryColor
                                height: parent.height * .50
                                Material.Label {
                                    anchors.centerIn: parent
                                    text: modelData
                                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                    color: "white"
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
    //                                    console.log( " header clicked");
                                        secsel = [["section"]]
                                        isl.clear()
                                        topc.changed()
                                    }
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
                    font.pixelSize: ProtoScreen.font(ProtoScreen.Tiny)
                    verticalAlignment: Text.AlignVCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * .20
                    Component.onCompleted: {
                        secsel[section] = ""
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if ( secsel[section] === "on")
                            secsel[section] = "off"
                        else secsel[section] = "on"
                        topc.changed()
                    }
                }
            }
        }

        Component {
            id: gamedel
            Material.Card {
                id: dcard

                Component.onCompleted: {
                    topc.changed.connect(myMethod)
                }

                Component.onDestruction: {
                    topc.changed.disconnect(myMethod)
                }

                height: ProtoScreen.guToPx(4)
                width: parent.width

                property var widths: [3.0/11.0,2.0/11.0,2.0/11.0,4.0/11.0]


                elevation: isl.isSelected(lv.model.index(index,0)) ? 1 : 0
                backgroundColor: (isl.isSelected(lv.model.index(index,0)) || !isl.hasSelection)  ? (index % 2 == 0 ?"#f5f5f5":"white") : Qt.darker("white",1.20)

                RowLayout {
                    anchors.fill: parent
                    spacing: 0
                    id: rr

                    Material.Label {
                        text: time
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillHeight: true
                        Layout.fillWidth: false
                        Layout.preferredWidth: (parent.width * widths[0])
                    }
                    Material.Label{
                        text: away
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillHeight: true
                        Layout.fillWidth: false
                        Layout.preferredWidth: (parent.width * widths[1] * .90)
                        ColorAnimation on color { to: TeamInfo.getPrimaryAt(away); duration: 10000 }
                        font.bold: true
                    }
                    Material.Label{
                        text: "@"
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillHeight: true
                        Layout.fillWidth: false
                        Layout.preferredWidth: (parent.width * widths[1] * .20)
                    }

                    Material.Label{
                        text: home
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillHeight: true
                        Layout.fillWidth: false
                        Layout.preferredWidth: (parent.width * widths[2] * .90)
                        ColorAnimation on color { to: TeamInfo.getPrimaryAt(home); duration: 10000 }
                        font.bold: true
                    }
                    Material.Label{
                        text: "Closed"
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillHeight: true
                        Layout.fillWidth: false
                        Layout.preferredWidth: (parent.width * widths[3])
                    }

                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        MiddleMan.toggleP(index,ItemSelectionModel.Toggle)
                        dcard.elevation = Qt.binding(
                            function() {
                                if ( isl.isSelected(lv.model.index(index,0)) )
                                    return 1;
                                else
                                    return 0
                            }
                        )

                        dcard.backgroundColor = Qt.binding(
                            function() {
                                if ( isl.isSelected(lv.model.index(index,0)) || !isl.hasSelection )
                                    return index % 2 == 0 ?"#f5f5f5":"white";
                                else
                                    return Qt.darker("white",1.20)
                            }
                        )

                        topc.secsel[time] = ""//Qt.binding( function() { return "" } )
                    }

                }


                function myMethod() {
                    if ( secsel[time] === "on" && !isl.isSelected(lv.model.index(index,0)))
                        MiddleMan.toggleP(index,ItemSelectionModel.Toggle)
                    else if ( secsel[time] === "off" && isl.isSelected(lv.model.index(index,0)) )
                        MiddleMan.toggleP(index,ItemSelectionModel.Toggle)

                    dcard.elevation = Qt.binding(
                        function() {
                            if ( isl.isSelected(lv.model.index(index,0)) )
                                return 1;
                            else
                                return 0
                        }
                    )

                    dcard.backgroundColor = Qt.binding(
                        function() {
                            if ( isl.isSelected(lv.model.index(index,0)) || !isl.hasSelection )
                                return index % 2 == 0 ?"#f5f5f5":"white";
                            else
                                return Qt.darker("white",1.20)
                        }
                    )

                }
            }
        }
    }
}
