import QtQuick 2.4
import QtQuick.Window 2.0
import Material 1.0
import Material.ListItems 1.0 as ListItem
import ProRotoQml.Backend 1.0


ApplicationWindow {
    id: demo
    property string gameId: "201500115"
    // we set this to 18 because there is no 18 so that it changes of the fly
    property int weekInFocus: 18
    property string leaderInView
    title: "Protoblock 2016 "
    width: Screen.width
    height:  Screen.height
    visible: true

    Component.onCompleted: {
        var attach = 'secret3.out'
        if (Qt.platform.os  === 'windows'){
            console.log("Where was 2015 installed ? " + Backend.lastKnowLoc() )
        }else if (Qt.platform.os   == 'osx'){
            userInfo.secertFile = homePath +"/tradingfootball/"+ attach
            userInfo.readData()
        }
    }
    UserInfo{
        id: userInfo
        onErrorStringChanged:{
            console.log(errorString)
        }
    }

    theme {
        primaryColor: "blue"
        accentColor: "red"
        tabHighlightColor: "white"
    }
    // Level Two
    property var styles: [
            "", "", ""
    ]
    // Level One
    property var basicComponents: [
            "LevelOn", "Leaderboard", "Schedule", "Stats",
            "Projections", "Awards", "Profit and Loss"
    ]
    // Level Three Aka Extras
    property var compoundComponents: [
            "Team News", "Hot", "Cold", "Sleepers", "Premium"
    ]

    property var sections: [ basicComponents, styles, compoundComponents ]

    property var sectionTitles: [ "Projections", "Trading", "News" ]

    property string selectedComponent: sections[0][0]

    initialPage: TabbedPage {
        id: page

        title: "ProtoBlock 2016"

        actionBar.maxActionCount: navDrawer.enabled ? 3 : 4

        actions: [


            Action {
                iconName: "image_color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
            },

            Action {
                iconName: "action_settings"
                name: "Settings"
                hoverAnimation: true
            },

            Action {
                iconName: "action_account_circle"
                name: userInfo.fantasyName
                onTriggered: userPicker.show()
            }
        ]

        backAction: navDrawer.action

        NavigationDrawer {
            id: navDrawer

            enabled: page.width < dp(500)

            onEnabledChanged: smallLoader.active = enabled

            Flickable {
                anchors.fill: parent
                contentHeight: Math.max(content.implicitHeight, height)

                Column {
                    id: content
                    anchors.fill: parent

                    Repeater {
                        model: sections

                        delegate: Column {
                            width: parent.width

                            ListItem.Subheader {
                                text: sectionTitles[index]
                            }

                            Repeater {
                                model: modelData
                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == demo.selectedComponent
                                    onClicked: {
                                        demo.selectedComponent = modelData
                                        navDrawer.close()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Repeater {
            model: !navDrawer.enabled ? sections : 0

            delegate: Tab {
                title: sectionTitles[index]

                property string selectedComponent: modelData[0]
                property var section: modelData

                sourceComponent: tabDelegate
            }
        }

        Loader {
            id: smallLoader
            anchors.fill: parent
            sourceComponent: tabDelegate

            property var section: []
            visible: active
            active: false
        }
    }

    Dialog {
        id: colorPicker
        title: "Pick color"

        positiveButtonText: "Done"

        MenuField {
            id: selection
            model: ["Primary color", "Accent color", "Background color"]
            width: dp(160)
        }

        Grid {
            columns: 7
            spacing: dp(8)

            Repeater {
                model: [
                    "red", "pink", "purple", "deepPurple", "indigo",
                    "blue", "lightBlue", "cyan", "teal", "green",
                    "lightGreen", "lime", "yellow", "amber", "orange",
                    "deepOrange", "grey", "blueGrey", "brown", "black",
                    "white"
                ]

                Rectangle {
                    width: dp(30)
                    height: dp(30)
                    radius: dp(2)
                    color: Palette.colors[modelData]["500"]
                    border.width: modelData === "white" ? dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)

                    Ink {
                        anchors.fill: parent

                        onPressed: {
                            switch(selection.selectedIndex) {
                                case 0:
                                    theme.primaryColor = parent.color
                                    break;
                                case 1:
                                    theme.accentColor = parent.color
                                    break;
                                case 2:
                                    theme.backgroundColor = parent.color
                                    break;
                            }
                        }
                    }
                }
            }
        }

        onRejected: {
            // TODO set default colors again but we currently don't know what that is
        }
    }


    // USERS INFO

    Dialog {
        id: userPicker
        title: "User Information"
        positiveButtonText: "Done"
        MenuField {
            model: userInfo.fantasyName
            width: dp(160)
        }
        Text{
            width: parent.width
            height: dp(160)
            wrapMode: Text.WordWrap
            text: "Your Secert \n"+userInfo.mnemonicKey
        }
    }

    Component {
        id: tabDelegate

        Item {

            Sidebar {
                id: sidebar

                expanded: !navDrawer.enabled

                Column {
                    width: parent.width

                    Repeater {
                        model: section
                        delegate: ListItem.Standard {
                            text: modelData
                            selected: modelData == selectedComponent
                            onClicked: selectedComponent = modelData
                        }
                    }
                }
            }
            Flickable {
                id: flickable
                anchors {
                    left: sidebar.right
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }
                clip: true
                contentHeight: Math.max(example.implicitHeight + 40, height)
                Loader {
                    id: example
                    anchors.fill: parent
                    asynchronous: true
                    visible: status == Loader.Ready
                    // selectedComponent will always be valid, as it defaults to the first component
                    source: {
                        var theFile = navDrawer.enabled ?  demo.selectedComponent : selectedComponent
                        Qt.resolvedUrl(theFile.replace(/\s/g, "") + ".qml" )
                    }
                }

                ProgressCircle {
                    anchors.centerIn: parent
                    visible: example.status == Loader.Loading
                }
            }
            Scrollbar {
                flickableItem: flickable
            }
        }
    }
}
