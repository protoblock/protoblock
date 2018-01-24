import QtQuick 2.0
import Communi 3.4

Item {
    TextEdit {
        id: textEdit1
        text: "kadnjsadnfowjdbscks\n\n\n\nds\nf\ndsaf\ndsf\nsda\n"
        readOnly: true
        textFormat: Text.RichText
        anchors.fill: parent
        font.pixelSize: 12

        PathView {
            id: pathView1
            x: 223
            y: 88
            width: 299
            height: 222
            path: Path {
                startY: 100
                startX: 120

                PathCubic {
                    x: 120
                    y: 25
                    control1X: 213.333
                    control2Y: 58.333
                    control2X: 213.333
                    control1Y: 83.333
                }

                PathCubic {
                    x: 120
                    y: 100
                    control1X: 26.667
                    control2Y: 83.333
                    control2X: 26.667
                    control1Y: 58.333
                }
            }
            model: ListModel {
                ListElement {
                    name: "Grey"
                    colorCode: "grey"
                }

                ListElement {
                    name: "Red"
                    colorCode: "red"
                }

                ListElement {
                    name: "Blue"
                    colorCode: "blue"
                }

                ListElement {
                    name: "Green"
                    colorCode: "green"
                }
            }
            delegate: Component {
                Column {
                    spacing: 5
                    Rectangle {
                        width: 40
                        height: 40
                        color: colorCode
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        x: 5
                        text: name
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.bold: true
                    }
                }
            }
        }
    }

}
