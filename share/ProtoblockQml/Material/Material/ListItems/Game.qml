import QtQuick 2.4
import QtQuick.Layouts 1.1
import Material 1.0
import ProtoblockQml.Utils 1.0

/*!
   \qmltype Subtitled
   \inqmlmodule Material.Game

   \brief A list item with a two or three lines of text and optional primary and secondary actions.
 */
BaseListItem {
    id: listItem
    height: maximumLineCount == 2 ? 72 * Units.dp : 88 * Units.dp

    property alias text: label.text
    property alias subText: subLabel.text
    property alias action: actionItem.children
    property alias secondaryItem: secondaryItem.children
    property alias itemLabel: label
    property alias itemSubLabel: subLabel
    dividerInset: actionItem.visible ? listItem.height : 0
    property int maximumLineCount: 2

    RowLayout {
        Layout.preferredWidth: parent.width / 1.07
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        spacing: 16 * Units.dp
        Layout.fillWidth: true
        Item {
            id: actionItem
            Layout.preferredWidth: 40 * Units.dp
            Layout.preferredHeight: width
            Layout.alignment: Qt.AlignCenter
            Layout.leftMargin: 3 * Units.dp
        }
        ColumnLayout {
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.fillWidth: false
            Layout.column: 2
            spacing: 3 * Units.dp
            Label {
                id: label
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                elide: Text.ElideRight
                style: "subheading"
            }
            Label {
                id: subLabel
                Layout.fillWidth: false
                Layout.preferredHeight: implicitHeight * maximumLineCount/lineCount
                color: Theme.light.subTextColor
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                style: "body1"
                visible: text != "" && !contentItem.showing
                maximumLineCount: listItem.maximumLineCount - 1
            }
        }
        ///awayTeam
        Item {
            id: secondaryItem
            Layout.preferredWidth: 40 * Units.dp
            Layout.preferredHeight: width
            Layout.alignment: Qt.AlignCenter
        }
    }
}
