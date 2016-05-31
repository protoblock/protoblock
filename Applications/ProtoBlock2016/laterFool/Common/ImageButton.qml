import QtQuick 2.4

Rectangle{
    id: imageButton
    width: 50
    height: 50
    color: "#88c3c3c3"
    border{ width: 2; color: "#805d5d5d" }
    radius: 5
    property bool hasText: false
    property string icon
    property string text
    signal clicked()
    Image {
        width: parent.width - 5
        height: parent.height - 5

        anchors.centerIn: parent
        source: icon
        MouseArea{
            anchors.fill: parent
            onClicked: imageButton.clicked()
        }
    }
    Text {
        text: qsTr(imageButton.text)
        anchors.top: imageButton.bottom
        anchors.topMargin: 2
        visible: hasText ? true : false
    }
}
