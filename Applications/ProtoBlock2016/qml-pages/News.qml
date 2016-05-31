import QtQuick 2.0
import "Twitter"
import Material 1.0
import Material.ListItems 1.0 as ListItems


Item {



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
            currentPage = model.source
        }


    }
}



//    Tweetsearch{
//        anchors.fill: parent
//    }








    ListModel{
        id: feedsModels
        ListElement{
            name: "Twitter"
            source : "Twitter/Tweetsearch"
            subT:  "All the latest news from rotoworld"
        }
        ListElement{
            name: "NFL"
            source : "Feeds/NflSearch"
            subT:  "All the latest news from rotoworld"
        }
        ListElement{
            name: "ESPN"
            source: "Feeds/EspnSearch"
            subT:  "All the latest news from rotoworld"
        }
        ListElement{
            name: "RotoWorld"
            source: "Feeds/RotoSearch"
            subT:  "All the latest news from rotoworld"
        }
        ListElement{
            name: "CBS Sports"
            source: "Feeds/CBSSearch"
            subT:  "All the latest news from rotoworld"
        }
    }
}
