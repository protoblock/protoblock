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
            subText: model.summary + " \n© 2016 NFL Enterprises LLC"
            maximumLineCount: 4
            onClicked:{
                Qt.openUrlExternally(model.link)
            }
        }
    }
    XmlListModel{
        id: xmlModel
        namespaceDeclarations: "declare default element namespace 'http://www.w3.org/2005/Atom';"
        source: "http://www.nfl.com/rss/rsslanding?searchString=home"
        query: "/feed/entry"
        XmlRole{name: "title" ; query: "title/string()"}
        XmlRole{name: "link" ; query: "link/string()"}
        XmlRole{name: "categoryOne" ; query: "category[0]/string()"}
        XmlRole{name: "categoryTwo" ; query: "category[1]/string()"}
        XmlRole{name: "summary" ; query: "summary/string()"}
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
