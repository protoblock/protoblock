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
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0

ComboBoxStyle {
//    id: style

//    __popupStyle: Style {
//        property int __maxPopupHeight: 400
//        property int submenuOverlap: 0
//        property int submenuPopupDelay: 0

//        property Component frame: Rectangle {
//            id: popupFrame
//            border.color: "white"
//            Text {
//                text: "NOT IMPLEMENTED"
//                color: "red"
//                font {
//                    pixelSize: 10
//                    bold: true
//                }
//                anchors.centerIn: parent
//                rotation: -Math.atan2(popupFrame.height, popupFrame.width) * 180 / Math.PI
//            }
//        }

//        property Component menuItemPanel: Text {
//            text: styleData.text
//        }

//        property Component __scrollerStyle: null
//    }

}
//    font {
//        family: "Roboto"
//        pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
//    }

//    property color color: control.hasOwnProperty("color")
//            ? control.color : Theme.accentColor

//    renderType: Text.QtRendering

//    selectedTextColor: "white"

//    textColor: control.pressed ? "red" : control.hovered ? "green" : "black" // Theme.light.textColor

//    selectionColor: Theme.tabHighlightColor

//    property int implicitwidth: ProtoScreen.guToPx(6)

//    padding {
//        left: style.padding.left * scaler
//        right: style.right.left * scaler
//        top: style.top.left * scaler
//        bottom: style.bottom.left * scaler
//    }

//    property double scaler: 1.0 / 1.0 // ProtoScreen.pxToGu(8)
//    property bool darkBackground: control.hasOwnProperty("darkBackground")
//            ? control.darkBackground : Theme.isDarkColor(controlBackground)


//    property int controlElevation: control.hasOwnProperty("elevation") ?
//                                       control.elevation : 1

//    property color controlBackground: control.hasOwnProperty("backgroundColor")
//            ? control.backgroundColor : controlElevation == 0 ? "transparent" : "white"


//    dropDownButtonWidth: style.dropDownButtonWidth

//    /*! This defines the background of the button. */
//    property Component background: Item {
//        implicitWidth: Math.round(scaler * (TextSingleton.implicitHeight * 4.5))
//        implicitHeight: scaler * (Math.max(25, Math.round(TextSingleton.implicitHeight * 1.2)))
//        Rectangle {
//            anchors.fill: parent
//            anchors.bottomMargin: control.pressed ? 0 : -1
//            color: "#10000000"
//            radius: baserect.radius
//        }
//        Rectangle {
//            id: baserect
//            gradient: Gradient {
//                GradientStop {color: control.pressed ? "#bababa" : "#fefefe" ; position: 0}
//                GradientStop {color: control.pressed ? "#ccc" : "#e3e3e3" ; position: 1}
//            }
//            radius: scaler * TextSingleton.implicitHeight * 0.16
//            anchors.fill: parent
//            border.color: control.activeFocus ? "#47b" : "#999"
//            Rectangle {
//                anchors.fill: parent
//                radius: parent.radius
//                color: control.activeFocus ? "#47b" : "white"
//                opacity: control.hovered || control.activeFocus ? 0.1 : 0
//                Behavior on opacity {NumberAnimation{ duration: 100 }}
//            }
//        }
//        Image {
//            id: imageItem
//            visible: control.menu !== null
//            source: "qrc:/icons/arrow-drop-down.png"//"images/arrow-down.png"
//            anchors.verticalCenter: parent.verticalCenter
//            anchors.right: parent.right
//            anchors.rightMargin: (dropDownButtonWidth / 2)
//            opacity: control.enabled ? 0.6 : 0.3
//        }
//    }

//    /*! \internal */
//    property Component __editor: Item {
//        implicitWidth: scaler * 100
//        implicitHeight: scaler * (Math.max(25, Math.round(TextSingleton.implicitHeight * 1.2)))
//        clip: true
//        Rectangle {
//            anchors.fill: parent
//            anchors.bottomMargin: 0
//            color: "#44ffffff"
//            radius: baserect.radius
//        }
//        Rectangle {
//            id: baserect
//            anchors.rightMargin: -radius
//            anchors.bottomMargin: scaler * 1
//            gradient: Gradient {
//                GradientStop {color: "#e0e0e0" ; position: 0}
//                GradientStop {color: "#fff" ; position: 0.1}
//                GradientStop {color: "#fff" ; position: 1}
//            }
//            radius: scaler * (TextSingleton.implicitHeight * 0.16)
//            anchors.fill: parent
//            border.color: control.activeFocus ? "#47b" : "#999"
//        }
//        Rectangle {
//            color: "#aaa"
//            anchors.bottomMargin: scaler * 2
//            anchors.topMargin: scaler * 1
//            anchors.right: parent.right
//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//            width: scaler * 1
//        }
//    }

