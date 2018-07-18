import QtQuick 2.0
import QtQuick.XmlListModel 2.0
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
        }
    }
    XmlListModel{
        id: xmlModel
        source: "http://www.cbssports.com/partners/feeds/rss/nfl_news"
        query: "/rss/channel/item"
        XmlRole{name: "title" ; query: "title/string()" }
        XmlRole{name: "summary" ; query: "description/string()" }
        XmlRole{name: "link" ; query: "link/string()" }
    }
}
