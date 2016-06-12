import QtQuick 2.0
import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
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
        property string protoblockemail: "contact@protoblock.com"

    Image {
        id: logo
        source: "qrc:/logoFinal.png"
        fillMode: Image.PreserveAspectFit
        width: parent.width / 1.07
        height: Screen.height / 6
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height / ProtoScreen.guToPx(2.75)
    }


    Column{
        width: parent.width
        height : parent.height - logo.height
        anchors.top: logo.bottom
        spacing: ProtoScreen.guToPx(2.5)
        Rectangle{
            height: ProtoScreen.guToPx(2.5)
            width: 1
            color: "transparent"
        }
        Label{
            width: parent.width
            font.pixelSize: ProtoScreen.guToPx(3)
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
