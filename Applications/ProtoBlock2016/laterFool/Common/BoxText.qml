import QtQuick 2.0

Rectangle {
    id: boxText
    property string text
    width: 20
    height: 20
    color: "transparent"
    border{width: 1; color: "white"}
    Text{
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "white"
        text: boxText.text
    }
}
