import QtQuick 2.3
import ProRotoQml.Theme 1.0
import QtQuick.Controls 1.4

Item {
    id: helperRoot
    width: rootLoader.width
    height: rootLoader.height
    property string title
    property bool closeShown: false
    property alias  object: mainPage.children
    signal closeClicked()
    signal settingsClicked()
    Item{
        width: helperRoot.width
        height: helperRoot.height - pageHeader.height
        anchors.top: bottomLine.bottom
        Rectangle{
            id: mainPage
            anchors.fill: parent
            color:"#30c7c7c7"
        }
    }
    Rectangle{
        id: bottomLine
        color:  Palette.alternateBase;
        width: helperRoot.width
        height: 1
        anchors.top: pageHeader.bottom
        anchors.topMargin: 2
    }
    Rectangle{
        id: pageHeader
        height: root.height / 6
        width: parent.width
        color:   Palette.base
        Text{
            text: qsTr(helperRoot.title)
            color: Palette.text
            font.pointSize:  Unit.gu(4)
            anchors.fill: parent
            horizontalAlignment:Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}

