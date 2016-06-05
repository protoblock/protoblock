import QtQuick 2.0
import Material 1.0

Item {
    Row{
        width: systemSettingsButton.width  * 2.2
        height: systemSettingsButton.height
        spacing: 12

        IconButton {
            id: systemSettingsButton
            iconName: "qrc:/icons/action_settings.png"
            hoverAnimation: true
            onClicked: {
//                root.currentPage = "UserSettings"
                rootLoader.source  = "qrc:/UserSettings.qml"
            }
        }

        IconButton {
            iconName: "qrc:/icons/action_account_circle.png"
            onClicked: {
                rootLoader.source = "qrc:/Account.qml"
            }
        }
    }
}
