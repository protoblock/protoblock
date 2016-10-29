import QtQuick 2.3
import QtQuick.Controls 1.4

import Material 1.0
import Material.ListItems 1.0 as ListItem

Item {
    id: helperRoot
    width: rootLoader.width
    height: rootLoader.height
    property string title
    property bool closeShown: false
    property bool buttonsEnabled: true
    property alias  object: mainPage.children
    property alias m_opacity: mainPage.opacity
    signal closeClicked()
    signal settingsClicked()
    Item{
        width: helperRoot.width
        height: helperRoot.height - pageHeader.height
        anchors.top: settingsMenu.top
        Rectangle{
            id: mainPage
            anchors.fill: parent
            color:"#30c7c7c7"
        }
    }

    //settings menu

    Card{
        id: settingsMenu
        width: parent.width / 1.5
        height: mainPage.height
        property bool shown: false
        elevation: shown ? 10 : 0
        opacity: shown === true ?  1 : 0
        x: shown ? parent.width  - width:  parent.width
        anchors.top: bottomLine.bottom
        Behavior on x {NumberAnimation{duration: 600; easing.type: Easing.OutBack}}
        Rectangle{
            id: settingsMenuHeader
            width: parent.width
            height: parent.height / 14
            color: "#2196f3"
            opacity: settingsMenu.opacity
            Label{
                id: settingsTXT
                text: qsTr("Settings")
                font.family: "Roboto"
                font.bold: true
                font.pixelSize: parent.height / 2
                height: parent.height
                width: parent.width - 10
                color: "white"
                anchors.left: parent.left
                anchors.leftMargin: 10
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WrapAnywhere
                opacity: settingsMenu.opacity
            }
        }
        Rectangle{
            id: spacer
            width: parent.width / 1.07
            color: "#c3c3c3"
            height: 1
            anchors.top: settingsMenuHeader.bottom
            anchors.topMargin: 2
            opacity: settingsMenu.opacity

        }
        ListView{
            width: parent.width / 1.07
            height: parent.height - ( settingsMenuHeader.height + 4)
            anchors.top: spacer.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5
            opacity: settingsMenu.opacity
            model: settingsModel
            delegate: ListItem.Standard{
                width: parent.width
                height: pageHelper.object.height / 8
                elevation: 5
                opacity: settingsMenu.opacity
                text: name === "Home" ? themeroot.uname : name
                // FIXME set this as a platform.os android
                itemLabel.font.pixelSize: 48
                action: Image {
                    source: image
                    width: height
                    height: parent.height
                }
                onClicked: {
                    if (name === "Quit"){
                        Qt.quit();
                    }else{
                        currentPage  = path
                        settingsMenu.shown = false
                    }
                }
            }
        }
    }

    Rectangle{
        id: bottomLine
        color:  Qt.darker("#2196f3",2);
        width: helperRoot.width
        height: 1
        anchors.top: pageHeader.bottom
        anchors.topMargin: 2
    }
    Rectangle{
        id: pageHeader
        height: themeroot.height / 10
        width: parent.width
        color:   "#2196f3"
        Image {
            id: logoOnly
            source: "qrc:/logoOnly.png"
            height: parent.height / 1.8
            width: height + 20
            fillMode: Image.PreserveAspectFit
            anchors.verticalCenter: parent.verticalCenter

            MouseArea{
                anchors.fill: parent
                onClicked:{
                    if(buttonsEnabled === true){
                        currentPage  = "WelcomeBack"
                    }
                }
            }
        }
        Label{
            text: qsTr(helperRoot.title)
            font.family: "Roboto"
            font.pixelSize: parent.height / 2
            font.bold: true
            height: parent.height
            width: parent.width - (logoOnly.width + cog.width)
            elide: Text.ElideRight
            color: "white"
            verticalAlignment: Text.AlignVCenter
            anchors{
                left: logoOnly.right
                leftMargin: 15
            }
        }
//        Image {
//            id: cog
//            source: "qrc:/icons/action_settings.png"
//            anchors.right: parent.right
//            anchors.rightMargin: 15
//            anchors.verticalCenter: parent.verticalCenter
//            width: height
//            height: parent.height / 2
//            rotation: 0
//            Behavior on rotation{RotationAnimation{duration: 600; from : 0 ; to : 360 ; }}
//            MouseArea{
//                anchors.fill: parent
//                onClicked: {
//                    if(buttonsEnabled === true){
//                        cog.rotation = 1
//                        settingsMenu.shown ?  settingsMenu.shown = false : settingsMenu.shown = true
//                    }
//                }
//            }
//        }
    }

    ListModel{
        id: settingsModel
        ListElement{
            name: "Home"
            path: "Settings"
            image: "qrc:/logoOnly.png"
        }
        ListElement{
            name: "Quick links"
            path: "WelcomeBack"
            image: "qrc:/icons/ic_lightbulb.png"
        }
        ListElement{
            name: "2015 Results"
            path: "Welcome"
            image: "qrc:/icons/ic_poll.png"
        }
        ListElement{
            name: "Chat"
            path: "Chat"
            image: "qrc:/icons/ic_help.png"
        }
        ListElement{
            name: "News"
            path: "News"
            image: "qrc:/icons/ic_help.png"
        }
        ListElement{
            name: "Quit"
            path: ""
            image: "qrc:/icons/power_settings.png"
        }
    }

}

