import QtQuick 2.12
import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
//import ProRotoQml.Protoblock 1.0
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
        height: Screen.height / 8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin:ProtoScreen.guToPx(.50)
    }

    CardWithContent{
        id: buySell
        anchors.top: logo.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: parent.height - logo.height
        anchrosType: "center"
        bannerText: "Contact protoblock"
        object:
            ListView{
                width: parent.width
                height: parent.height
                clip: true
                spacing: ProtoScreen.guToPx(1)
                model: contactmod
                delegate: ListItem.Subtitled {
                    elevation:  5
                    text: model.source
                    subText: model.subT
                    width: parent.width
                    maximumLineCount: 2
                    onClicked:{
                        model.name === "Chat" ?
                                    rootLoader.source ="qrc:/ProtoChat.qml"
                                  :
                            Qt.openUrlExternally(model.link)
                    }
                }
            }


        }

    ListModel{
        id: contactmod
        ListElement{
            name: "Twitter"
            source : "Twitter @protoblock "
            subT:  "Follow @protoblock on twitter"
            link: "https://twitter.com/protoblock/"
        }
        ListElement{
            name: "Email"
            source : "contact@protoblock.com"
            subT:  "Please email us!"
            link: "mailto:contact@protoblock.com+?Subject=contact page in app"
        }
        ListElement{
            name: "Chat"
            source : "Chat Rooms"
            subT:  "See who chatting"
        }
    }

}

