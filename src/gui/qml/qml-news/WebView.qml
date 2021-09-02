import QtQuick 2.12
import Material.ListItems 1.0 as ListItems
//import QtWebEngine 1.3

Item {
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
//        WebEngineView {
//            id: webView
//            scale: wView.shown === true ? 1 : 0
//            width: wView.shown === true ?  parent.width : 0
//            height:wView.shown === true ?   parent.height - back.height : 0
//            anchors.top: back.bottom
//            onLoadProgressChanged:  {
//                if( wView.shown === true ){
//                    if (webView.loadProgress < 90){
//                        opacity = 0
//                    }else{
//                        opacity = 1
//                    }
//                }
//            }
//            Behavior on opacity {NumberAnimation{duration: 600; }}
//        }
    }
}
