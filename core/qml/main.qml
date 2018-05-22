import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

ApplicationWindow {

    id: realRoot
    title: "Protoblock"

    height: 800
    width: 800

    Rectangle {
        anchors.fill: parent
        color: "Blue"
    }

    Component.onCompleted: {
        showNormal()
    }
}
