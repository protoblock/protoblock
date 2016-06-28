import QtQuick 2.4

import ProRotoQml.Theme 1.0

import Material 1.0

Card{
    id: cardWithBanner
    property string text
    property string bannerText
//    property alias  object: objects.children
    property int margin: ProtoScreen.guToPx(1)
    width: parent.width
    height: (cBan.height + cLab.paintedHeight) + margin
    Column{
        id: cCol
        width: parent.width
        height: parent.height
        spacing: 1
        Banner{
            id: cBan
            width: parent.width
            text: bannerText
            backgroundColor:  themeroot.theme.primaryColor
        }
        Label{
            id: cLab
            width: parent.width / 1.2
            font.pixelSize: ProtoScreen.formFactor === "phone" ? ProtoScreen.guToPx(4):ProtoScreen.guToPx(2.75)
            font.family: "Roboto"
            anchors.horizontalCenter: parent.horizontalCenter
            color: realRoot.theme ===  "Pinky" ? "white" : "black"
            wrapMode: Text.WordWrap
            text: cardWithBanner.text
        }
    }

//    Item {
//        id: objects
//        width: parent.width
//        height: children.height + ProtoScreen.guToPx(3)
//        anchors.top: cCol.bottom

//    }



//    Scrollbar{
//        flickableItem: objects.children
//    }
}
