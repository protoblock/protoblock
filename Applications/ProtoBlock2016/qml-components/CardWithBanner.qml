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
    height: (cBan.height + cLab.paintedHeight) + (margin*2)
    Banner{
        id: cBan
        width: parent.width
        text: bannerText
        backgroundColor:  themeroot.theme.primaryColor
    }
    Label{
        id: cLab
        font.pixelSize: ProtoScreen.formFactor === "phone" ? ProtoScreen.font(ProtoScreen.NORMAL):ProtoScreen.guToPx(2.75)
        font.family: "Roboto"
        width: parent.width / 1.07
        height: parent.height - cBan.height
        anchors.top: cBan.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: margin
        color: realRoot.theme ===  "Pinky" ? "white" : "black"
        wrapMode: Text.WordWrap
        text: cardWithBanner.text
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
