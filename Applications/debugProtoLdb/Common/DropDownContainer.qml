import QtQuick 2.4

FocusScope {
    property variant container: undefined
    property int containerYOffset: 0
    property int spacing: 20
    property bool first: false

    onActiveFocusChanged: {
        if (activeFocus && container) {
            var bottom = containerYOffset + y + height + spacing * 2
            var containerBottom = container.contentY + container.height

            if (first && !container.atYBeginning) {
                container.contentY = 0
            } else if (container.contentY > y + spacing || (containerBottom < bottom && container.height < height)) {
                container.contentY = containerYOffset + y + spacing
            } else if (containerBottom < bottom) {
                container.contentY += bottom - containerBottom
            }
        }
    }
}
