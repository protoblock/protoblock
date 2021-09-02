import QtQuick 2.12
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
                pageHelper.selectedTabIndex = 3
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
                pageHelper.selectedTabIndex = themeroot.accountIndex
                pageHelper.title = "Account Settings"
            }
        }
    }
}