//    /*! This defines the label of the button. */
//    property Component label: Item {
//        implicitWidth: scaler * (textitem.implicitWidth + 20)
//        baselineOffset: textitem.y + textitem.baselineOffset
//        Text {
//            id: textitem
//            anchors.left: parent.left
//            anchors.right: parent.right
//            anchors.leftMargin: scaler * 4
//            anchors.rightMargin: scaler * 10
//            anchors.verticalCenter: parent.verticalCenter
//            text: control.currentText
//            renderType: style.renderType
//            font: font
//            color: textColor
//            elide: Text.ElideRight
//        }
//    }

//    /*! \internal */
//    property Component panel: Item {
//        property bool popup: false
//        property font font: font
//        property color textColor: textColor
//        property color selectionColor: selectionColor
//        property color selectedTextColor: selectedTextColor
//        property int dropDownButtonWidth: dropDownButtonWidth
//        anchors.centerIn: parent
//        anchors.fill: parent
//        implicitWidth: scaler * backgroundLoader.implicitWidth
//        implicitHeight:scaler * Math.max(labelLoader.implicitHeight + padding.top + padding.bottom, backgroundLoader.implicitHeight)
//        baselineOffset:scaler * labelLoader.item ? padding.top + labelLoader.item.baselineOffset: 0

//        Loader {
//            id: backgroundLoader
//            anchors.fill: parent
//            sourceComponent: background

//        }

//        Loader {
//            id: editorLoader
//            anchors.fill: parent
//            anchors.rightMargin: (dropDownButtonWidth + padding.right)
//            anchors.bottomMargin: -1
//            sourceComponent: control.editable ? __editor : null
//        }

//        Loader {
//            id: labelLoader
//            sourceComponent: label
//            visible: !control.editable
//            anchors.fill: parent
//            anchors.leftMargin: padding.left
//            anchors.topMargin: padding.top
//            anchors.rightMargin: padding.right
//            anchors.bottomMargin: padding.bottom
//        }
//    }

//    /*! \internal */
//    property Component __dropDownStyle: MenuStyle {
//        font: font
//        __labelColor: textColor
//        __selectedLabelColor: selectedTextColor
//        __selectedBackgroundColor: selectionColor
//        __maxPopupHeight: scaler * 600
//        __menuItemType: "comboboxitem"
//        __scrollerStyle: ScrollViewStyle { }
//    }

//    /*! \internal */
//    property Component __popupStyle: Style {
//        property int __maxPopupHeight: scaler * 400
//        property int submenuOverlap: 0
//        property int submenuPopupDelay: 10

//        property Component frame: Rectangle {
//            id: popupFrame
//            border.color: "white"
//            Text {
//                text: "NOT IMPLEMENTED"
//                color: "red"
//                font {
//                    pixelSize: scaler * 10
//                    bold: true
//                }
//                anchors.centerIn: parent
//                rotation: -Math.atan2(popupFrame.height, popupFrame.width) * 180 / Math.PI
//            }
//        }

//        property Component menuItemPanel: Text {
//            text: styleData.text
//        }

//        property Component __scrollerStyle: null
//    }

    /*! \internal
        The cursor handle.
        \since QtQuick.Controls.Styles 1.3

        The parent of the handle is positioned to the top left corner of
        the cursor position. The interactive area is determined by the
        geometry of the handle delegate.

        The following signals and read-only properties are available within the scope
        of the handle delegate:
        \table
            \row \li \b {styleData.activated()} [signal] \li Emitted when the handle is activated ie. the editor is clicked.
            \row \li \b {styleData.pressed} : bool \li Whether the handle is pressed.
            \row \li \b {styleData.position} : int \li The character position of the handle.
            \row \li \b {styleData.lineHeight} : real \li The height of the line the handle is on.
            \row \li \b {styleData.hasSelection} : bool \li Whether the editor has selected text.
        \endtable
    */
//    property Component __cursorHandle

    /*! \internal
        The selection handle.
        \since QtQuick.Controls.Styles 1.3

        The parent of the handle is positioned to the top left corner of
        the first selected character. The interactive area is determined
        by the geometry of the handle delegate.

        The following signals and read-only properties are available within the scope
        of the handle delegate:
        \table
            \row \li \b {styleData.activated()} [signal] \li Emitted when the handle is activated ie. the editor is clicked.
            \row \li \b {styleData.pressed} : bool \li Whether the handle is pressed.
            \row \li \b {styleData.position} : int \li The character position of the handle.
            \row \li \b {styleData.lineHeight} : real \li The height of the line the handle is on.
            \row \li \b {styleData.hasSelection} : bool \li Whether the editor has selected text.
        \endtable
//    */
//    property Component __selectionHandle

    /*! \internal
        The cursor delegate.
        \since QtQuick.Controls.Styles 1.3
    */
