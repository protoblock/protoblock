import QtQuick 2.4
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1
import Material 1.0
import ProRotoQml.Theme 1.0 as ProtoTheme

TextFieldStyle {
    id: style

    padding {
        left: 0
        right: 0
        top: 0
        bottom: 0
    }

    font {
        family: echoMode == TextInput.Password ? "Default" : "Roboto"
        pixelSize: ProtoTheme.ProtoScreen.guToPx(2)
    }

    renderType: Text.QtRendering
    placeholderTextColor: "transparent"
    selectedTextColor: "white"
    selectionColor: control.hasOwnProperty("color") ? control.color : Theme.accentColor
    textColor: Theme.light.textColor

    background : Item {
        id: background

        property color color: control.hasOwnProperty("color") ? control.color : Theme.accentColor
        property color errorColor: control.hasOwnProperty("errorColor")
                ? control.errorColor : Palette.colors["red"]["500"]
        property string helperText: control.hasOwnProperty("helperText") ? control.helperText : ""
        property bool floatingLabel: control.hasOwnProperty("floatingLabel") ? control.floatingLabel : ""
        property bool hasError: control.hasOwnProperty("hasError")
                ? control.hasError : characterLimit && control.length > characterLimit
        property int characterLimit: control.hasOwnProperty("characterLimit") ? control.characterLimit : 0
        property bool showBorder: control.hasOwnProperty("showBorder") ? control.showBorder : true

        Rectangle {
            id: underline
            color: background.hasError ? background.errorColor
                                    : control.activeFocus ? background.color
                                                          : Theme.light.hintColor

            height: control.activeFocus ? 2  : 1
            visible: background.showBorder

            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            Behavior on height { NumberAnimation { duration: 200 } }

            Behavior on color {
                ColorAnimation { duration: 200 }
            }
        }


        Label {
            id: fieldplaceholder
            anchors.verticalCenter: parent.verticalCenter
            text: control.placeholderText
            font.pixelSize: ProtoTheme.ProtoScreen.guToPx(2)
            anchors.margins: -12
            color: background.hasError ? background.errorColor
                                  : control.activeFocus && control.text !== ""
                                        ? background.color : "#70c9c9c9"

            states: [
                State {
                    name: "floating"
                    when: control.displayText.length > 0 && background.floatingLabel
                    AnchorChanges {
                        target: fieldplaceholder
                        anchors.verticalCenter: undefined
                        anchors.top: parent.top
                    }
                    PropertyChanges {
                        target: fieldplaceholder
                        font.pixelSize: ProtoTheme.ProtoScreen.guToPx(1.5)
                    }
                },
                State {
                    name: "hidden"
                    when: control.displayText.length > 0 && !background.floatingLabel
                    PropertyChanges {
                        target: fieldplaceholder
                        visible: false
                    }
                }
            ]

            transitions: [
                Transition { id: floatingTransition; enabled: false;
                    AnchorAnimation { duration: 200; }
                    NumberAnimation { duration: 200; property: "font.pixelSize"; }
                }
            ]

            Component.onCompleted: floatingTransition.enabled = true
        }

        RowLayout {
            anchors {
                left: parent.left
                right: parent.right
                top: underline.top
                topMargin: ProtoTheme.ProtoScreen.guToPx(.5)
            }

            Label {
                id: helperTextLabel
                visible: background.helperText && background.showBorder
                text: background.helperText
                font.pixelSize: ProtoTheme.ProtoScreen.font(ProtoTheme.ProtoScreen.SMALL)
                color: background.hasError ? background.errorColor
                                           : Qt.darker(Theme.light.hintColor)
                Behavior on color { ColorAnimation { duration: 200 } }

                property string helperText: control.hasOwnProperty("helperText")
                        ? control.helperText : ""
            }

            Label {
                id: charLimitLabel
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                visible: background.characterLimit && background.showBorder
                text: control.length + " / " + background.characterLimit
                font.pixelSize: ProtoTheme.ProtoScreen.guToPx(1.5)
                color: background.hasError ? background.errorColor : Theme.light.hintColor
                horizontalAlignment: Text.AlignLeft

                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
            }
        }
    }
}
