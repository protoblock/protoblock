import QtQuick 2.4
import Material 1.0
import ProRotoQml.Theme  1.0
Item {
//    Component.objectName: {
//        listProperty(bannerRoot);
//    }

    id: inthelperroot
    property string labelTxt
    property int lo: 0
    property int hi: 11
    property alias txtN: combo.itext
    signal changed()

    property string text
    property string color: "black"
    property string backgroundColor: "white"
    property int fontSize: ProtoScreen.font(ProtoScreen.SMALL)
    property bool bold: false
    property int elevation: 2
    property bool helpShown: false
    property string helperTxt
    property string  helperHeader: "Help"
    // FIXME set up alias on anchos for the text and not a string
//    property alias anchorsType: bannerTxt.verticalAlignment
    property string anchrosType: "default"
    height: ProtoScreen.guToPx(6)
    width: labelleft.width + baserect.width
    property double recwidth: ProtoScreen.guToPx(hi.toString().length)

//        height: ProtoScreen.SMALL
//        width: ProtoScreen.SMALL * 5.0

//        id: row


    Label{
        id: labelleft
        text: labelTxt
        height: parent.height
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
//        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.Right
    }


//    Item {
//        id: background
//        height: parent.height
//            anchors.left: labelTxt.Right

//            Rectangle {
//                anchors.fill: parent
//                anchors.bottomMargin: control.pressed ? 0 : -1
//                color: "#10000000"
//                radius: baserect.radius
//            }

        Rectangle {
            height: labelleft.contentHeight + ( 2 * ProtoScreen.guToPx(.50))
            width: recwidth + ProtoScreen.guToPx(2)
            anchors.left: labelleft.right
            anchors.leftMargin: ProtoScreen.guToPx(1)
            anchors.verticalCenter: parent.verticalCenter
//            anchors.horizontalCenter: parent.horizontalCenter

            id: baserect
//                gradient: Gradient {
//                    GradientStop {color: control.pressed ? "#bababa" : "#fefefe" ; position: 0}
//                    GradientStop {color: control.pressed ? "#ccc" : "#e3e3e3" ; position: 1}
//                }
            radius: ProtoScreen.SMALL * 0.16
//            anchors.fill: parent
            border.color:  combo.activeFocus ? Theme.accentColor : "#999"
            color: combo.activeFocus ?   "white" : "lightgray"


            TextField {
//                fontSize: fontSize
                height: parent.height
                font: font

                anchors.verticalCenter: parent.verticalCenter
//                anchors.fill: parent
                property int itext
                id: combo
                width: recwidth
                anchors.left: parent.left
                anchors.leftMargin: ProtoScreen.guToPx(.25)
//                anchors.right: imageItem.left
    //            anchors.verticalCenter: labelTxr
                validator: IntValidator {bottom: lo; top: hi;}
                text: lo.toString()
                itext: lo
                inputMethodHints: Qt.ImhDigitsOnly
                color: Theme.accentColor//"#999" // "#47b"
                bcolor: "lightgray" //Theme.light.textColor // "#999"
                showBorder: true
                onAccepted: {
//                    imageItem.colorize
                    console.log("accepted" + text)
//                    inthelperroot.txtNumber = parseInt(text,10)
                    itext = parseInt(text,10)
                    changed()
                    console.log("accepted itext" + itext + " ? " + parseInt(text,10))

                }
                onEditingFinished: {
                    console.log("onEditingFinished" + text)
//                    inthelperroot.txtNumber = parseInt(text,10)
                    itext = parseInt(text,10)
                    changed()
                }

                textColor: combo.activeFocus ? Theme.light.textColor : "black"
            }
            Icon {
                id: imageItem
                visible: helpShown
                width: parent.width - combo.width
                height: width
                anchors.right: parent.right
//                anchors.rightMargin: ProtoScreen.guToPx(1)
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/icons/ic_help.png"
//                color: Theme.dark.textColor
                MouseArea{
                    anchors.fill: imageItem
                    onClicked: {
                        realRoot.helperHeader = inthelperroot.helperHeader
                        realRoot.helperTxt = inthelperroot.helperTxt
                        helperDialog.toggle()
                    }
                }
            }

        }

    }

