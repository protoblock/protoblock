import QtQuick 2.4
Item {
    id: scrollbar
    property variant targetFlickable
    property alias sliderAnchors: sliderImage.anchors

    MouseArea {
        id: scrollMouseArea

        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

        onPressed: {
            /* Scroll one page without overshooting */
            var scrollAmount = mouseY > slider.y ? targetFlickable.height : -targetFlickable.height
            var destination = targetFlickable.contentY + scrollAmount
            var clampedDestination = Math.max(0, Math.min(targetFlickable.contentHeight - targetFlickable.height,
                                                          destination))
            scrollAnimation.to = clampedDestination
            scrollAnimation.restart()
        }

        NumberAnimation {
            id: scrollAnimation
            duration: 200
            easing.type: Easing.InOutQuad
            target: targetFlickable
            property: "contentY"
        }
    }

    Item {
        id: slider

        property int minimalHeight: 40

        anchors.left: parent.left
        anchors.right: parent.right

        Binding {
            target: slider
            property: "y"
            value: {
                var clampedYPosition = Math.max(0, Math.min(1-targetFlickable.visibleArea.heightRatio,
                                                            targetFlickable.visibleArea.yPosition))
                return clampedYPosition * scrollbar.height
            }
            when: !dragMouseArea.drag.active
        }

        height: Math.min(scrollbar.height, Math.max(minimalHeight, targetFlickable.visibleArea.heightRatio * scrollbar.height))

        Behavior on height {NumberAnimation {duration: 200; easing.type: Easing.InOutQuad}}

        Rectangle {
            id: sliderImage
            color: "green"
            anchors.fill: parent
        }

        MouseArea {
            id: dragMouseArea

            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton
            drag.target: slider
            drag.axis: Drag.YAxis
            drag.minimumY: 0
            drag.maximumY: scrollbar.height - slider.height

            onPositionChanged: {
                if (drag.active) {
                    targetFlickable.contentY = slider.y * targetFlickable.contentHeight / scrollbar.height
                }
            }
        }
    }
}
