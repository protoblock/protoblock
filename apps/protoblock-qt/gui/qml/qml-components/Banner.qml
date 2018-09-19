import QtQuick 2.4
import Material 1.0
import ProtoblockQml.Theme  1.0
Item {
//    Component.objectName: {
////        listProperty(bannerRoot);
//    }

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
    property string anchrosHType: "default"
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
            width: parent.width - ProtoScreen.guToPx(1.25)  - (!bannerRoot.helpShown ? 0.0 : ProtoScreen.guToPx(6))
            font.pixelSize: bannerRoot.fontSize
            font.bold: bannerRoot.bold
            color: bannerRoot.color
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            Binding on verticalAlignment {
                when: anchrosType === "default"
                value: Text.AlignVCenter
            }
            Binding on verticalAlignment {
                when: anchrosType === "verticalCenter"
                value: Text.AlignVCenter
            }
            Binding on verticalAlignment {
                when: anchrosType === "top"
                value: Text.AlignTop
            }
            Binding on verticalAlignment {
                when: anchrosType === "bottom"
                value: Text.AlignBottom
            }

            Binding on verticalAlignment {
                when: anchrosType === "center"
                value: Text.AlignVCenter
            }

            Binding on horizontalAlignment {
                when: anchrosHType === "default"
                value: (anchrosType === "center") ? Text.AlignHCenter : Text.AlignLeft
            }
            Binding on horizontalAlignment {
                when: anchrosHType === "horizontalCenter"
                value: Text.AlignHCenter
            }
            Binding on horizontalAlignment {
                when: anchrosHType === "center"
                value: Text.AlignHCenter
            }
            Binding on horizontalAlignment {
                when: anchrosHType === "right"
                value: Text.AlignRight
            }
            Binding on horizontalAlignment {
                when: anchrosHType === "left"
                value: Text.AlignLeft
            }
            Binding on horizontalAlignment {
                when: anchrosHType === "justify"
                value: Text.AlignJustify
            }


            anchors{
                left: parent.left
                leftMargin:  ProtoScreen.guToPx(1.25)
            }
        }
    }


    IconButton {
        id: helperArea
        visible: helpShown
        width: ProtoScreen.guToPx(4)
        height: width
        anchors.right: parent.right
        anchors.rightMargin: ProtoScreen.guToPx(1)
        anchors.leftMargin: ProtoScreen.guToPx(1)
        anchors.verticalCenter: parent.verticalCenter
        iconSource: "qrc:/icons/ic_help.png"
        color: bannerRoot.color
        size: ProtoScreen.guToPx(2.5)

        onClicked: {
            realRoot.helperHeader = bannerRoot.helperHeader
            realRoot.helperTxt = bannerRoot.helperTxt
            helperDialog.toggle()
        }

        /*
        MouseArea{
            anchors.fill: helperArea
            onClicked: {
                realRoot.helperHeader = bannerRoot.helperHeader
                realRoot.helperTxt = bannerRoot.helperTxt
                helperDialog.toggle()
            }
        }
        */
    }


//function listProperty(item)
//{
//    for (var p in item)
//    {
//        if( typeof item[p] != "function" )
//            if(p != "objectName")
//                console.log(p + ":" + item[p]);
//    }

//}
}
