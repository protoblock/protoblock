import QtQuick 2.4
import Material 1.0
import ProRotoQml.Theme  1.0
Item {
    id: bannerRoot
    property string text
    property string color: "white"
    property string backgroundColor: "black"
    property int fontSize: ProtoScreen.guToPx(4)
    property bool bold: false
    property int elevation: 2
    // FIXME set up alias on anchos for the text and not a string
//    property alias anchorsType: bannerTxt.verticalAlignment
    property string anchrosType: "default"
    height: ProtoScreen.guToPx(6)
    width: parent.width
    Card{
        height: parent.height
        width: parent.width
        backgroundColor: bannerRoot.backgroundColor
        elevation: bannerRoot.elevation
        radius: 0
        Label {
            id: bannerTxt
            text: bannerRoot.text
            height:parent.height
            width: parent.width - 10
            font.pixelSize: bannerRoot.fontSize
            font.bold: bannerRoot.bold
            color: bannerRoot.color
            verticalAlignment: {
                if(anchrosType === "default"){
                    Text.AlignBottom
                }
                else if (anchrosType === "verticalCenter")
                {
                    Text.AlignVCenter
                }
                else if (anchrosType === "top")
                {
                    Text.AlignTop
                }
                else if (anchrosType === "bottom")
                {
                    Text.AlignBottom
                }
            }
            anchors{

                left: parent.left
                leftMargin:  ProtoScreen.guToPx(1.25)
            }
        }

    }
}
