import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import Torrent.RateController 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItem
import Material.Extras 1.0

View{
    anchors.fill: parent
    Column{
        anchors.fill: parent
        Label {
            text: "Download speed limit"
            wrapMode: Text.WordWrap
        }

        Slider {
            id: downloadSlider
            width: parent.width / 1.07
            anchors.horizontalCenter: parent.horizontalCenter
            focus: true
            tickmarksEnabled: true
            numericValueLabel: true
            stepSize: 10
            minimumValue: 80
            maximumValue: 1000
            activeFocusOnPress: true

            onValueChanged: {
                RateController.downloadLimit = value * 1024
            }

            Component.onCompleted: {
                value = RateController.downloadLimit / 1024
            }
        }

        Label {
            text: "Upload speed limit"
            wrapMode: Text.WordWrap
        }

        Slider {
            id: uploadSlider

            focus: true
            tickmarksEnabled: true
            numericValueLabel: true
            stepSize: 10
            minimumValue: 0
            maximumValue: 1000
            activeFocusOnPress: true
            width: parent.width / 1.07
            anchors.horizontalCenter: parent.horizontalCenter

            onValueChanged: {
                RateController.uploadLimit = value * 1024
            }

            Component.onCompleted: {
                value = RateController.uploadLimit / 1024
            }
        }
    }
}
