import QtQuick 2.0
import Material 1.0

Item {
    width: systemSettingsButton.width  * 2.2 + (name.paintedWidth + (mainROw.spacing *2 ))
    height: systemSettingsButton.height

    Row{
        id: mainROw
        width: parent.width
        height:parent.height
        spacing: 12

        IconButton {
            id: systemSettingsButton
            iconName: "qrc:/icons/action_settings.png"
            hoverAnimation: true
            onClicked: {
                rootLoader.source  = "qrc:/Settings.qml"
                pageHelper.title = "System Settings"
            }
        }
            Label{
            id: name
            text: realRoot.uname

            }
        IconButton {
            iconName: "qrc:/icons/action_account_circle.png"
            onClicked: {
                rootLoader.source = "qrc:/Account.qml"
                pageHelper.selectedTabIndex = 5
                pageHelper.title = "Account Settings"
            }
        }
    }
}
