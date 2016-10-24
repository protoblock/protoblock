import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import Material 1.0 as Material
//import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import QtQml.Models 2.2


Material.Card {
    id: topc
//    property alias scheduleModel: lv.model
//    width: parent.width
//    height: parent.height
//    anchors.margins: ProtoScreen.guToPx(1)
    anchors.fill: parent
//    width: parent.width
//    height: parent.height
//    border.color: "grey"
    elevation: 0
    radius: 0
    flat: true
    border.width: 0
    signal changed
    property var isl: MiddleMan.pQItemSelectionModel
    property var secsel: [["section"]]
    property string statusfilter: "Scheduled"
//    ItemSelectionModel {
//        id: isl
//        model: MiddleMan.pWeeklyScheduleModel

//        onCurrentRowChanged: {
//            console.log(" isl onCurrentRowChanged" + isl.selection)
//        }
//        onCurrentIndexChanged: {
//            console.log(" isl onCurrentIndexChanged" + isl.selection)
//        }
//        onSelectifnged: {
//            console.log(" isl selection changed" + isl.selection)
//            console.log(" isl selection isSelected " + isl.isSelected(lv.model.index(currentIndex,0)))
//        }

//        onModelChanged: {
//            console.log(" isl onModelChanged changed" )
//        }
//    }

    ListView {
//        selection: ItemSelectionModel {
//            model: MiddleMan.pWeeklyScheduleModel
//        }
        clip: true
        property bool first: true
//        anchors.fill: parent
        width: parent.width
        height: parent.height - ProtoScreen.guToPx(8)
        id: lv
        model: MiddleMan.pWeeklyScheduleModel
//        MiddleMan.pWeeklyScheduleModel
//        selectionModel: MiddleMan.pQItemSelectionModel
        headerPositioning: ListView.OverlayHeader
        header : header
        delegate : gamedel
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

        onCurrentIndexChanged: {
//            if ( first && isl.hasSelection ) {
//               console.log(" lv ci " + currentIndex + " in in ")
//               first = false;
//                forceLayout()
//            }

            console.log(" lv ci " + currentIndex + " first" + first + " has " + isl.hasSelection)
//            isl.select(currentIndex, ItemSelectionModel.ToggleCurrent)
//            for ( var i in isl.selectedIndexes )
//                console.log( " isl " + i.row())

//            console.log( " isl " + isl.hasSelection)

//            isl.select(model.index(currentIndex,0),isl.isSelected(lv.model.index(currentIndex,0)) ?ItemSelectionModel.Deselect : ItemSelectionModel.Select);
//            MiddleMan.pQItemSelectionModel.select(model.index(currentIndex,0),ItemSelectionModel.ToggleCurrent)
//            MiddleMan.pQItemSelectionModel.select(currentIndex,currentIndex)
//            MiddleMan.pQItemSelectionModel.reset()


        }


    }

//     | ItemSelectionModel.Rows
    Component {
        id: header
        Item {
            id: ih
            height: ProtoScreen.guToPx(8)
            width: parent.width
            property var widths: [3.0/11.0,2.0/11.0,2.0/11.0,4.0/11.0]

//            GroupBox {
//                id: group2
//                height: parent.height * .50
//                anchors.left: parent.left
//                anchors.horizontalCenter: scheduleView1.horizontalCenter
//                anchors.verticalCenter: parent.parent.verticalCenter
//                title: qsTr("Which Games onCopy?")
//                Layout.fillWidth: true
//                RowLayout {
//                    ExclusiveGroup { id: tabPositionGroup }
//    //                Material.
//                    RadioButton {
//                        id: topButton
//                        text: qsTr("Selection Only")
//                        checked: true
//                        exclusiveGroup: tabPositionGroup
//    //                    Layout.minimumWidth: 100
//                    }
//                    RadioButton {
//                        id: bottomButton
//                        text: qsTr("All - Ignore Selection")
//                        exclusiveGroup: tabPositionGroup
//    //                    Layout.minimumWidth: 100
//                    }
//                }
//            }

//            ColumnLayout {
//                spacing: 0
//                anchors.fill: parent
//                ComboBox {
//                    id: combo
//                    Layout.preferredHeight: ProtoScreen.guToPx(4)
//                    width: parent.width * (4.0/11.0)
////                    anchors.bottom: rrr.top
//                    anchors.right: parent.right
//                    model: ["Scheduled", "Scored" , "Locked", "All" ]
//    //                enabled: modelData === "  Status  "
//                    currentIndex: 0
//    //                visible: modelData === "  Status  "
//                    //                            anchors.fill: parent
//                    onCurrentTextChanged: {
//    //                               MiddleMan.pProjectionsViewFilterProxyModel.setPos(currentText)
//                    }
//                }
//            Rectangle {
//                id: rec
//                height: ProtoScreen.guToPx(8)
//                width: parent.width
//                color: "transparent"
//                anchors.bottom: parent.bottom
                RowLayout {
                    spacing: 0
                    anchors.fill: parent
                    ExclusiveGroup { id: tabPositionGroup }
//                    width: parent.width
//                    Layout.preferredHeight: ProtoScreen.guToPx(4)
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
                                ComboBox {
                                    id: combo
                                    model: ["All"]//, "Scored" , "Locked", "All" ]
                                    enabled: modelData === " Status "
                                    currentIndex: 0 //3
                                    visible: false//modelData === " Status "
                                    anchors.fill: parent
                                    onCurrentTextChanged: {
                                        MiddleMan.setScheduleFilter(currentText)
                                    }
                                }

//                                Label {
//                                    text: "Games to"
//                                    visible: modelData === " Time "
//                                    horizontalAlignment: Text.Right
//                                    id: pt
//                                    anchors.right: parent.right
//                                    anchors.verticalCenter: parent.verticalCenter
//                                }

//                                Label {
//                                    id: pc
//                                    text: " Copy:"
//                                    visible: modelData === " Away "
//                                    anchors.left: parent.left
//                                    anchors.verticalCenter: parent.verticalCenter
//                                }
//                                RadioButton {
//                                    id: topButton
//                                    anchors.fill: parent
//                                    visible: modelData === " Status "
//                                    text: qsTr("Selected Only")

//                                    exclusiveGroup: tabPositionGroup

//                                }
//                                RadioButton {
//                                    id: bottomButton
//                                    visible: modelData === " Home "
//                                    anchors.fill: parent
//                                    checked: modelData === " Home "
//                                    text: qsTr("All")
//                                    exclusiveGroup: tabPositionGroup
//                                    onCheckedChanged: {
//                                        MiddleMan.set_useSelected(!checked)
//                                    }

//                                }
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

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        secsel = [["section"]]
                                        isl.clear()
                                        topc.changed()
                                    }
                                }

                            }
                        }
                    }
                }

