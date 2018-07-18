import QtQuick 2.0
import  ProRotoQml.Theme 1.0
import QtGraphicalEffects 1.0
AbstractButton {
    id: stRoot
    height: 86
    width: parent.width / 1.07

     property bool enabled: false
     property bool active

//    property alias textAnchorsCentrer:


    property color color: Colors.white
    property color borderColor: Colors.grey
    property int borderWidth: 0
    property int radius: 0
    property string icon: "qrc:/Artwork/help.svg"

    property string text
    property string textColor : Colors.black

    property string subText
    property color subTextColor: Qt.lighter(Colors.black, 1.5)

    property bool hasIcon: true
    property string iconBackground: "transparent"
    property int iconBackgroundRadius: 0
    property int  iconBackgroundBorderWidth: 0
    property string  iconBackgroundBorderColor:  "transparent"


    property bool glowEnabledByMouse: false
    Keys.onPressed: {
        if (event.key === Qt.Key_Return && !event.modifiers) {
            clicked()
            event.accepted = true;
        }
    }
    states: [
           State {
               name: "default"
               PropertyChanges { target: glow; color: "transparent"}
           },
           State {
               name: "selected"
               PropertyChanges { target: glow; color: stRoot.color }
           },
           State {
               name: "hovered"
               PropertyChanges { target: glow; color: stRoot.color}
           },
           State {
               name: "selected-hovered"
               PropertyChanges { target: glow; color: stRoot.color}
           }
       ]
    RectangularGlow{
        id:glow
        anchors.fill: backer
        color:  "transparent"
        glowRadius: 5
        opacity: .77
        spread: 0.2
    }

    Rectangle{
        id: backer
        anchors.fill: parent
        color: stRoot.color
        radius: stRoot.radius
        border{
            width: stRoot.borderWidth
            color: stRoot.borderColor
        }

        Rectangle{
             id: i
            height: parent.height - 4
            width: height
            color: iconBackground
            radius: iconBackgroundRadius
            border.width: iconBackgroundBorderWidth
            border.color: iconBackgroundBorderColor
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 4
            Image {
                source: stRoot.icon
                height: parent.height - 5
                width: height
                visible:  hasIcon
                anchors.centerIn: parent
            }
        }
        Text {
            id: mainText
            font.pixelSize: 22
            width: parent.width - i.width - 5
            wrapMode: Text.WrapAnywhere
            anchors.left: i.right
            anchors.leftMargin: 6
            anchors.top: parent.top
            anchors.topMargin: 12
            text: qsTr(stRoot.text)
            color: textColor
        }
        Text {
            id: subTexts
            text: qsTr(subText)
            width: parent.width - i.width - 20
            wrapMode: Text.WordWrap
            color: subTextColor
            anchors.left: mainText.left
            anchors.top: mainText.bottom
            anchors.topMargin: 2
        }
    }
}
