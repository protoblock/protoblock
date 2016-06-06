import QtQuick 2.4
import Material 1.0
import ProRotoQml.Theme  1.0
Item {
    id: bannerRoot
    property string text
    property string color: "white"
    Card{
        height: 48
        width: parent.width
        backgroundColor: root.theme.primaryColor
        elevation: 0
        Text {
            text: bannerRoot.text
            height:38
            width: parent.width - 10
            font.pixelSize: 32
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin:  10
            color: bannerRoot.color
        }

    }
}
