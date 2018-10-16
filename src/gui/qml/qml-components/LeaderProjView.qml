import QtQuick 2.5
import QtQuick.Controls 1.4

import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0
import QtQuick.Layouts 1.1



Item {
    id: topi

    property bool extvisible: false
    property bool offvisible: (myTheWeek < 1 || myTheWeek > 17)

    signal releasedit(string fname)
    property string dragtarget: "dropProxy"
    property int myTheWeek: MiddleMan.theWeek
    anchors.fill: parent
//    anchors.margins: ProtoScreen.guToPx(1)

    Component.onCompleted: {
        MiddleMan.pLeaderBoardSortModel.filterString = ""
    }

    Item {
//        anchors.top: cb.bottom
        width: parent.width
        height: parent.height //- cb.height

        TableView {
            id: tv
            Component.onCompleted: {
//                resizeColumnsToContents()
                model.sortAgain("lastupdate",Qt.DescendingOrder)
            }

            sortIndicatorColumn: 1
            highlightOnFocus:   false
//            anchors.fill: parent
            height: parent.height //- ProtoScreen.guToPx(5)
            implicitWidth: parent.width

            selectionMode: SelectionMode.NoSelection
            model: MiddleMan.pLeaderBoardSortModel
//            sortIndicatorVisible: true
            sortIndicatorOrder: Qt.DescendingOrder
            onSortIndicatorColumnChanged: {
                model.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
//                resizeColumnsToContents();
            }

            onSortIndicatorOrderChanged: {
                model.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
//                resizeColumnsToContents()
            }


            rowDelegate: Rectangle {
               height: ProtoScreen.guToPx(3)
               color: styleData.alternate?"#f5f5f5":"transparent"
            }

            headerDelegate:  Rectangle {
                width: parent.width
                height: ProtoScreen.guToPx(8)
                implicitWidth: textItem2.implicitWidth
                color: "white"
//                anchors.fill: parent
                Rectangle {
                    id: rec
                    height: parent.height * .50
                    width: parent.width
                    color: "transparent"
                    anchors.top: parent.top
                    anchors.margins: ProtoScreen.guToPx(.25)
                    Material.TextField {
                        anchors.leftMargin: ProtoScreen.guToPx(.5)
                        anchors.centerIn: parent
                        id: fnamesearch
                        width: parent.width / 1.07
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        placeholderText: "search..."
//                        helperText: "search..."

//                        anchors.horizontalCenter: parent.horizontalCenter
           //                    onAccepted: nameCheckBlank(nameText.text)
                        inputMethodHints: Qt.ImhNoPredictiveText;
                        onTextChanged: {
                           MiddleMan.pLeaderBoardSortModel.filterString = text
                        }
                        enabled: styleData.column === 0
                        visible: styleData.column === 0
//                        showBorder: false
//                        color: Material.Theme.light.textColor
                        activeFocusOnPress: true
                    }
                }

                Rectangle {
                    id: rec2
                    height: parent.height * .50
                    width: tv.width - (parent.width *.50)
//                    anchors.left: parent.right
                    color: "transparent"
                    anchors.top: parent.top
                    anchors.left: parent.horizontalCenter
                    anchors.margins: ProtoScreen.guToPx(.25)
//                    anchors.horizontalCenter: tv.horizontalCenter
//                    anchors.horizontalCenterOffset:
                    enabled: styleData.column === 0
                    visible: styleData.column === 0

                    RowLayout {
                        anchors.fill: parent

                        Material.Label {
                            text: "Fantasy Skill Leaders"
                            Layout.preferredWidth: ProtoScreen.guToPx(12)
                            font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                            elide: Text.ElideRight
                            wrapMode: Text.WordWrap
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            color: "green"
                            Layout.fillHeight: true
                            Layout.fillWidth: false
                        }
                        Material.IconButton {
//                            anchors.centerIn: parent
//                            anchors.fill: parent
                            Layout.fillHeight: true
                            Layout.fillWidth: false
                            Layout.alignment: Qt.AlignLeft
                            width: ProtoScreen.guToPx(3)
//                            height: parent.height
                            color: Material.Theme.light.textColor
                            onClicked: {
                                extvisible = !extvisible;
                            }
                            size: ProtoScreen.guToPx(1.5)


//                            visible: styleData.column === ocol && !posvisible
//                            enabled: visible
                            action: Material.Action {
                                iconName: "awesome/expand"
                                hoverAnimation: true
                                name: "details"
                            }
                        }

                    }
                }

                Material.Card {
                    width: parent.width
                    height: parent.height * .50
                    backgroundColor: themeroot.theme.primaryColor
                    anchors.bottom: parent.bottom
                    radius: 1
                    border.color:
                        styleData.column  === tv.sortIndicatorColumn ? themeroot.theme.accentColor : "black"

                    Material.Label {
                        id: textItem2
                        text: " " + styleData.value + " "
                        anchors.fill: parent
                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        elide: Text.ElideRight
                        wrapMode: Text.WordWrap
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: "white"
                    }
                }
            }

            frameVisible: false

            //                width: tv.width / 4.0
            //                    width: tv.width / 4.0
            //                    height: ProtoScreen.guToPx(6)
            //                        height: ProtoScreen.guToPx(3)
            //                        width: win.width * .75
            //Qt.rgba(Math.random(), Math.random(), Math.random(), 1)
//                    x: Math.random() * (win.width / 2 - 100)
//                    y: Math.random() * (win.height - 100)
            //                        radius: 1
            //                        anchors.fill: parent


            TableViewColumn {
                role: "name"
                title: "Fantasy Name"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                delegate:

//                    Item {
//                    id: win
//                    height: tx.height
                    //                        anchors.fill: parent
                    //                        anchors.centerIn: parent;
                    Material.Card {
                        anchors.margins: 0
                        id: mcard
                        anchors.fill: parent
                        flat: true
                        backgroundColor: MiddleMan.isMyName(styleData.value) ?
                                             (styleData.value !== uname ? Material.Theme.light.textColor : themeroot.theme.accentColor )
                                           : "#AFE1FF"


//                        height: parent.height
//                        width: parent.width
//                        z: mouseArea.drag.active || mouseArea.pressed ? -20 : -10
                        //Qt.lighter(Colors.blue,1.20)

                        Rectangle {
                        anchors.margins: 0
                        id: rect
                        width: parent.width
                        height: parent.height
                        property point beginDrag
                        property bool caught: false
                        property string fname: styleData.value
                        border { width:ProtoScreen.guToPx(.125); color: "grey" }
                        Drag.active: mouseArea.drag.active
                        color: "transparent"


                        //                            anchors.fill: parent
                        //                            style: Text.Raised
                        Material.Label {
                            id: tx
                            anchors.fill: parent
                            anchors.centerIn: parent
                            text: styleData.value
//                            color: "white"
//                            font.bold: true
                            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            Layout.fillHeight: true
                            Layout.fillWidth: false
                            color: MiddleMan.isMyName(styleData.value) ?
                                       (styleData.value !== uname ? themeroot.theme.accentColor : Material.Theme.light.textColor)
                                     : Material.Theme.light.textColor
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            drag.target: rect
                            onClicked: { console.log(" mouse click")}
                            onHoveredChanged: {console.log(" mouse hover")}

                            property int startX
                            property int startY

                            onPositionChanged: {
//                                console.log(" mouse onPositionChanged   " + rect.caught)
//                                                                rect.x += mouseX - startX
//                                                                rect.y += mouseY - startY
//                                                                startX = mouseX
//                                                                startY = mouseY
                            }

                            onPressed: {
                                startX = mouseX
                                startY = mouseY
                                console.log(" mouse pressed")
                                rect.beginDrag = Qt.point(rect.x, rect.y);
                            }

                            //                                 MiddleMan.newProjColumn(tx.text)

                            onReleased: {
                                 console.log(" mouse released   " + rect.caught)
                                 if(rect.caught) {
                                    lpv.releasedit(styleData.value)
//                                    backAnimX.duration = 1;
//                                    backAnimY.duration = 1;
//                                    backAnimX.spring = 0;
//                                    backAnimY.spring = 0;
//                                    rect.color = Qt.binding( function() {
//                                        return "transparent"
//                                    })

                                    rect.x = rect.beginDrag.x;
                                    rect.y = rect.beginDrag.y;
                                     mcard.backgroundColor = Qt.binding( function() {
                                        return themeroot.theme.primaryColor
                                     })
                                     tx.color = "white"
                                 }
                                 backAnimX.from = rect.x;
                                 backAnimX.to = rect.beginDrag.x;
                                 backAnimY.from = rect.y;
                                 backAnimY.to = rect.beginDrag.y;
                                 backAnim.start()
                            }
                        }

                        ParallelAnimation {
                            id: backAnim
                            SpringAnimation { id: backAnimX; target: rect; property: "x"; duration: 500; spring: 2; damping: 0.2 }
                            SpringAnimation { id: backAnimY; target: rect; property: "y"; duration: 500; spring: 2; damping: 0.2 }
                        }
                        }
                    }
//                }
            }


            //Block
            TableViewColumn{
                role: "lastupdate"
                title: "Block"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(5)
                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: styleData.value
                }
            }

            //2018 Leaders - off season
            TableViewColumn {
                role: "leaders20XX"
                title: myTheWeek === 0 ? MiddleMan.theSeason-1 : MiddleMan.theSeason
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(8)

                visible: offvisible

                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //Skill - off season
            TableViewColumn {
                role: "bits"
                title: "Skill"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(8)
                visible: offvisible


                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //Stake - off season
            TableViewColumn {
                role: "stake"
                title: "Stake"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(8)

                visible: offvisible

                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //PnL - off season
            TableViewColumn {
                role: "pnl"
                title: "PnL"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(8)

                visible: offvisible

                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //Net - off season
            TableViewColumn {
                role: "net"
                title: "Net"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(8)

                visible: offvisible

                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //Count - in season
            TableViewColumn{
                role: "numberproj"
                title: "Count"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(5)


                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //Wk- in season
            TableViewColumn{
                role:  "thisweek"
                title: "wk" + myTheWeek
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(5)

                visible: myTheWeek >= 1 && myTheWeek < 17


                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //Wk-1 - in season
            TableViewColumn{
                role:  "lastweek"
                title: "wk" + (myTheWeek-1)
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(5)

                visible: myTheWeek > 1 && myTheWeek <= 17
                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //2018 Leaders - in season
            TableViewColumn {
                role: "leaders20XX"
                title: MiddleMan.theSeason
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(8)

                visible: myTheWeek >= 1 || myTheWeek <= 17

                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //Overall - in season
            TableViewColumn {
                role: "bits"
                title: "Overall"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(8)
                visible: myTheWeek >= 1 || myTheWeek <= 17


                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //Stake - in season
            TableViewColumn {
                role: "stake"
                title: "Stake"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(8)

                visible: !offvisible && extvisible

                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            TableViewColumn {
                role: "pnl"
                title: "PnL"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(8)

                visible: !offvisible && extvisible

                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }

            //Net - off season
            TableViewColumn {
                role: "net"
                title: "Net"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(8)

                visible: !offvisible && extvisible

                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
                }
            }



        }

    }


    function timeSince(date) {

        var seconds = Math.floor((new Date() - date) / 1000);

        var interval = Math.floor(seconds / 31536000);

        if (interval > 1) {
            return interval + " years";
        }
        interval = Math.floor(seconds / 2592000);
        if (interval > 1) {
            return interval + " months";
        }
        interval = Math.floor(seconds / 86400);
        if (interval > 1) {
            return interval + " days";
        }
        interval = Math.floor(seconds / 3600);
        if (interval > 1) {
            return interval + " hours";
        }
        interval = Math.floor(seconds / 60);
        if (interval > 1) {
            return interval + " minutes";
        }
        return Math.floor(seconds) + " seconds";
    }
}


//                    Item {
//                    anchors.fill: parent
//                    id: rootr

//                    MouseArea {
//                        id: mouseArea

//                        anchors.fill: parent
//                        anchors.centerIn: parent
//                        drag.target: dragTarget

//                        onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : rootr

//                        Rectangle {
//                            id: tile

//                            anchors.fill: parent
//                            anchors.verticalCenter: parent.verticalCenter
//                            anchors.horizontalCenter: parent.horizontalCenter

//                            color: "green"

//                            Drag.keys: [ "red" ]
//                            Drag.active: mouseArea.drag.active
//                            states: State {
//                                when: mouseArea.drag.active
//                                ParentChange { target: tile; parent: dragTarget }
//                                AnchorChanges { target: tile; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
//                                PropertyChanges {
//                                    target: tile
//                                    color: "transparent"
//                                }
//                            }

//                        }
//                    }
//                }

//            }
