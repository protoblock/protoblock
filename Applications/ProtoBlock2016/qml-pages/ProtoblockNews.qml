import QtQuick 2.0

import Material 1.0
import Material.ListItems 1.0 as ListItems
Item {
    Component.onCompleted: pageHelper.title = "Protoblock"
    Image {
        id: logo
        source: "qrc:/logoFinal.png"
        fillMode: Image.PreserveAspectFit
        width: parent.width / 1.07
        height: parent.height / 6
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height / 22
    }
    Label {
        id: welcomeTxt
        anchors.top: logo.bottom
        anchors.topMargin:  paintedHeight
        width: parent.width / 1.07
        font.pixelSize: Qt.platform.os === "android" ? 32 : 22
        font.family: "Roboto"
        color: realRoot.theme ===  "Pinky" ? "white" : "black"
        horizontalAlignment: Text.AlignHCenter
        text: "Welcome  To Protoblock"
        wrapMode: Text.WordWrap
    }
    Column{
        id:buttons
        width: parent.width
        height: parent.height - (welcomeTxt.paintedHeight - logo.height)
        spacing: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: welcomeTxt.bottom
        anchors.topMargin: 10

        Label {
            width: parent.width / 1.07
            font.pixelSize: Qt.platform.os === "android" ? 32 : 22
            font.family: "Roboto"
            horizontalAlignment: Text.AlignHCenter
            text: "Tell your Friends"
            color: realRoot.theme ===  "Pinky" ? "white" : "black"
            wrapMode: Text.WordWrap
        }
    }
}
