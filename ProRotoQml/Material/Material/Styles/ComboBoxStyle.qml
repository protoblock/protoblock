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
import QtQuick.Controls 1.2
//import QtQuick.Controls.Styles 1.2


ComboBoxStyle {
//    property color borderColor: "#222"
//    property color highlightColor: "orange"
//    property color textColor: "#eee"



//    Component.onCompleted: {
//        console.log(" jjtextsingletom " + TextSingleton.implicitHeight)
//        console.log("scaler " + scaler + " gutopx " + ProtoScreen.guToPx(1));

//    }

    property double scaler: 1.0
    //ProtoScreen.pxToGu(1) / 8.0``

    property double implicitheighttext: ProtoScreen.guToPx(TextSingleton.implicitHeight/8.0)
//    property double implicitwidthtext: ProtoScreen.pxToGu(TextSingleton.implicitWidth/8.0)

    /*! The padding between the background and the label components. */
    padding { top: ProtoScreen.guToPx(.50) ; left: ProtoScreen.guToPx(.75) ; right: ProtoScreen.guToPx(.75) ; bottom: ProtoScreen.guToPx(.50) }

   dropDownButtonWidth: ProtoScreen.guToPx(2.5) //ProtoScreen.guToPx(1.5)//20//scaler * 35 * maxdigits //ProtoScreen.guToPx(1.5)
    __editor: Item {
        implicitWidth: ProtoScreen.guToPx(9)
        implicitHeight:  ProtoScreen.guToPx(4) //(Math.max( ProtoScreen.guToPx(3.125), Math.round(ProtoScreen.guToPx((TextSingleton.implicitHeight )/8.0)* 1.2)))
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
            anchors.bottomMargin: ProtoScreen.guToPx(.0125)
            gradient: Gradient {
                GradientStop {color: "#e0e0e0" ; position: 0}
                GradientStop {color: "#fff" ; position: 0.1}
                GradientStop {color: "#fff" ; position: 1}
            }
            radius: ProtoScreen.guToPx(.1)
            anchors.fill: parent
            border.color: control.activeFocus ? "#47b" : "#999"
        }
        Rectangle {
            color: "#aaa"
            anchors.bottomMargin: ProtoScreen.guToPx(.25)
            anchors.topMargin: ProtoScreen.guToPx(.0125)
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: ProtoScreen.guToPx(.0125)
        }
    }

//    /*! This defines the background of the button. */
    background: Item {
        implicitWidth: ProtoScreen.guToPx(9)// ProtoScreen.guToPx(4 * maxdigits)//ProtoScreen.guToPx(1.25 * maxdigits)//Math.round(ProtoScreen.guToPx((TextSingleton.implicitHeight * 4.5)/8.0))
//        implicitHeight:  scaler * Math.max(ProtoScreen.guToPx(3.125), Math.round(implicitheighttext * 1.2))
        implicitHeight:  ProtoScreen.guToPx(4) //Math.max( ProtoScreen.guToPx(3.125), Math.round(ProtoScreen.guToPx((TextSingleton.implicitHeight )/8.0)* 1.2))

        Rectangle {
            anchors.fill: parent
            anchors.bottomMargin: /*scaler * */ (control.pressed ? 0 :  ProtoScreen.guToPx(-.125))
            color: "black" //#10000000"
            radius: baserect2.radius
        }
        Rectangle {
            id: baserect2
            gradient: Gradient {
                GradientStop {color: control.pressed ? "#bababa" : "#fefefe" ; position: 0}
                GradientStop {color: control.pressed ? "#ccc" : "#e3e3e3" ; position: 1}
            }
            radius: ProtoScreen.guToPx(.1)
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
            anchors.rightMargin: 0//-dropDownButtonWidth
//            opacity: control.enabled ? 0.6 : 0.3
//            size: ProtoScreen.guToPx(4)
            opacity: control.enabled ? 0.6 : 0.3

        }
    }

    /*! This defines the label of the button. */
    label: Item {
        implicitWidth: textitem.implicitWidth + ProtoScreen.guToPx(2.5)
        baselineOffset: textitem.y + textitem.baselineOffset
        Text {
            id: textitem
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: ProtoScreen.guToPx(.5)
            anchors.rightMargin: ProtoScreen.guToPx(1.25)
            anchors.verticalCenter: parent.verticalCenter
            text: control.currentText
            renderType: renderType
            font: font
            color: textColor
            elide: Text.ElideRight
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
