
import QtQuick 2.0
import Material 1.0
FocusScope {
    id: wrapper

    property alias text: input.text
    property alias hint: input.placeholderText
    property alias prefix: input.text

    signal accepted


        TextField {
            id: input
            focus: true
            anchors { left: parent.right; right: parent.right; top: parent.top; bottom: parent.bottom }
            verticalAlignment: Text.AlignVCenter
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
