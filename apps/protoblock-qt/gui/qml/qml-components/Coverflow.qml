import QtQuick 2.4
import Material 1.0
import ProRotoQml.Theme 1.0

PathView {
    id: coverFlow
    property alias models :coverFlow.model
    property double dimmingFactor: 1.0
    property bool isSelected: false
    property int selection
    property string itemInFocus
    property string backgroundColor: themeroot.theme.primaryColor
    signal clicked();
    path: circlePath
    interactive: ! isSelected
    focus: true
    pathItemCount: 4
    dragMargin: 100
    preferredHighlightBegin: 0.5
    preferredHighlightEnd: 0.5
    clip: true
    Behavior on dimmingFactor {
        NumberAnimation { duration: 300 }
    }

    delegate: Card {
        property bool isSelected: (coverFlow.isSelected && index == coverFlow.selection)
        property int oldX
        property int oldY
        id: cover
        backgroundColor: coverFlow.backgroundColor
        width: coverFlow.height * 2/3
        scale: isSelected ? 1.0 : PathView.itemscale
        elevation:isSelected ? 3:1 //PathView.itemscale
        z: PathView.z
        visible: isSelected || (! coverFlow.isSelected)
        transform: Rotation {
            axis.x: 0
            axis.y: 1
            axis.z: 0
            origin.x: cover.width / 2
            origin.y: cover.height / 4
            angle: cover.isSelected ? 0 : 0
            Behavior on angle {
                NumberAnimation { duration: 300 }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("current idx: " + index);
                itemInFocus = modelData
            }
            Label {
                anchors.centerIn: parent
                font.pixelSize:  ProtoScreen.font(ProtoScreen.NORMAL)
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                text: modelData + "\n" +  "Bid: 1 Bid Size:1 Ask: 200 Ask Size: 1"
            }

            Behavior on x {
                NumberAnimation { duration: 300 }
            }
            Behavior on y {
                NumberAnimation { duration: 300 }
            }

            Behavior on scale {
                enabled: coverFlow.isSelected
                NumberAnimation { duration: 300 }
            }

            states: [
                State {
                    name: "selected"
                    PropertyChanges {
                        target: cover
                        x: 120
                        y: 48
                    }
                }

            ]
        }
    }

    Path {
        id: threeDeePath

        startX: coverFlow.width -100
        startY: 100
        PathAttribute { name: "itemscale"; value: 0.4 }
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: -25 }
        PathAttribute { name: "angle2"; value: -30 }
        PathAttribute { name: "dimming"; value: 1.0 }

        PathQuad { x: -200; y : coverFlow.height; controlX: coverFlow.width; controlY: coverFlow.height * 3/4 }
        PathAttribute { name: "itemscale"; value: 1.0 }
        PathAttribute { name: "z"; value: 10 }
        PathAttribute { name: "angle"; value: -25 }
        PathAttribute { name: "angle2"; value: -30 }
        PathAttribute { name: "dimming"; value: 0.0 }
    }

    Path {
        id: circlePath
        startX: -100; startY: coverFlow.height
        PathAttribute { name: "itemscale"; value: 0.8 }
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: 80 }
        PathAttribute { name: "angle2"; value: 0 }
        PathAttribute { name: "dimming"; value: 0 }

        PathCubic {
            x: coverFlow.width + 100; y: coverFlow.height
            control1X: -30; control1Y: coverFlow.height / 2
            control2X: coverFlow.width + 30; control2Y: coverFlow.height / 2
        }
        PathAttribute { name: "itemscale"; value: 0.8 }
        PathAttribute { name: "z"; value: 10 }
        PathAttribute { name: "angle"; value: -80 }
        PathAttribute { name: "angle2"; value: 0 }
        PathAttribute { name: "dimming"; value: 0.0 }
    }


    Path {
        id: simplePath
        startX: -200
        startY: coverFlow.height * 3/4
        PathAttribute { name: "itemscale"; value: 1.0 }
        PathAttribute { name: "z"; value: 10 }
        PathAttribute { name: "angle"; value: -30 }
        PathAttribute { name: "angle2"; value: 0 }
        PathAttribute { name: "dimming"; value: 0 }

        PathLine { x: coverFlow.width + 100; y : coverFlow.height * 3/4 }
        PathAttribute { name: "itemscale"; value: 1.0 }
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: -30 }
        PathAttribute { name: "angle2"; value: 0 }
        PathAttribute { name: "dimming"; value: 0.8 }
    }


}


