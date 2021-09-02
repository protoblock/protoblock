import QtQuick 2.12
import Material 1.0
import ProRotoQml.Theme 1.0
Item {
    id: b
    property string img
    property string name
    property int radius: 5
    property bool highlighted
    property alias elevation: bkg.elevation
    property string backgroundColor: Colors.blue
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
            anchors.margins:  ProtoScreen.guToPx( 2.5)
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
            height: ProtoScreen.guToPx(8)
            color: "#87000000"
            font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
            style: Text.Raised
            font.bold: true
            anchors.top: parent.bottom
            anchors.topMargin: 2
    }
}

