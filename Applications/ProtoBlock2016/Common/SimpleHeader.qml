import QtQuick 2.4
import QtQuick.Controls 1.4
// FIXME the height should not be like so
AbstractCategoryHeader {
    id: categoryHeader
    height: 86
    width:  parent.width
    Rectangle {
        id: selectableArea
        color: "#c3c3c3"
        anchors{
            left: parent.left
            right: parent.right
            top: title.top
            bottom: title.bottom
            bottomMargin: -4
        }
        opacity: categoryHeader.state == "hovered" || categoryHeader.state == "selected-hovered" ? 0.4 : 0.0
        Behavior on opacity { NumberAnimation { duration: 100 } }
    }

    Image {
        id: icon
        anchors{
            bottom: title.bottom
            bottomMargin: 2
            left: parent.left
            leftMargin: -5
        }
        source: "qrc:/Artwork/chevron.svg"
        opacity: uncollapsed ? 1.0 : 0.0
        Behavior on opacity {NumberAnimation { duration: 100 }}

    }
    Label{
        id: title
        text: categoryHeader.label
        font.pointSize:  20
        anchors{
            bottom: parent.bottom
            left: icon.right
            leftMargin: 3
        }
    }

    Label {
        id: count
        text: "(%1)".arg(parent.count)
        font.pointSize:  20
        anchors{
            baseline: title.baseline
            left: title.right
            leftMargin: 25
        }
        opacity: 0.3
    }
}
