import QtQuick 2.0
import "tweetsearch.js" as Helper

Rectangle {
    id: main
//    width: parent.width
//    height: parent.height
    color: "transparent"

    property int inAnimDur: 250
    property int counter: 0
    property alias isLoading: tweetsModel.isLoading
    property var idx
    property var ids

    Component.onCompleted: ids = new Array()

    function idInModel(id)
    {
        for (var j = 0; j < ids.length; j++)
            if (ids[j] === id)
                return 1
        return 0
    }

    TweetsModel {
        id: tweetsModel
        onIsLoaded: {
//            console.debug("Reload")
            idx = new Array()
            for (var i = 0; i < tweetsModel.model.count; i++) {
                var id = tweetsModel.model.get(i).id
                if (!idInModel(id))
                    idx.push(i)
            }
            main.counter = idx.length
        }
    }

    Timer {
        id: timer
        interval: 500; running: main.counter; repeat: true
        onTriggered: {
            main.counter--;
            var id = tweetsModel.model.get(idx[main.counter]).id
            var item = tweetsModel.model.get(main.counter)
            mainListView.add( { "statusText": Helper.insertLinks(item.text, item.entities),
                                "twitterName": item.user.screen_name,
                                "name" : item.user.name,
                                "userImage": item.user.profile_image_url,
                                "source": item.source,
                                "id": id,
                                 "uri": Helper.insertLinks(item.user.url, item.user.entities),
                                "published": item.created_at } );
            ids.push(id)
        }
    }

    ListView {
        id: mainListView
        anchors.fill: parent
        delegate: TweetDelegate { }
        model: ListModel { id: finalModel }

        add: Transition {
            NumberAnimation { property: "hm"; from: 0; to: 1.0; duration: 300; easing.type: Easing.OutQuad }
            PropertyAction { property: "appear"; value: 250 }
        }

        onDragEnded: if (header.refresh) { tweetsModel.reload() }

        ListHeader {
            id: header
            y: -mainListView.contentY - height
        }

        footer: ListFooter { }

        function clear() {
            ids = new Array()
            model.clear()
        }

        function add(obj) {
            model.insert(0, obj)
        }

        signal autoSearch(string type, string str) // To communicate with Footer instance
    }
}