//    property Component __cursorDelegate


/*
    background: Item {
        implicitWidth: implicitwidth
        implicitHeight: implicitwidth / 2.5
        Rectangle {
            anchors.fill: parent
            anchors.bottomMargin: control.pressed ? 0 : ProtoScreen.guToPx(-.125)
            color: "#10000000"
            radius: baserect.radius
        }
        Rectangle {
            id: baserect
            gradient: Gradient {
                GradientStop {color: control.pressed ? "#bababa" : "#fefefe" ; position: 0}
                GradientStop {color: control.pressed ? "#ccc" : "#e3e3e3" ; position: 1}
            }
            radius: implicitHeight * 0.05
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
        Image {
            id: imageItem
            visible: control.menu !== null
            source: "qrc:/icons/arrow-drop-down.png"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: dropDownButtonWidth / 2
            opacity: control.enabled ? 0.6 : 0.3
        }
    }

    property Component __editor: Item {
        implicitWidth: implicitwidth
        implicitHeight: implicitwidth / 2.5
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
            anchors.bottomMargin: 1
            gradient: Gradient {
                GradientStop {color: "#e0e0e0" ; position: 0}
                GradientStop {color: "#fff" ; position: 0.1}
                GradientStop {color: "#fff" ; position: 1}
            }
            radius: implicitHeight * 0.05
            anchors.fill: parent
            border.color: control.activeFocus ? "#47b" : "#999"
        }
        Rectangle {
            color: "#aaa"
            anchors.bottomMargin: ProtoScreen.guToPx(.25)
            anchors.topMargin: ProtoScreen.guToPx(.125)
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: ProtoScreen.guToPx(.125)
        }
    }

    property Component label: Item {
        implicitWidth: textitem.implicitWidth + ProtoScreen.guToPx(2.5)
        baselineOffset: textitem.y + textitem.baselineOffset
        Text {
            id: textitem
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: ProtoScreen.guToPx(.25)
            anchors.rightMargin: ProtoScreen.guToPx(2.25)
            anchors.verticalCenter: parent.verticalCenter
            text: control.currentText
            renderType: cbStyle.renderType
            font: cbStyle.font
            color: cbStyle.textColor
            elide: Text.ElideRight
        }
    }

    property Component panel: Item {
        property bool popup: false
        property font font: cbStyle.font
        property color textColor: cbStyle.textColor
        property color selectionColor: cbStyle.selectionColor
        property color selectedTextColor: cbStyle.selectedTextColor
        property int dropDownButtonWidth: cbStyle.dropDownButtonWidth
        anchors.centerIn: parent
        anchors.fill: parent
        implicitWidth: backgroundLoader.implicitWidth
        implicitHeight: Math.max(labelLoader.implicitHeight + padding.top + padding.bottom, backgroundLoader.implicitHeight)
        baselineOffset: labelLoader.item ? padding.top + labelLoader.item.baselineOffset: 0

        Loader {
            id: backgroundLoader
            anchors.fill: parent
            sourceComponent: background

        }

        Loader {
            id: editorLoader
            anchors.fill: parent
            anchors.rightMargin: dropDownButtonWidth + padding.right
            anchors.bottomMargin: -1
            sourceComponent: control.editable ? __editor : null
        }

        Loader {
            id: labelLoader
            sourceComponent: label
            visible: !control.editable
            anchors.fill: parent
            anchors.leftMargin: padding.left
            anchors.topMargin: padding.top
            anchors.rightMargin: padding.right
            anchors.bottomMargin: padding.bottom
        }
    }

    property Component __dropDownStyle: MenuStyle {
        font: cbStyle.font
        __labelColor: cbStyle.textColor
        __selectedLabelColor: cbStyle.selectedTextColor
        __selectedBackgroundColor: cbStyle.selectionColor
        __maxPopupHeight: ProtoScreen.guToPx(75)
        __menuItemType: "comboboxitem"
        __scrollerStyle: ScrollViewStyle { }
    }


    property Component __popupStyle: Style {
        property int __maxPopupHeight: ProtoScreen.guToPx(50)
        property int submenuOverlap: 0
        property int submenuPopupDelay: 30

        property Component frame: Rectangle {
            id: popupFrame
            border.color: "white"
            Text {
                text: "NOT IMPLEMENTED"
                color: "red"
                font {
                    pixelSize: 10
                    bold: true
                }
                anchors.centerIn: parent
                rotation: -Math.atan2(popupFrame.height, popupFrame.width) * ProtoScreen.guToPx(22.5) / Math.PI
            }
        }

        property Component menuItemPanel: Text {
            text: styleData.text
        }

        property Component __scrollerStyle: null
    }
*/
//}

