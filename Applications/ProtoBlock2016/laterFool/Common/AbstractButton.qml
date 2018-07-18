import QtQuick 2.0

FocusScope {
    property bool enabled: true
    property bool hoverEnabled: false
    property bool pressed: false
    property alias mouseOver: mouse_area.containsMouse
    signal clicked
    Accessible.role: Accessible.PushButton
    MouseArea {
        id: mouse_area
        enabled: parent.enabled
        hoverEnabled: parent.hoverEnabled
        anchors.fill: parent
        onClicked:{
            parent.clicked()
        }
    }

    state: {
        if (activeFocus && mouse_area.containsMouse)
            return "selected-hovered"
        else if(pressed || mouse_area.pressed)
            return "pressed"
        else if(activeFocus)
            return "selected"
        else if(mouse_area.containsMouse)
            return "hovered"
        else
            return "default"
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Return && !event.modifiers) {
            clicked()
            event.accepted = true;
        }
    }
}
