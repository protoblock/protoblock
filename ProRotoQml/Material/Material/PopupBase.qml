import QtQuick 2.4
import QtQuick.Window 2.2
import Material 1.0
import Material.Extras 1.0
import "utils.js" as UtilsJS

FocusScope {
    id: popup
    property color overlayColor: "transparent"
    property string overlayLayer: "overlayLayer"
    property bool globalMouseAreaEnabled: true
    property bool dismissOnTap: true
    property bool showing: false
    property bool closeOnResize: false
    property Item __lastFocusedItem

    signal opened
    signal closed

    function toggle(widget) {
        if (showing) {
            close()
        } else {
            open(widget)
        }
    }

    function open() {
        __lastFocusedItem = Window.activeFocusItem
        parent = UtilsJS.findRootChild(popup, overlayLayer)

        if (parent.enabled === false)
            return

        showing = true
        forceActiveFocus()
        parent.currentOverlay = popup

        opened()
    }

    function close() {
        showing = false

        if (parent.hasOwnProperty("currentOverlay")) {
            parent.currentOverlay = null
        }

        if (__lastFocusedItem !== null) {
            __lastFocusedItem.forceActiveFocus()
        }

        closed()
    }
}
