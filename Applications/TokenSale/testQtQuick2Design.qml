import QtQuick 2.0
import QtQuick.Extras 1.4
import QtQuick.Controls 2.0

Item {
    property string property0: "none.none"
    clip: false
    TextInput {
        id: textInput1
        x: 149
        y: 156
        width: 80
        height: 20
        color: "#9e2121"
        text: qsTr("Text Input State0")
        property string property0: "none.none"
        font.pixelSize: 12
    }

    Connections {
        target: textInput1
        onClicked: print("clicked")
    }

    Connections {
        target: textInput1
        onFontChanged: print("clicked")
    }

    Connections {
        target: parent
        onClicked: print("clicked")
    }
    states: [
        State {
            name: "State1"

            PropertyChanges {
                target: textInput1
                text: qsTr("Text Input State1")
            }
        }
    ]

}
