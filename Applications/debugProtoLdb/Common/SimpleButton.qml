import QtQuick 2.0
import  ProRotoQml.Theme 1.0
import QtGraphicalEffects 1.0
AbstractButton {
    id: stRoot
    height: 86
    width: parent.width / 1.07

     property bool enabled: false
     property bool active
    property color color: Colors.white
    property color borderColor: Colors.grey
    property int borderWidth: 0
    property int radius: 3
    property string icon: "qrc:/Artwork/help.svg"
    property string  glowColor: Colors.grey
    property string text
    property string textColor : Colors.white
    property bool glowEnabledByMouse: false
    Keys.onPressed: {
        if (event.key === Qt.Key_Return && !event.modifiers) {
            clicked()
            event.accepted = true;
        }
    }
    states: [
           State {
               name: "default"
               PropertyChanges { target: glow; color: "transparent"}
           },
           State {
               name: "selected"
               PropertyChanges { target: glow; color: stRoot.glowColor }
           },
           State {
               name: "hovered"
               PropertyChanges { target: glow; color: stRoot.glowColor}
           },
           State {
               name: "selected-hovered"
               PropertyChanges { target: glow; color: stRoot.glowColor}
           }
       ]
    RectangularGlow{
        id:glow
        anchors.fill: backer
        anchors.margins: -20
        color:  "transparent"
        glowRadius: 5
        opacity: .77
        spread: 0.2
    }

    Rectangle{
        id: backer
        anchors.fill: parent
        color: stRoot.color
        radius: stRoot.radius
        border{
            width: stRoot.borderWidth
            color: stRoot.borderColor
        }

        Text {
            id: mainText
            font.pixelSize: parent.height / 3
            width: parent.width  - 5
            wrapMode: Text.WrapAnywhere
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment:  Text.AlignHCenter
            text: qsTr(stRoot.text)
            color: textColor
        }
    }
}
