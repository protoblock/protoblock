/*
 * QML Material - An application framework implementing Material Design.
 *
 * Copyright (C) 2015-2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.4
import QtQuick.Controls.Styles 1.3
import Material 1.0
import ProRotoQml.Utils 1.0
import ProRotoQml.Theme 1.0

import QtQuick.Controls.Private 1.0
//import QtQuick 2.4
import QtQuick.Controls 1.2 as Controls
//import QtQuick.Controls.Styles 1.2

ComboBoxStyle {
//    property color borderColor: "#222"
//    property color highlightColor: "orange"
//    property color textColor: "#eee"

    property double scaler: ProtoScreen.guToPx(1) / 8.0

    property double implicitheighttext: ProtoScreen.pxToGu(13)
    property double implicitwidthtext: ProtoScreen.pxToGu(6)

    /*! The padding between the background and the label components. */
    padding { top: scaler * 4.0 ; left: scaler * 6.0 ; right: scaler *6 ; bottom:scaler *4 }

   dropDownButtonWidth: scaler * Math.round(TextSingleton.implicitHeight) //ProtoScreen.guToPx(1.5)//20//scaler * 35 * maxdigits //ProtoScreen.guToPx(1.5)
    __editor: Item {
        implicitWidth: scaler * 33.33 * maxdigits
        implicitHeight: scaler * (Math.max(25, Math.round(TextSingleton.implicitHeight * 1.2)))
        clip: true
        Rectangle {
            anchors.fill: parent
            anchors.bottomMargin: 0
            color: "#44ffffff"
            radius: baserect.radius
        }
        Rectangle {
            id: baserect
            anchors.rightMargin: -radius
            anchors.bottomMargin: scaler
            gradient: Gradient {
                GradientStop {color: "#e0e0e0" ; position: 0}
                GradientStop {color: "#fff" ; position: 0.1}
                GradientStop {color: "#fff" ; position: 1}
            }
            radius: scaler * (TextSingleton.implicitHeight * 0.16)
            anchors.fill: parent
            border.color: control.activeFocus ? "#47b" : "#999"
        }
        Rectangle {
            color: "#aaa"
            anchors.bottomMargin: scaler * 2.0
            anchors.topMargin: scaler * 1.0
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: scaler * 1.0
        }
    }

//    /*! This defines the background of the button. */
    background: Item {
        implicitWidth: scaler *  Math.round(TextSingleton.implicitHeight * 4.5)
        implicitHeight:  scaler * Math.max(25, Math.round(TextSingleton.implicitHeight * 1.2))
        Rectangle {
            anchors.fill: parent
            anchors.bottomMargin: /*scaler * */ (control.pressed ? 0 : -scaler)
            color: "black" //#10000000"
            radius: baserect.radius * scaler
        }
        Rectangle {
            id: baserect
            gradient: Gradient {
                GradientStop {color: control.pressed ? "#bababa" : "#fefefe" ; position: 0}
                GradientStop {color: control.pressed ? "#ccc" : "#e3e3e3" ; position: 1}
            }
            radius: scaler * TextSingleton.implicitHeight * 0.16
            anchors.fill: parent
            border.color: control.activeFocus ? "#47b" : "#999"
            Rectangle {
                anchors.fill: parent
                radius: parent.radius
                color: control.activeFocus ? "#47b" : "white"
                opacity: control.hovered || control.activeFocus ? 0.1 : 0
                Behavior on opacity {NumberAnimation{ duration: 100 }}
            }
        }
        Icon {
            id: imageItem2
            visible: true//control.menu !== null
            source: "qrc:/icons/arrow_drop_down.png"
//            C:\work\protoblock\ arrow_drop_down.png
//            source: "qrc:/icons/ic_trending_down.png"
//            source: "qrc:/icons/navigation/arrow_drop_down.svg"
//            source: "qrc:/icons/ic_help.png"
//            source: "images/arrow-down.png"
            color: "black"
//            anchors.fill: parent
//            anchors.centerIn: parent
//            hasColor:true
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: dropDownButtonWidth * -.15
//            opacity: control.enabled ? 0.6 : 0.3
//            size: ProtoScreen.guToPx(4)
            opacity: control.enabled ? 0.6 : 0.3

        }
    }


//    background: Item {
//        implicitWidth: 120
//        implicitHeight: 25

//        RoundedPanel {
//            anchors.fill: parent
//            roundLeft: true
//            roundRight: true
//            visible: !control.pressed
//        }

//        RoundedPanel {
//            gradient: Gradient {
//                GradientStop {color: '#444' ; position: 0}
//                GradientStop {color: '#333' ; position: 1}
//            }
//            anchors.fill: parent
//            roundLeft: true
//            roundRight: true
//            visible: control.pressed
//        }

//        Rectangle {
//            border.color: highlightColor
//            anchors.fill: parent
//            anchors.margins: -1
//            color: "transparent"
//            opacity: 0.3
//            visible: control.activeFocus
//        }

//        Rectangle {
//            color: "#333"
//            width: 1
//            anchors.right: imageItem.left
//            anchors.topMargin: 4
//            anchors.bottomMargin: 4
//            anchors.rightMargin: 6
//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//        }

//        Image {
//            id: imageItem
//            source: "images/down-arrow.png"
//            anchors.verticalCenter: parent.verticalCenter
//            anchors.right: parent.right
//            anchors.rightMargin: 10
//            opacity: control.enabled ? 0.7 : 0.5
//        }
//    }

//    label: Item {
//        implicitWidth: textitem.implicitWidth + 20
//        Text {
//            id: textitem
//            anchors.left: parent.left
//            anchors.leftMargin: 14
//            anchors.verticalCenter: parent.verticalCenter
//            text: control.currentText
//            renderType: Text.NativeRendering
//            color: textColor
//        }
//    }

//    __dropDownStyle: MenuStyle {
//        __maxPopupHeight: 600
//        __menuItemType: "comboboxitem"
//        frame: Rectangle {
//        }
//    }
}
