import QtQuick 2.0
import QtQuick.Controls 1.4
import ProRotoQml.Theme 1.0
import ProRotoQml.Backend 1.0
import "Utils.js" as Utils
import "Common"
Card{
    id: header
    width: parent.width
    height:   headerText.paintedHeight + 20
    backgroundColor:  Colors.deepOrange
    elevation: 12
    property variant model
    property string headerText
    Text {
        id: headerText
        text:  header.headerText
        width: parent.width
        height: header.height
        color: Colors.white
        font.pixelSize: 48
        wrapMode: Text.WrapAnywhere
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

        ImageButton {
            id: cog
            width: parent.height - 10
            anchors.right: parent.right
            height: width
            icon: "http://findicons.com/files/icons/1580/devine_icons_part_2/128/home.png"
            anchors{
                right: parent.right
                rightMargin: 10
                verticalCenter: parent.verticalCenter
            }
            onClicked: {
                    comboTimer.start()
                }
        }

        ImageButton {
            width: parent.height - 10
            height: width
            anchors.right: cog.left
            anchors.verticalCenter: parent.verticalCenter
            icon: "http://50.0.50.101/artwork/circle-icons/one-color/png/64px/arrow-down.png"
                onClicked: {
                    Utils.getAllL(leaves,  stye.currentText)
                }
            }



    Row{
        width: parent.width / 3
        height: parent.height
        anchors{
            left: parent.left
            leftMargin: 10
        }

        Text {
            text: "Block Number: "
            color: Colors.white
            font.bold: true
            font.pixelSize:  parent.height / 2
            anchors.verticalCenter: parent.verticalCenter
        }
        TextInput{
            text: LdbHashReaderTool.getBkNumber()
            anchors.verticalCenter: parent.verticalCenter
            color: Colors.white
            font.pixelSize:  parent.height / 2
            validator: IntValidator{bottom: 0; top:LdbHashReaderTool.getBkNumber() }
            onAccepted: {
                focus = false
                isLeaves = false
                LdbHashReaderTool.jumpToBlock("blocknum_" + text, "string");
            }
        }
    }
}
