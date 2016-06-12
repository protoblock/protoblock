import QtQuick 2.0
import Material 1.0
import Material.ListItems 1.0 as ListItems


Item {
    Component.onCompleted:  pageHelper.title = qsTr("NFL News")
    ListView{
        width: parent.width
        height: parent.height
        clip: true
        spacing: 3
        model: feedsModels
        delegate: ListItems.Subtitled{
            elevation: 5
            text: model.name
            subText: model.subT
            onClicked: {
                rootLoader.source ="qrc:/"+ model.source + ".qml"
            }
        }
    }

    ListModel{
        id: feedsModels
        ListElement{
            name: "Twitter"
            source : "Twitter"
            subT:  "All the latest news from rotoworld"
        }
        ListElement{
            name: "NFL"
            source : "NFL"
            subT:  "All the latest news from rotoworld"
        }
        ListElement{
            name: "ESPN"
            source: "ESPN"
            subT:  "All the latest news from rotoworld"
        }
        ListElement{
            name: "RotoWorld"
            source: "RotoWorld"
            subT:  "All the latest news from rotoworld"
        }
        ListElement{
            name: "CBS Sports"
            source: "Feeds/CBS"
            subT:  "All the latest news from rotoworld"
        }
    }
}
