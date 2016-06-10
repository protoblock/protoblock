import QtQuick 2.0
import Material 1.0
Item {
    id: b
    property string img
    property string name
    property int radius: 5
    property bool highlighted
    property alias elevation: bkg.elevation
    property string backgroundColor: "blue"
    signal clicked()


    // Background Rectangle
    Card{
        id: bkg
        width: parent.width
        height: parent.height
        elevation: highlighted === true ? 1 : 0
        radius: b.radius
        backgroundColor: highlighted === true ? b.backgroundColor : "transparent"
        // image
        Image{
            id: imgage
            anchors.fill: parent
            anchors.margins: 20
            anchors.centerIn: parent
            source: img
            fillMode: Image.PreserveAspectFit
        }
    }
    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {}
        onExited: {}
        onClicked: {b.clicked() }
    }

    //name
        Label {
            text: b.name
            width: parent.width - 5
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
            height: 64
            color:  realRoot.theme !== "Pinky" ? "#c7c7c7": "black"
            font.pixelSize: 22
            style: Text.Raised
            font.bold: true
            anchors.top: parent.bottom
            anchors.topMargin: 2
    }
}

