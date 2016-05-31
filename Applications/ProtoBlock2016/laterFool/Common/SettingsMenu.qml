import QtQuick 2.0
import QtQuick.Controls 1.4

//LETS re-think this whole thing

Rectangle{
    height: 20
    width: 30
    color: "transparent"
    visible: root.currentUser === "NOTLOGEDIN" ? 0 : 1
    property bool closeShown: false
    signal closeClicked();
    signal settingsClicked();
    Row{
        anchors.fill: parent
        spacing: 5
        Image {
            id: uImg
            source: "qrc:/Artwork/contact.svg"
            height:   parent.height
            width: height
        }
        Label{
            id: user
            text: root.currentUser
            color: "#5d5d5d"
            font.pointSize: 16
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }

        Rectangle{width: 2; height: parent.height ; color: "#5d5d5d"}
        Label{
            id: time
            text: root.currentTime
            color: "#5d5d5d"
            font.pointSize: 20
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }
        Rectangle{width: 2; height: parent.height ; color: "#5d5d5d"}
        ImageButton{
            width: height
            height: helperRoot.closeShown === true ?  pageHeader.height / 2 : 0
            icon: "qrc:/Artwork/properties.svg"
            onClicked: {
                settingsClicked()
                //                    rootLoader.source = "qrc:/Pages/Settings.qml"
            }
        }
        Rectangle{width: closeShown  === true ? 2 : 0 ; height: parent.height ; color: "#5d5d5d"}
        ImageButton{
            width: height
            height: closeShown === true ?  pageHeader.height / 2 : 0
            icon: "qrc:/Artwork/close.svg"
            onClicked: {
                closeClicked()
            }
        }
    }
}
