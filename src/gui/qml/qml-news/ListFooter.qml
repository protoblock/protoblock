
import QtQuick 2.12
import ProRotoQml.Theme 1.0
Rectangle {
    color: "transparent"
    width: parent.width
    height: childrenRect.height
    z: 2
    Connections {
        target: mainListView
        onAutoSearch: {
            if (type == 'tag') {
                tagSearch.open()
                tagSearch.searchText = str
            } else if (type == 'user'){
                userSearch.open()
                userSearch.searchText = str
            }
//            else {
//                wordSearch.open()
//                wordSearch.searchText = str
//            }
        }
    }

    Column {
        width: parent.width

//        SearchDelegate {
//            id: wordSearch
//            label: "Search word..."
//            placeHolder: "Enter word"
//            onHasOpened: {
//                tagSearch.close()
//                userSearch.close()
//            }
//            onOk: {
//                mainListView.positionViewAtBeginning()
//                mainListView.clear()
//                tweetsModel.from = ""
//                tweetsModel.phrase = searchText
//            }
//        }

        SearchDelegate {
            id: userSearch
            label: "From user..."
            placeHolder: "@prototicker"
            prefix: "@prototicker"
            onHasOpened:{
                tagSearch.close()
//                wordSearch.close()
            }
            onOk: {
                mainListView.positionViewAtBeginning()
                mainListView.clear()
                tweetsModel.phrase = ""
                tweetsModel.from = searchText
            }
            Component.onCompleted: {
                initTimmer.start()
            }
        }

        Timer{
            id: initTimmer
            interval: 200
            repeat: false
            running: false
            onTriggered: {
                mainListView.positionViewAtBeginning()
                mainListView.clear()
                tweetsModel.phrase = ""
                tweetsModel.from = "@prototicker"
            }

        }

        SearchDelegate {
            id: tagSearch
            label: "Search hashtag..."
            placeHolder: "#"
            prefix: "#fantasyfootball"
            onHasOpened:{
                userSearch.close()
//                wordSearch.close()
            }
            onOk: {
                mainListView.positionViewAtBeginning()
                mainListView.clear()
                tweetsModel.from = ""
                tweetsModel.phrase = "#" + searchText
            }
        }

//        SpriteSequence {
//            id: sprite
//            anchors.horizontalCenter: parent.horizontalCenter
//            width: ProtoScreen.guToPx(40)
//            height: ProtoScreen.guToPx(40)
//            running: true
//            interpolate: false
//            Sprite {
//                name: "bird"
//                source: "qrc:/bird-anim-sprites.png"
//                frameCount: 1
//                frameRate: 1
//                frameWidth:ProtoScreen.guToPx(40)
//                frameHeight: ProtoScreen.guToPx(40)
//                to: { "bird":10, "trill":1, "blink":1 }
//            }
//            Sprite {
//                name: "trill"
//                source: "qrc:/bird-anim-sprites.png"
//                frameCount: 5
//                frameRate: 3
//                frameWidth: ProtoScreen.guToPx(40)
//                frameHeight: ProtoScreen.guToPx(40)
//                to: {"bird":1}
//            }
//            Sprite {
//                name: "blink"
//                source: "qrc:/bird-anim-sprites.png"
//                frameCount: 1
//                frameRate: 3
//                frameWidth: ProtoScreen.guToPx(40)
//                frameHeight: ProtoScreen.guToPx(40)
//                frameX: ProtoScreen.guToPx(200)
//                to: {"bird":1}
//            }
//        }
    }
}
