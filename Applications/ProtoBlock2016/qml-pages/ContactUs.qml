import QtQuick 2.0
import QtQuick 2.4
import QtQuick.Layouts 1.1
import ProRotoQml.Protoblock 1.0
import ProRotoQml.Theme 1.0
import ProRotoQml.Utils 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItem
import Material.Extras 1.0
Item {
    Component.onCompleted: {

        pageHelper.title = "Contact Protoblock"
    }
        property string protoblockemail: "jay@protoblock.com"

    Image {
        id: logo
        source: "qrc:/logoFinal.png"
        fillMode: Image.PreserveAspectFit
        width: parent.width / 1.07
        height: parent.height / 6
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height / 22
    }


    Column{
        width: parent.width
        height : parent.height - logo.height
        anchors.top: logo.bottom
        spacing: 20
        Rectangle{height: 20 ; width: 1; color: "transparent" }
        Text{
            width: parent.width
            font.pixelSize: 32
            text: "Subject"
            anchors.left: subInput.left
        }
        ThinDivider{
            width: parent.width / 1.07
        }
        TextField{
            id: subInput
            width: parent.width / 1.07
            placeholderText: "Please enter in the subject"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Button{
            width:  parent.width  / 1.07
            elevation: 3
            backgroundColor: Colors.primaryColor
            text: "Send Email"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                Qt.openUrlExternally("mailto:" + protoblockemail + "?Subject="+subInput.text)
            }
        }
    }
}
