
import QtQuick 2.12
import Material 1.0
import Material.ListItems 1.0 as ListItems

import ProRotoQml.Theme 1.0
FlipBar {
    id: flipBar
    animDuration: 250
    property string label: ""
    property string placeHolder: ""
    property alias searchText: lineInput.text
    property alias prefix: lineInput.text
    property bool opened: false
    signal ok
    signal hasOpened

    height: ProtoScreen.guToPx(7.5)
    width: parent.width

    function open() {
        flipBar.flipUp()
        flipBar.opened = true
        lineInput.forceActiveFocus()
        flipBar.hasOpened()
    }

    function close() {
        if (opened) {
            flipBar.flipDown()
            flipBar.opened = false
        }
    }

    front: ListItems.Standard {
        elevation:  5
        height: 60
        text: flipBar.label
        width: parent.width
        onClicked: {
            if (!flipBar.opened)
                open()
            else if (!lineInput.activeFocus)
                lineInput.forceActiveFocus()
            else
                close()
        }
    }

    back: ListItems.Standard {
        elevation: 5
        height: ProtoScreen.guToPx(7.5)
        width: parent.width
        TextField{
            id: lineInput
            helperText:   flipBar.placeHolder
            focus: flipBar.opened
            anchors { fill: parent; margins: ProtoScreen.guToPx(.75) }
            onAccepted: {
                if (Qt.inputMethod.visible)
                    Qt.inputMethod.hide()
                flipBar.ok()
            }
        }
    }
}
