
import QtQuick 2.0
import Material 1.0
import Material.ListItems 1.0 as ListItems
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

    height: 60
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
        height: 60
        width: parent.width
            TextField{
                id: lineInput
                placeholderText:  flipBar.placeHolder
                focus: flipBar.opened
                anchors { fill: parent; margins: 6 }
                onAccepted: {
                    if (Qt.inputMethod.visible)
                        Qt.inputMethod.hide()
                    flipBar.ok()
                }
            }
        }
}
