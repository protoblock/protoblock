
import QtQuick 2.0
import Material 1.0
FocusScope {
    id: wrapper

    property alias text: input.text
    property alias hint: input.placeholderText
    property alias prefix: input.text

    signal accepted

//    Rectangle {
//        anchors.fill: parent
//        border.color: "grey"
//        color: "transparent"
//        radius: 4

//        Text {
//            id: hint
//            anchors { fill: parent; leftMargin: 14 }
//            verticalAlignment: Text.AlignVCenter
//            text: "Enter word"
//            font.pixelSize: 18
//            color: "transparent"
//            opacity: input.length ? 0 : 1
//        }

//        Text {
//            id: prefix
//            anchors { left: parent.left; leftMargin: 14; verticalCenter: parent.verticalCenter }
//            verticalAlignment: Text.AlignVCenter
//            font.pixelSize: 18
//            color: "transparent"
//            opacity: !hint.opacity
//        }

        TextField {
            id: input
            focus: true
            anchors { left: parent.right; right: parent.right; top: parent.top; bottom: parent.bottom }
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 18
//            color: "#707070"
            onAccepted: wrapper.accepted()
        }

//        Image {
//            source: "qrc:/icons/ic_sync.png"
//            anchors.right: parent.right
//            anchors.rightMargin: 12
//            height: parent.height / 1.6
//            width: height
//            fillMode: Image.PreserveAspectFit
//            anchors.verticalCenter: parent.verticalCenter
//            MouseArea {
//                anchors { fill: parent; margins: -10 }
//                onClicked: wrapper.accepted()
//            }
//        }
//    }
}
