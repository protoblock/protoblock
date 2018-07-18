import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
    anchors.fill: parent
    Rectangle {
        id: lineColumn
        property int rowHeight: text.font.pixelSize + 3
        color: "#f2f2f2"
        width: 50
        height: parent.height
        Rectangle {
            height: parent.height
            anchors.right: parent.right
            width: 1
            color: "#ddd"
        }
        Column {
            y: -text.flickableItem.contentY + 4
            width: parent.width
            Repeater {
                model: Math.max(text.lineCount + 2, (lineColumn.height/lineColumn.rowHeight) )
                Text {
                    id: text
                    color: "#666"
                    font: text.font
                    width: lineColumn.width
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    height: lineColumn.rowHeight
                    renderType: Text.NativeRendering
                    text: index
                }
            }
        }
    }
    TextArea {
        id: editor
        anchors.left: lineColumn.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        wrapMode: TextEdit.NoWrap
        frameVisible: false
    }
}

