import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import Torrent.TorrentModel 1.0

import Material 1.0
import Material.ListItems 1.0 as ListItem
import Material.Extras 1.0

Dialog {
    QtObject{
        id: startColors

        property string primaryColor;
        property string backgroundColors;
        property string accentColor;
    }

    onVisibleChanged: {
        if ( visible ){
            startColors.primaryColor       = theme.primaryColor;
            startColors.accentColor        = theme.accentColor;
            startColors.backgroundColors   = theme.backgroundColor;
        }
    }

    onRejected: {
        theme.primaryColor      = startColors.primaryColor;
        theme.accentColor       = startColors.accentColor;
        theme.backgroundColor   = startColors.backgroundColors;
    }

    id: colorPicker
    title: "Pick color"

    positiveButtonText: "Done"

    MenuField {
        id: selection
        model: ["Primary color", "Accent color", "Background color"]
        width: Units.dp(160)
    }

    Grid {
        columns: 7
        spacing: Units.dp(8)

        Repeater {
            model: [
                "red", "pink", "purple", "deepPurple", "indigo",
                "blue", "lightBlue", "cyan", "teal", "green",
                "lightGreen", "lime", "yellow", "amber", "orange",
                "deepOrange", "grey", "blueGrey", "brown", "black",
                "white"
            ]

            Rectangle {
                width: Units.dp(30)
                height: Units.dp(30)
                radius: Units.dp(2)
                color: Palette.colors[modelData]["500"]
                border.width: modelData === "white" ? Units.dp(2) : 0
                border.color: Theme.alpha("#000", 0.26)

                Ink {
                    anchors.fill: parent

                    onPressed: {
                        switch(selection.selectedIndex) {
                        case 0:
                            theme.primaryColor = parent.color
                            break;
                        case 1:
                            theme.accentColor = parent.color
                            break;
                        case 2:
                            theme.backgroundColor = parent.color
                            break;
                        }
                    }
                }
            }
        }
    }
}
