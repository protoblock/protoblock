/*
 * QML Material - An application framework implementing Material Design.
 *
 * Copyright (C) 2015-2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.4
import QtQuick.Layouts 1.1

import Material 1.0
import ProRotoQml.Theme 1.0
import Material.ListItems 1.0

Item {
    id: tabBar

    property bool centered: false
    property var tabs: []
    property int leftKeyline

    property bool isLargeDevice: ProtoScreen.formFactor === "desktop" ||  ProtoScreen.formFactor === "tablet" ||  ProtoScreen.formFactor === "tv"

    property bool fullWidth: isLargeDevice
            ? false : width - maxTabsWidth <= ProtoScreen.guToPx(2) && tabsWidth <= width

    property int tabsWidth: {
        var width = 0
        for (var i = 0; i < tabRow.children.length; i++) {
            width += tabRow.children[i].implicitWidth
        }

        return width
    }

    property int maxTabsWidth: {
        var tabWidth = 0

        for (var i = 0; i < tabRow.children.length; i++) {
            tabWidth = Math.max(tabRow.children[i].implicitWidth, tabWidth)
        }

        return tabWidth * tabRow.children.length
    }

    property int tabPadding: isLargeDevice ? ProtoScreen.guToPx(3) : ProtoScreen.guToPx(1.5)
    property int tabMinWidth: isLargeDevice ? ProtoScreen.guToPx(20) : ProtoScreen.guToPx(9)
    property int
    Index: 0
    property bool darkBackground
    property color highlightColor: Theme.tabHighlightColor
    property color textColor: darkBackground ? Theme.dark.textColor : Theme.light.accentColor
    property bool isTabView: String(tabs).indexOf("TabView") != -1
    readonly property int tabCount: isTabView ? tabs.count : tabs.length

    visible: tabCount > 0
    implicitHeight: ProtoScreen.guToPx(6)
//    anchors.bottom: parent.bottom
    onTabCountChanged: {
        selectedIndex = Math.min(selectedIndex, tabCount)
    }

    function removeTab(tab, index) {
        if (tab.hasOwnProperty("close")) {
            tab.close()
        } else if (tabs.hasOwnProperty("removeTab")) {
            tabs.removeTab(index)
        }
    }

    property Item activeTab: selectedIndex < tabCount &&
                             tabRow.children[selectedIndex] !== undefined
            ? tabRow.children[selectedIndex] : null

    property int activeTabX: activeTab ? activeTab.x : -1
    property int activeTabWidth: activeTab ? activeTab.width : -1

    property int __animated

    onActiveTabChanged: __animated = false

    Component.onCompleted: animateStablization.restart()

    onActiveTabXChanged: {
        if (activeTabX == -1)
            return
        if (__animated) {
            selectionIndicator.x = activeTab.x
        } else {
            animateStablization.restart()
        }
    }
    onActiveTabWidthChanged: {
        if (activeTabWidth == -1)
            return
        if (__animated) {
            selectionIndicator.width = activeTab.width
        } else {
            animateStablization.restart()
        }
    }

    Timer {
        id: animateStablization
        interval: 5
        onTriggered: {
            __animated = true
            seqX.to = activeTabX
            seqWidth.to = activeTabWidth
            selectionAnimation.restart()
        }
    }

    ParallelAnimation {
        id: selectionAnimation

        NumberAnimation {
            id: seqX
            target: selectionIndicator; property: "x"; duration: 200
        }
        NumberAnimation {
            id: seqWidth
            target: selectionIndicator; property: "width"; duration: 200
        }
    }

    Item {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: centered ? undefined : parent.left
            leftMargin: fullWidth ? 0 : Math.max(leftKeyline - tabPadding, 0)
            horizontalCenter: centered ? parent.horizontalCenter : undefined
        }

        width: tabRow.width

        Row {
            id: tabRow
            height: parent.height

            Repeater {
                id: repeater
                model: isTabView ? tabs.count : tabs
                delegate: tabDelegate
            }
        }

        Rectangle {
            id: selectionIndicator
            anchors {
                bottom: parent.bottom
            }

            height: 2
            color: tabBar.highlightColor
        }
    }

    Component {
        id: tabDelegate

        View {
            id: tabItem

            width: tabBar.fullWidth ? tabBar.width/repeater.count : implicitWidth
            height: tabBar.height

            implicitWidth: isLargeDevice
                    ? Math.min(2 * tabPadding + row.width, ProtoScreen.guToPx(33))
                    : Math.min(Math.max(2 * tabPadding + row.width, tabMinWidth), ProtoScreen.guToPx(33))


            property bool selected: index == tabBar.selectedIndex

            property var tab: isTabView ? tabs.getTab(index) : modelData

            Ink {
                anchors.fill: parent
                enabled: tab.enabled
                onClicked: tabBar.selectedIndex = index

                Row {
                    id: row

                    anchors.centerIn: parent
                    spacing: ProtoScreen.guToPx(1.25)

                    Icon {
                        anchors.verticalCenter: parent.verticalCenter

                        source: tabItem.tab.hasOwnProperty("iconSource")
                                ? tabItem.tab.iconSource : tabItem.tab.hasOwnProperty("iconName")
                                ? "icon://" + tabItem.tab.iconName : ""
                        color: tabItem.selected
                                ? darkBackground ? Theme.dark.iconColor : Theme.light.accentColor
                                : darkBackground ? Theme.dark.shade(tab.enabled ? 0.6 : 0.2) : Theme.light.shade(tab.enabled ? 0.6 : 0.2)

                        visible: source != "" && source != "icon://"

                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    Label {
                        id: label

                        text: typeof(tabItem.tab) == "string"
                                ? tabItem.tab : tabItem.tab.title
                        color: tabItem.selected
                                ? darkBackground ? Theme.dark.textColor : Theme.light.accentColor
                                : darkBackground ? Theme.dark.shade(tab.enabled ? 0.6 : 0.2) : Theme.light.shade(tab.enabled ? 0.6 : 0.2)

                        style: "body2"
                        font.capitalization: Font.AllUppercase
                        anchors.verticalCenter: parent.verticalCenter
                        maximumLineCount: 2

//                        Behavior on color {
//                            ColorAnimation { duration: 200 }
//                        }
                    }

                    IconButton {
                        iconName: "qrc:/icons/navigation_close.png"
                        visible: tab.hasOwnProperty("canRemove") && tab.canRemove && (tabs.hasOwnProperty("removeTab") || tab.hasOwnProperty("close"))
                        color: tabItem.selected
                                ? darkBackground ? Theme.dark.iconColor : Theme.light.accentColor
                                : darkBackground ? Theme.dark.shade(tab.enabled ? 0.6 : 0.2) : Theme.light.shade(tab.enabled ? 0.6 : 0.2)
                        onClicked: tabBar.removeTab(tab, index)
                        size: ProtoScreen.guToPx(2.5)
                    }
                }
            }
        }
    }
}
