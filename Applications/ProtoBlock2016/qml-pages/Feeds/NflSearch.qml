import QtQuick 2.0
import QtQuick.XmlListModel 2.0
import Material 1.0
import Material.ListItems 1.0 as ListItems
import QtWebEngine 1.2

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




    Rectangle{
        id: wView
        scale: shown === true ? 1 : 0
        property bool shown: false
        width: shown === true ?  parent.width : 0
        height:shown === true ?   parent.height - back.height : 0
        ListItems.Standard{
            id: back
            elevation: 8
            text: "back"
            onClicked: wView.shown = false
            visible: wView.shown === true ?  true : false
        }
        WebEngineView {
            id: webView
            scale: wView.shown === true ? 1 : 0
            width: wView.shown === true ?  parent.width : 0
            height:wView.shown === true ?   parent.height - back.height : 0
            anchors.top: back.bottom
            onLoadProgressChanged:  {
                if( wView.shown === true ){
                    if (webView.loadProgress < 90){
                        opacity = 0
                    }else{
                        opacity = 1
                    }
                }
            }
            Behavior on opacity {NumberAnimation{duration: 600; }}
        }
    }
}
