import QtQuick 2.4

Item {
    id: expandingDropDown
    //    property alias label: label.text
    property alias itemHeight: dropDown.itemHeight
    property alias maxHeight: dropDown.maxHeight
    property alias maxItems: dropDown.maxItems
    property alias minItems: dropDown.minItems
    property alias contextItems: dropDown.contextItems
    property alias model: dropDown.model
    property alias delegate: dropDown.delegate
    property alias selectedIndex: dropDown.selectedIndex
    property alias selectedItem: dropDown.selectedItem
    property int spacing: 20
    property string color: "transparent"
    height: label.height + dropDown.height //+ spacing  //* 2

    Rectangle {
        id: label
        anchors.top: parent.top
        height:  0//paintedHeight
        width: height
        color: "transparent"
    }

    AbstractExpandingDropDown {
        id: dropDown
        anchors.left:   parent.left;
        anchors.right: parent.right
        anchors.top: label.bottom;
        focus: true
        maxHeight: parent.height
        maxItems: 10
        minItems: 3
        contextItems: 2
        itemHeight: 63

        Keys.onReturnPressed: {
            toggle()
        }

        Keys.onEscapePressed: {
            if (state == "expanded") dismiss()
            else event.accepted = false
        }

        MouseArea {
            anchors.fill: parent
            enabled: dropDown.state == "collapsed"
            onClicked: { dropDown.forceActiveFocus(); dropDown.open() }
        }

        delegate:
            Rectangle {
            anchors.left: parent.left; anchors.right: parent.right
            height: itemHeight
            color: expandingDropDown.color

            Text {
                text: label
                anchors.fill: parent
                color: "white"
                font.pointSize:  itemHeight / 2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent
                enabled: dropDown.state == "expanded"
                onClicked: { dropDown.selectedIndex = index; dropDown.state = "collapsed" }
            }
        }
    }
    Image {
        id: topArrow
        source: "qrc:/Artwork/chevron.svg"
        height: itemHeight - 10

        width:  height

        anchors.right:  parent.right
        anchors.rightMargin: dropDown.state === "expanded" ? 5: 0
        rotation: dropDown.state === "expanded" ? 90 : 0

        anchors.top:  parent.top
        anchors.topMargin: 6
        Behavior on opacity { NumberAnimation { duration: 250 } }
    }
}
