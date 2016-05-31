import QtQuick 2.4
AbstractCategoryHeader {
    id: categoryHeader
    height:  35
    property bool moving: false
    Rectangle {
        id: divider
        color: "black"
        height: 1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Image {
        id: iconImage
        source: "qrc:/Artwork/ok.svg"
        width: 22
        height: 22
        anchors.top: divider.bottom
        anchors.topMargin: 13
        anchors.left: parent.left
        anchors.leftMargin: 4
        fillMode: Image.PreserveAspectFit
        sourceSize.width: width
        sourceSize.height: height
    }

    Text{
        id: title
        text: categoryHeader.label
        font.pointSize:  20
        anchors.top: divider.bottom
        anchors.topMargin: 13
        anchors.left: iconImage.right
        anchors.leftMargin: 8
        color: "white"
    }

    Item {
        id: moreResults

        visible: availableCount > 0
        anchors.left: title.right
        anchors.leftMargin: 14
        anchors.baseline: title.baseline

        opacity: ( categoryHeader.state == "selected" || categoryHeader.state == "pressed"
                  || categoryHeader.state == "hovered" ) ? 1.0 : 0.5
        Behavior on opacity {NumberAnimation { duration: 100 }}

        Text{
            id: label
            font.pointSize: 10
            text: if(categoryHeader.folded) {
                      return  qsTr("See %1 more results See %1 more results").arg(availableCount).arg(availableCount)
                  } else {
                      return  "See fewer results"
                  }
            color: "white"
            anchors.left: parent.left
            anchors.baseline: parent.baseline
        }

        FoldingArrow {
            id: arrow
            folded: categoryHeader.folded
            anchors.left: label.right
            anchors.leftMargin: 10
            anchors.bottom: label.baseline
        }
    }
}
