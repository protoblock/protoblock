
import QtQuick 2.12
import ProRotoQml.Theme 1.0
// this is used to refresh the current page
// we could also put this on a timmer if we like

Item {
    height: ProtoScreen.guToPx(7.5)
    width: parent.width

    property bool refresh: state == "pulled" ? true : false

    Row {
        spacing: ProtoScreen.guToPx(.8)
        height: childrenRect.height
        anchors.centerIn: parent

        Image {
            id: arrow
            source: "qrc:/icons/ic_sync.png"
            height: ProtoScreen.guToPx(6)
            width: height
            fillMode: Image.PreserveAspectFit
            transformOrigin: Item.Center
            Behavior on rotation { NumberAnimation { duration: 200 } }
        }

        Text {
            id: label
            anchors.verticalCenter: arrow.verticalCenter
            text: "Pull to refresh...    "
            font.pixelSize: ProtoScreen.guToPx(2.25)
            color: "#999999"
        }
    }

    states: [
        State {
            name: "base"; when: mainListView.contentY >= -120
            PropertyChanges { target: arrow; rotation: 180 }
        },
        State {
            name: "pulled"; when: mainListView.contentY < -120
            PropertyChanges { target: label; text: "Release to refresh..." }
            PropertyChanges { target: arrow; rotation: 0 }
        }
    ]
}
