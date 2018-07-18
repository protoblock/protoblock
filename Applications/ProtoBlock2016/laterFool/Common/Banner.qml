import QtQuick 2.0
import QtQuick.Controls 1.4
import ProRotoQml.Theme 1.0

Item{
    id: banner
    width:  20
    height:     86
    property string color:Qt.darker( Colors.blue, .9)
    property string text
    property string textColor: "white"
    property bool bold: true
    property bool italic: true
    property int fontSize: 20
    property bool tickShown: false
    property bool open: false

    Rectangle{
        width: parent.width
        height: parent.height
        color:  banner.color
        Rectangle{
            width: parent.width / 1.07
            height:  parent.height
            color: banner.color
            anchors.horizontalCenter: parent.horizontalCenter
            Label{
                id: bannerLabel
                font.pointSize: banner.fontSize
                font.bold: banner.bold
                font.italic: banner.italic
                color: banner.textColor
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 4
                text:qsTr(banner.text)
            }
            Image {
                id: tick
                width:banner.height / 2
                height:  width
                source: "qrc:/report_problem.png"
                opacity:  tickShown === true ? 1 : 0
                rotation: open === true ? 90 : 0
                anchors{
                    right: parent.right
                    rightMargin: 5
                    verticalCenter: bannerLabel.verticalCenter

                }
            }
        }
    }
}
