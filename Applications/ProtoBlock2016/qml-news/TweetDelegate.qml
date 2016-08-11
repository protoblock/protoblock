
import QtQuick 2.0
import "tweetsearch.js" as Helper
import Material 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
Item {
    id: container
    property real hm: 1.0
    property int appear: -1
    property real startRotation: 1

    onAppearChanged: {
        container.startRotation = 0.5
        flipBar.animDuration = appear;
        delayedAnim.start();
    }

    SequentialAnimation {
        id: delayedAnim
        PauseAnimation { duration: 50 }
        ScriptAction { script: flipBar.flipDown(startRotation); }
    }

    width: ProtoScreen.guToPx(40)
    height: flipBar.height * hm

    FlipBar {
        id: flipBar

        property bool flipped: false
        delta: startRotation

        anchors.bottom: parent.bottom
        width: container.ListView.view ? container.ListView.view.width : 0
        height: Math.max(ProtoScreen.guToPx(9), frontPlaceHolder.y + frontPlaceHolder.height + ProtoScreen.guToPx(1.25))

        front: ListItems.Subtitled {
            id: frontPlaceHolder
            elevation: 4
            width: container.ListView.view ? container.ListView.view.width : 0
            action: Image {
                id: placeHolder
                source: model.userImage // "qrc:/icons/action_account_circle.png"
                fillMode: Image.PreserveAspectFit
            }

//            onClicked: {
//                Qt.openUrlExternally(model.text)
//            }

//            Image {
//                id: avatar
//                source: model.userImage
//                anchors.fill: placeHolder
//                fillMode: Image.PreserveAspectFit
//                MouseArea {
//                    id: mouseArea
//                    anchors.fill: parent
//                    onClicked: {
//                        flipBar.flipUp()
//                        flipBar.flipped = true
//                    }
//                }
//            }
            text:  model.name
            subText:  model.statusText

//                onLinkActivated: {
//                    var tag = link.split("https://twitter.com/search?q=%23")
//                    var user = link.split("https://twitter.com/")
//                    if (tag[1] != undefined) {
//                        mainListView.positionViewAtBeginning()
//                        mainListView.clear()
//                        mainListView.autoSearch('tag', tag[1])
//                        tweetsModel.from = ""
//                        tweetsModel.phrase = "#" + tag[1]
//                    } else if (user[1] != undefined) {
//                        mainListView.positionViewAtBeginning()
//                        mainListView.clear()
//                        mainListView.autoSearch('user', user[1])
//                        tweetsModel.phrase = ""
//                        tweetsModel.from = user[1]
//                    } else
//                        Qt.openUrlExternally(link)
//                }
//            }
        }

        back: Rectangle {
            width: container.ListView.view ? container.ListView.view.width : 0
            height: Math.max(ProtoScreen.guToPx(9), frontPlaceHolder.y + frontPlaceHolder.height + ProtoScreen.guToPx(1.25))
            color: "#be4a25"

            Rectangle { color: "#ff6633"; width: parent.width; height: 1 }
            Rectangle { color: "#80341a"; width: parent.width; height: 1; anchors.bottom: parent.bottom }

            Image {
                id: avatar2
                source: model.userImage
                anchors.right: parent.right
                anchors.rightMargin: ProtoScreen.guToPx(1.25)
                y: 9
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        flipBar.flipDown()
                        flipBar.flipped = false
                    }
                }
            }

            Text {
                id: username
                text: model.twitterName
                x: ProtoScreen.guToPx(1.25);
                anchors { top: avatar2.top; topMargin: -3 }
                font.pixelSize: ProtoScreen.guToPx(1.5)
                font.bold: true
                color: "white"
                linkColor: "white"
            }

            Text {
                text: model.source + "<br>" + Helper.formatDate(model.published) + "<br>" + model.uri
                x:ProtoScreen.guToPx(1.25); anchors { top: username.bottom; topMargin: 0 }
                wrapMode: Text.WordWrap
                font.pixelSize: ProtoScreen.guToPx(1.5)
                font.bold: false
                color: "#ffc2ad"
                linkColor: "white"
                onLinkActivated: Qt.openUrlExternally(link);
            }
        }
    }
}
