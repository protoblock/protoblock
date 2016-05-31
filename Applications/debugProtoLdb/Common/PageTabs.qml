import QtQuick 2.3
import QtQuick.Controls 1.4
import  ProRotoQml.Theme 1.0

// FIXME make main page property for loader to load
Item {
    id: helperRoot
    width: rootLoader.width
    height: rootLoader.height
    property bool closeShown: false
    signal closeClicked()
    property alias  object: mainPage.children
    property int currentTab: 0

    signal clicked()
    property var tabs:[];
        Component{
            id: menuDel
            Rectangle{
                width: rep.width / 10
                height: parent.height
                color:  "transparent"
                Label{
                    id: tabText
                    color: Colors.white
                    text: tabs[index]
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    opacity: ListView.isCurrentItem ? 1 : .7
                    font.bold: ListView.isCurrentItem  ? true : false
                    font.pointSize: 20
                }
                Keys.onPressed: {
                    if (event.key === Qt.Key_Space && !event.modifiers) {
                        console.log("SPACE")
//                        helperRoot.clicked()
                        mainView.source = "qrc:/Pages/"+tabs[index] +".qml"
                        event.accepted = true;
                    }else if (event.key === Qt.Key_Enter) {
                        event.accepted = true;
                        mainView.source = "qrc:/Pages/"+tabs[index] +".qml"
                    }
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        mainView.source = "qrc:/Pages/"+tabs[index] +".qml"
                        rep.currentIndex = index
                    }
                }
            }
        }

        Component{
            id: high
            Rectangle{
                id: heightlisght
                width: parent.width /10
                height: parent.height
                color:  "#10000000"
                border.width: 1
                border.color: "#20333333"
                radius: 5
            }
        }
        SettingsMenu{
            closeShown:  helperRoot.closeShown
            onCloseClicked: helperRoot.closeClicked()
            width: parent.width / 3
            height: parent.height / 2
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }

    // All my children
    Item{
        width: helperRoot.width
        height: helperRoot.height - pageHeader.height
        anchors.top: bottomLine.bottom
        //add keynav up
        Rectangle{
            id: mainPage
            anchors.fill: parent
            color: Colors.white
            Loader{
                id: mainView
                anchors.fill: parent
                source: "qrc:/Pages/Nodes.qml"
                onLoaded: tab.focus = true
            }
        }
    }

    Rectangle{
        id: bottomLine
        color:Qt.darker( Colors.blue,2)
        width: helperRoot.width
        height: 1
        anchors.top: pageHeader.bottom
        anchors.topMargin: 2
    }

    Rectangle{
        id: pageHeader
        height: root.height / 8
        width: parent.width
        color: Colors.blue
        ListView{
            id: rep
            anchors.fill: pageHeader
            spacing: 5
            orientation: ListView.Horizontal
            focus: true
            model: tabs.length
            highlight: high
            highlightFollowsCurrentItem: true
            delegate:menuDel
        }
    }
}
