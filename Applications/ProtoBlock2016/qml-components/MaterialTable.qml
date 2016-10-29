import QtQuick 2.0

import QtQuick.Controls 1.3 as Controls
import QtQuick.Controls.Styles 1.3
import Material 1.0

import ProRotoQml.Theme 1.0
TableViewStyle{
//    itemDelegate:
//        Card{
//                width: parent.width
//                height: ProtoScreen.guToPx(2)
//                radius: 0
//                backgroundColor: styleData.row ? "blue" : "white"
//                Label{
//                    id: textItem
//                    text: styleData.value
//                    width: parent.width
//                    height: parent.height
//                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                    elide: Text.ElideRight
//                    wrapMode: Text.WordWrap
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    color: styleData.row ? "white" : "back"
//                }
//            }
    headerDelegate:Card{
        width: parent.width
        height: ProtoScreen.guToPx(2)
        backgroundColor: themeroot.theme.primaryColor
        radius: 0
        border.color: "black"
        Label{
            id: textItem
            text: styleData.value
            width: parent.width
            height: parent.height
            font.pixelSize: ProtoScreen.font(ProtoScreen.TINY)
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "white"
        }
    }
}
