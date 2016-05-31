import QtQuick 2.4

FocusScope {
    property alias scrollbar: scrollbar
    property alias model: list.model
    property alias bodyDelegate: list.bodyDelegate
    property alias headerDelegate: list.headerDelegate
    anchors.fill: parent
    ViewWithBanner{
        id: list
        focus: true
        anchors.fill: parent
    }
    AbstractScrollbar {
        id: scrollbar
        width: 4
        anchors{
            top: parent.top
            topMargin: 15
            bottom: parent.bottom
            bottomMargin: 10
            right: list.right
            rightMargin: 10
        }

        targetFlickable: list.flickable
        /* The glow around the slider is 5 pixels wide on the left and right sides
           and 10 pixels tall on the top and bottom sides. */
        sliderAnchors.rightMargin: -5
        sliderAnchors.leftMargin: -5
        sliderAnchors.topMargin: -10
        sliderAnchors.bottomMargin: -10
        /* Hide the scrollbar if there is less than a page of results */
        opacity: targetFlickable.visibleArea.heightRatio < 1.0 ? 1.0 : 0.0
        Behavior on opacity {NumberAnimation {duration: 100}}
    }
}
