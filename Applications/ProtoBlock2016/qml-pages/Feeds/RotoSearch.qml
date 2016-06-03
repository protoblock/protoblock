import QtQuick 2.0
import QtQuick.XmlListModel 2.0
import QtQuick.Controls 1.4
import QtWebEngine 1.2

import Material 1.0
import Material.ListItems 1.0 as ListItems

Item {
    signal reload()
    ListView{
        width: parent.width
        height: parent.height
        clip: true
        spacing: 3
        model:xmlModel
        delegate: ListItems.Subtitled{
            elevation:  5
            text: model.title
            subText: model.summary + " \nCopyright 2016, CBSSports.com"
            maximumLineCount: 4
            onClicked:{
                Qt.openUrlExternally(model.link)
            }
        }
    }
    XmlListModel{
        id: xmlModel
        source: "http://www.rotoworld.com/rss/feed.aspx?sport=nfl&ftype=article&count=50&format=rss"
        query: "/rss/channel/item"
        XmlRole{name: "title" ; query: "title/string()" }
        XmlRole{name: "summary" ; query: "description/string()" }
        XmlRole{name: "link" ; query: "link/string()" }
        onStatusChanged: {
            switch(status){
            case XmlListModel.Loading :
                console.log("Loading")
                actInd.visible = true
                break;
            case XmlListModel.Ready :
                console.log("ready count " + count )
                actInd.visible = false
                break;
            case XmlListModel.Error :
                    console.log(errorString())
                break;
            }
        }
    }
}
