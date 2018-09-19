import QtQuick 2.4
import ProtoblockQml.Utils 1.0
import Material.Extras 1.0
import "utils.js" as UtilsJS

PopupBase {
    id: popover

    visible: view.opacity > 0
    closeOnResize: true

    property bool isBelow

    property alias backgroundColor: view.backgroundColor

    property int padding: 16 * Units.dp

    default property alias data: view.data

    function open(caller, offsetX, offsetY) {
        parent = UtilsJS.findRootChild(popover, overlayLayer)

        if (!parent.enabled)
            return

        if (parent.currentOverlay)
            parent.currentOverlay.close()

        if(typeof offsetX === "undefined")
            offsetX = 0

        if(typeof offsetY === "undefined")
            offsetY = 0

        var position = caller.mapToItem(popover.parent, 0, 0)
        var globalPos = caller.mapToItem(null, 0, 0)
        var root = UtilsJS.findRoot(popover)

        popover.x = Qt.binding(function() {
            var x = position.x + (caller.width / 2 - popover.width / 2) + offsetX

            if(x + width > root.width - padding)
                x = root.width - width - padding

            if (x < padding)
                x = padding

            return x
        })

        popover.y = Qt.binding(function() {
            var y = y = position.y + caller.height + offsetY

            if (y + popover.height > root.height - padding) {
                isBelow = false
                y = position.y - popover.height - offsetY
            } else {
                isBelow = true
            }

            return y
        })

        showing = true
        parent.currentOverlay = popover

        opened()
    }

    function close() {
        showing = false
        parent.currentOverlay = null
    }

    View {
        id: view

        elevation: 2
        radius: 2 * Units.dp

        anchors {
            left: parent.left
            right: parent.right
            top: isBelow ? parent.top : undefined
            topMargin: popover.showing ? 0 : -popover.height/4
            bottom: !isBelow ? parent.bottom : undefined
            bottomMargin: popover.showing ? 0 : -popover.height/4

            Behavior on topMargin {
                NumberAnimation { duration: 200 }
            }

            Behavior on bottomMargin {
                NumberAnimation { duration: 200 }
            }
        }

        height: popover.height

        opacity: popover.showing ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
    }
}
