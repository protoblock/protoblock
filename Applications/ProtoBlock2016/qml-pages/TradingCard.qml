import QtQuick 2.4
import QtQuick.Layouts 1.1
import Material 1.0
import Material.ListItems 1.0 as ListItems
import Material.Extras 1.0

Item {
    property bool buying: true
    property string playerName
    property bool seaesonLong: false
    View {
        anchors.centerIn: parent
        backgroundColor: "#10FFFFFF"
        width: Unit.dp(350)
        height: column.implicitHeight + Unit.dp(32)
        radius: Unit.dp(2)

        ColumnLayout {
            id: column
            anchors {
                fill: parent
                topMargin: Unit.dp(16)
                bottomMargin: Unit.dp(16)
            }
            Label {
                id: titleLabel
                color: "white"
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Unit.dp(16)
                }
                style: "title"
                text: buying ? "Buy " : "Sell"
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: dp(8)
            }
            ListItems.Standard {
                action: Item {}
                content: RowLayout {
                    anchors.centerIn: parent
                    width: parent.width

                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: 0.4 * parent.width
                        text: "Ammount "
                        color: "white"
                    }

                    MenuField {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: 0.4 *  parent.width
                        model: seaesonLong  ? 400 : 40
                    }
                }
            }
            ListItems.Standard {
                action: Item {}
                content: RowLayout {
                    anchors.centerIn: parent
                    width: parent.width

                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: 0.4 * parent.width
                        text: "Number of contracts "
                        color: "white"
                    }

                    MenuField {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: 0.4 *  parent.width
                        model:  400
                    }
                }
            }

            ListItems.Standard {
                action: Image {
                    anchors.centerIn: parent
                    source: "qrc:/icons/ic_lightbulb.png"
                    height: parent.height / 1.07
                    width:  height
                    fillMode: Image.PreserveAspectFit
                }

                content: Label {
                    anchors.centerIn: parent
                    width: parent.width
                    text: "Make podcast about buy"
                    color: "white"
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: Unit.dp(8)
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: Unit.dp(8)

                anchors {
                    right: parent.right
                    margins: Unit.dp(16)
                }
                Button {
                    elevation: 5
                    text: buy ? "Buy" : "Sell"
                    backgroundColor:  Theme.primaryColor
                }
            }
        }
    }
}
