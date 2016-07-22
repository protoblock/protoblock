import QtQuick 2.4
import Material 1.0
import ProRotoQml.Theme  1.0
Item {
    id: bannerRoot
    property string text
    property string color: "white"
    property string backgroundColor: "black"
    property int fontSize: ProtoScreen.font(ProtoScreen.LARGE)
    property bool bold: false
    property int elevation: 2
    property bool helpShown: false
    property string helperTxt
    property string  helperHeader: "Help"
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
                    Text.AlignVCenter
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


    Icon {
        id: helperArea
        visible: helpShown
        width: ProtoScreen.guToPx(5)
        height: width
        anchors.right: parent.right
        anchors.rightMargin: ProtoScreen.guToPx(1)
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:/icons/ic_help.png"
        color: "white"
        MouseArea{
            anchors.fill: helperArea
            onClicked: {
                realRoot.helperHeader = bannerRoot.helperHeader
                realRoot.helperTxt = bannerRoot.helperTxt
                helperDialog.toggle()
            }
        }
    }
}
