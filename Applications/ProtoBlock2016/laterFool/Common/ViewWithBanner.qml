import QtQuick 2.4
FocusScope {
    id: list
    property Component bodyDelegate
    property variant model
    property string bannerText
    property int headerHeight: 64
    property bool expanded: true
    property alias currentHeight: bodyItem.height
    width: 20
    height:     20
    Banner{
        id: banner
        width: parent.width
        height: headerHeight
        text: {
            if( delLoader.count > 1 ){
            bannerText + " ("+delLoader.count +")"
            }else{
                bannerText
            }
        }
            open: expanded
        tickShown: true
        MouseArea{
            anchors.fill: banner
            onClicked:{
                if(expanded === true){
                    expanded = false

                }else if (expanded !== true ){
                    expanded = true
                }
            }
        }
    }
    Item {
        id: bodyItem
        clip: true
        width: parent.width
        height: {
            if(expanded === true){
                parent.height - banner.height
            }else{
                0
            }
        }
        Behavior on height {NumberAnimation{duration: 1200 ; easing.type: Easing.InOutCubic}}
        anchors.top: banner.bottom
        ListView{
            id: delLoader
            width: bodyItem.width
            height: bodyItem.height
            model: list.model
            delegate:  bodyDelegate

        }
    }
}


