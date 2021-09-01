import QtQuick 2.5
import Material 1.0
import Material.ListItems 1.0 as ListItem
import QtQuick.Layouts 1.1


Item {
    property string strCheck: section[0]
    Sidebar {
        id: sidebar
//        width: ProtoScreen.guToPx(31.25)
        expanded: pageHelper.expanded//!navDrawer.enabled && pageHelper.expanded
        ColumnLayout {
            width: parent.width
            Repeater {
                model: section
                delegate: ListItem.Subtitled {
                    text: modelData
                    selected: modelData === strCheck
//                    onSelectedChanged: console.log(index)
                    onClicked: {
                        strCheck= section[index]
                        pageHelper.title = modelData
//                        console.log(section[index])
                        var theFile = modelData

                        rootLoader.source  = Qt.resolvedUrl(theFile.replace(/\s/g, "") + ".qml" )
                    }
                }
            }
        }
    }
}
