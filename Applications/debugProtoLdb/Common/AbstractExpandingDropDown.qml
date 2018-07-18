import QtQuick 2.0

FocusScope {
    // Height of the items
    property int itemHeight
    // The number of items to show around currently selected one
    property int contextItems
    // Maximum drop-down height, takes precedence over maxItems
    property int maxHeight
    // Maximum items shown in an expanded list
    property int maxItems
    // Minimum items shown in an expanded list
    property int minItems

    property int expandedHeight
    property int resultingOffscreenItems
    property int selectedIndex
    property variant selectedItem: null

    // Whether height is animating
    property bool animating: heightAnimation.running
    property bool topVisible: false //animating || listView.atYBeginning
    property bool bottomVisible: false //animating || listView.atYEnd

    // ListView properties
    property alias model: listView.model
    property alias delegate: listView.delegate
    property alias highlight: listView.highlight
    property variant listView: listView

    id: dropDown
    state: "collapsed"

    expandedHeight: {
        // Minimum height
        var minh, maxh = 0
        minh = Math.min(minItems, listView.count) * itemHeight

        // Maximum height, maxHeight takes precedence
        if (maxHeight && maxItems) {
            maxh = Math.min(maxHeight, Math.min(maxItems, listView.count) * itemHeight)
        } else if (maxItems) {
            maxh = Math.min(maxItems, listView.count) * itemHeight
        } else if (maxHeight) {
            maxh = Math.min(maxHeight, listView.count * itemHeight)
        }
        return Math.max(minh, maxh)
    }

    // Three because: one over selected, one selected, one under selected
    resultingOffscreenItems: Math.min(contextItems, Math.floor(expandedHeight / (itemHeight * 3)))

    function toggle() {
        if (state == "collapsed") {
            open()
        } else {
            close()
        }
    }

    function open() {
        if (state == "collapsed") {
            state = "expanded"
        }
    }

    function close() {
        if (state == "expanded") {
            state = "collapsed"
            selectedIndex = listView.currentIndex
            selectedItem = listView.currentItem
        }
    }

    function dismiss() {
        state = "collapsed"
        listView.currentIndex = selectedIndex
    }

    Behavior on height { NumberAnimation { id: heightAnimation } }

    Item {
        id: list
        focus: true
        anchors.fill: parent

        ListView {
            id: listView
            anchors.fill: parent
            clip: true
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 200
            highlight:  Rectangle {
                color: "transparent"
                opacity: dropDown.activeFocus ? 0.8 : 0
                border.color: "blue"
                border.width: 1
                 Behavior on opacity { NumberAnimation { duration: 250 } }
            }

            Behavior on preferredHighlightBegin { PropertyAnimation { duration: 250 } }
            Behavior on preferredHighlightEnd { PropertyAnimation { duration: 250 } }

            /* FIXME: The below is needed for the hightlight to not get confused */
            Component.onCompleted: {
                if (dropDown.selectedIndex != undefined) {
                    currentIndex = dropDown.selectedIndex
                }
            }

            Connections {
                target: dropDown
                onSelectedIndexChanged: listView.currentIndex = dropDown.selectedIndex
            }

            /* Inhibit key navigation when at end or beginning of the list */
            Keys.onUpPressed: if (currentIndex != 0) event.accepted = false
            Keys.onDownPressed: if (currentIndex != count - 1) event.accepted = false

            interactive: dropDown.state == "expanded"
        }
    }

    onActiveFocusChanged: if (!activeFocus) { state = "collapsed" }

    states: [
        State {
            name: "collapsed"
            PropertyChanges {
                target: dropDown
                height: itemHeight
            }
            PropertyChanges {
                target: listView
                preferredHighlightBegin: 0
                preferredHighlightEnd: itemHeight
            }
            PropertyChanges {
                target: list
                Keys.forwardTo: []
            }
        },
        State {
            name: "expanded"
            PropertyChanges {
                target: dropDown
                height: expandedHeight
            }
            PropertyChanges {
                target: listView
                preferredHighlightBegin: resultingOffscreenItems * itemHeight
                preferredHighlightEnd: expandedHeight - (resultingOffscreenItems * itemHeight)
            }
            PropertyChanges {
                target: list
                Keys.forwardTo: [listView]
            }
        }
    ]
}
