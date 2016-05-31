import QtQuick 2.4
import 'awesome.js' as Awesome
import ProRotoQml.Theme 1.0

Item {
    id: widget
    property string name
    property bool rotate: widget.name.match(/.*-rotate/) !== null
    property bool valid: text.implicitWidth > 0

    property alias color: text.color
    property int size: Unit.dp(24)

    width: text.width
    height: text.height

    property bool shadow: true
    property var icons: Awesome.map
    property alias weight: text.font.weight
    FontLoader { id: fontAwesome; source: Qt.resolvedUrl("qrc:/Common/fonts/fontawesome/FontAwesome.otf") }
    Text {
        id: text
        anchors.centerIn: parent

        property string name: widget.name.match(/.*-rotate/) !== null ? widget.name.substring(0, widget.name.length - 7) : widget.name

        font.family: fontAwesome.name
        font.weight: Font.Light
        text: widget.icons.hasOwnProperty(name) ? widget.icons[name] : ""
        color: Colors.blueGrey
        style: shadow ? Text.Raised : Text.Normal
        styleColor: Qt.rgba(0,0,0,0.5)
        font.pixelSize: widget.size

        Behavior on color {
            ColorAnimation { duration: 200 }
        }

        NumberAnimation on rotation {
            running: widget.rotate
            from: 0
            to: 360
            loops: Animation.Infinite
            duration: 1100
        }
    }
}
