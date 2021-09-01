import QtQuick 2.5
import QtQuick.Controls 1.4

import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1
import Material 1.0

Item {
    id: orview
//    anchors.fill: parent
    property double dihight: ProtoScreen.guToPx(4)
    property string mysymbol: ""
//    property alias dihightordersl
    property alias globalorders: ordersl.model;

    anchors.fill: parent

    Card{
        id: topcard
        width: parent.width
        height: parent.height
        elevation: 0
        anchors{
            top: parent.top
            topMargin:ProtoScreen.guToPx(.5)
            horizontalCenter: parent.horizontalCenter
            margins: ProtoScreen.guToPx(.5)
        }


       Banner {
           id: bandepth
           anchors.top: parent.top
           anchors.horizontalCenter: parent.horizontalCenter
           text: mysymbol + " Orders" +( (uname === "") ?  ""  :  (" (" + uname + ")"))
           anchrosType: "center"
           height: ProtoScreen.guToPx(4)
           backgroundColor: themeroot.theme.primaryColor
           width: parent.width
           fontSize: ProtoScreen.font(ProtoScreen.SMALL)
           bold: true
       }

       Rectangle {
           id: boundingRect
           anchors.top: bandepth.bottom
           height: parent.height - bandepth.height
           anchors.horizontalCenter: bandepth.horizontalCenter
           width: bandepth.width
            ListView {
                anchors.fill: parent
                id: ordersl
                anchors.margins: 0
                anchors.horizontalCenter: boundingRect.horizontalCenter
                width: parent.width
                clip: true
//                model: MiddleMan.pROWGlobalOpenOrdersModel
                headerPositioning: ListView.OverlayHeader
                header: RowLayout {
                    z: 2
                    width: parent.width
                    height: dihight
                    spacing: 0

                    Repeater{
                        model: ["Cancel","Symbol","Side","Size","Price","Ref#"]
                        Card{
                            flat: true
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                            Layout.preferredWidth: (parent.width / 7.0) * (modelData === "Symbol") ? 2.0 : 1.0;
                            border.color:"black"
                            backgroundColor: themeroot.theme.primaryColor
        //                    border.width:  ProtoScreen.guToPx(.2)
                            radius: 0
                            Label{
                                anchors.centerIn: parent
                                text: modelData
                                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                color: "white"
                            }
                        }
                    }
                }
                delegate: OMSCard {
                    height: dihight
                    elevation: 2
                    width: parent.width
                    refnum:  model.refnum
                    qty: model.size
                    price: model.price
                    side: model.isbuy ? 1 : -1
                    symbolp: model.symbol
                }
            }
       }
    }
}