//            }
//            }
       }
    }
//    Component {
//        id: sectionDelegate

//        Rectangle {
//            width: ListView.view.width
//            height: ProtoScreen.guToPx(.25);
//            color: Colors.blue
//        }
//    }

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

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if ( secsel[section] === "on")
                    secsel[section] = "off"
                else secsel[section] = "on"

//                    topc.secsel[section] = Qt.binding( function() {
//                        if ( secsel[section] === "on")
//                            return "off"
//                        else return "on"
//                        }
//                    )

                    topc.changed()
                }
            }
        }
    }

    Component {

        id: gamedel
        Material.Card {
            id: dcard
            function myMethod() {
                console.log("Button was clicked!" + secsel[time])
                if ( secsel[time] === "on" && !isl.isSelected(lv.model.index(index,0)))
                    MiddleMan.toggle(index,ItemSelectionModel.Toggle)
                else if ( secsel[time] === "off" && isl.isSelected(lv.model.index(index,0)) )
                    MiddleMan.toggle(index,ItemSelectionModel.Toggle)

                dcard.elevation = Qt.binding(
                    function() {
                        if ( isl.isSelected(lv.model.index(index,0)) )
                            return 5;
                        else
                            return 0
                    }
                )

                dcard.backgroundColor = Qt.binding(
                    function() {
                        if ( isl.isSelected(lv.model.index(index,0)) || !isl.hasSelection )
                            return "white";
                        else
                            return Qt.darker("white",1.20)
                    }
                )

            }

            Component.onCompleted: {
                if ( status === "Scheduled" )
                    MiddleMan.select(index, ItemSelectionModel.Select)
                else
                    MiddleMan.deselect(index, ItemSelectionModel.Deselect)

                topc.changed.connect(myMethod)
//                topc.changed()
//                console.log("complete is row" + index + " is " + isl.isSelected(lv.model.index(index,0)))
//                if ( isl.hasSelection  ) {
//                    console.log("complete is row" + index + " is " + isl.isSelected(lv.model.index(index,0)))
//                    elevation =  isl.isSelected(lv.model.index(index,0)) ? 5 : 0
//                    backgroundColor = isl.isSelected(lv.model.index(index,0)) ? "white" : Qt.darker("white",1.20)
//                }

            }

//            ListView.view.model.isSelected(index)
//            border.color:  isl.isRowSelected(index,0) ? "red" : "grey"
//ListView.view.model.indexOf(modelData))
            height: ProtoScreen.guToPx(4)
            width: parent.width

//            anchors.topMargin: (ListView.section === ListView.previousSection) ?
//                                   10 : -2;
//            anchors.bottomMargin: (ListView.section === ListView.nextSection) ?
//                                   10 : -2;

//            anchors.fill: parent
//            border.color: "transparent"
//            border.width: isl.isSelected(lv.model.index(index,0)) === true ? 2 : 0


//            elevation: isl.isSelected(lv.model.index(index,0)) === true ? 5 : 0
            property var widths: [3.0/11.0,2.0/11.0,2.0/11.0,4.0/11.0]


            elevation: isl.isSelected(lv.model.index(index,0)) ? 5 : 0
//                    border.color =  isl.isSelected(lv.model.index(index,0)) ? "Green" : "transparent"
//                    border.width = isl.isSelected(lv.model.index(index,0)) ? 2 : 0
            backgroundColor: (isl.isSelected(lv.model.index(index,0)) || !isl.hasSelection)  ? "white" : Qt.darker("white",1.20)

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
                text: status
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.fillHeight: true
                Layout.fillWidth: false
                Layout.preferredWidth: (parent.width * widths[3])
//                width: (parent.width * widths[3])
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    MiddleMan.toggle(index,ItemSelectionModel.Toggle)
                    dcard.elevation = Qt.binding(
                        function() {
                            if ( isl.isSelected(lv.model.index(index,0)) )
                                return 5;
                            else
                                return 0
                        }
                    )

                    dcard.backgroundColor = Qt.binding(
                        function() {
                            if ( isl.isSelected(lv.model.index(index,0)) || !isl.hasSelection )
                                return "white";
                            else
                                return Qt.darker("white",1.20)
                        }
                    )

                    topc.secsel[time] = ""//Qt.binding( function() { return "" } )


                    console.log("click is row" + index + " is " + isl.isSelected(lv.model.index(index,0)))
                    console.log("click is row is any" + isl.hasSelection)
//                    update()
//                    lv.currentIndex = index
                }

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
