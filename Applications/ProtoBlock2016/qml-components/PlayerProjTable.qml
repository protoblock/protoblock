import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
//import ProRotoQml.Protoblock 1.0
import QtQuick.Layouts 1.1

Item {
    id: topw
    anchors.fill: parent
    signal indrop(string fname)
    signal donedrop
    signal addcolumn(string fname)

    property alias ccount: tv.columnCount
    property int focuscount: 0

    Item {
        id: i2
        anchors.top: parent.top
        width: parent.width
        height: parent.height //- cb.height

        TableView {
            id: tv
            Component.onCompleted: {
                resizeColumnsToContents()

                //                width = Qt.binding(function(){
//                   return ProtoScreen.guToPx(6) * columnCount
//                })
                ppt.addcolumn.connect(addcolumnMethod)
            }

            function addcolumnMethod(fname) {
//                customRoleNames[0] = fname
                console.log(" addColumn " )
                var role = MiddleMan.addFnameColumn(fname)
                tv.addColumn(columnComponent.createObject(tv, { "role": role, "title": fname, "horizontalAlignment": Text.AlignHCenter}))
                tv.resizeColumnsToContents()
            }
            highlightOnFocus: false
            anchors.fill: parent
            model: MiddleMan.pProjectionsViewFilterProxyModel

            sortIndicatorVisible: true
            sortIndicatorOrder: Qt.DescendingOrder
            onSortIndicatorColumnChanged: {
                MiddleMan.pProjectionsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            onSortIndicatorOrderChanged: {
                MiddleMan.pProjectionsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            headerDelegate: headerdel
            frameVisible: false
            selectionMode: SelectionMode.NoSelection

            TableViewColumn {
                role: "lastname"
                title: "Player Name"
                horizontalAlignment : Text.AlignLeft
                movable: false
                delegate: Material.Card {
                    flat: true
                    radius: 0
                    border.width: 0
                    anchors.fill: parent
//                    anchors.leftMargin: ProtoScreen.guToPx(1)
                     backgroundColor: "transparent" //TeamInfo.getPosColor(model.pos)
                     Material.Label{
                         id: lll
                                 anchors.margins: ProtoScreen.guToPx(.25)
                                 anchors.left: parent.left
                                 text: styleData.value
                                 font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                 verticalAlignment: Text.AlignVCenter
                                 horizontalAlignment: Text.AlignHCenter
                                 Layout.fillHeight: true
                                 Layout.fillWidth: false

                    }

                    Component.onCompleted: {
//                        backgroundColor = Qt.binding(function() {
//                            if ( model )
//                                return TeamInfo.getPosColor(model.pos)
//                            else
//                                return "transparent"
//                        })

                        lll.text = Qt.binding(function() {
                            if ( !model )
                                return styleData.value
                            else
                                return model.fullname

                        })

                        backgroundColor = Qt.binding(function() {
                            if ( !model )
                                return "transparent"
                            else {
                                switch(model.pos) {
                                case "WR":
                                    return "#FEFBB6";
                                case "RB":
                                    return "#BCFAAD";
                                case "QB":
                                    return "#F8ADAA";
                                case "TE":
                                    return "#CCB4F0";
                                case "K":
                                    return "#FBD580";
                                case "DEF":
                                    return "#AFE1FF";
                                default:
                                    return "transparent";
                                }
                            }
                        })
                    }
                }

            }
            TableViewColumn {
                id: tvm
                role: "pos"
                title: "Position"
                horizontalAlignment : Text.AlignHCenter
                movable: false

//                width: parent.width
//                anchors.fill: parent
//                width: lbl.implicitWidth + 2
                width: ProtoScreen.guToPx(6)
                delegate: Material.Card {
                    flat: true
                    radius: 0
                    border.width: 0
                    anchors.fill: parent
                    anchors.centerIn: parent
                    backgroundColor: "transparent" //TeamInfo.getPosColor(styleData.value)
//                    width: lbl.implicitWidth
                     Material.Label {
                         anchors.centerIn: parent
                         id: lbl
                         text: " " + styleData.value + " "
                         font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                         verticalAlignment: Text.AlignVCenter
                         horizontalAlignment: Text.AlignHCenter
                         Layout.fillHeight: true
                         Layout.fillWidth: false
                    }
                    Component.onCompleted: {
//                        tvm.width = Qt.binding(function() {
//                                return lbl.implicitWidth + 2
//                        })
                        backgroundColor = Qt.binding(function() {
//                            if ( !model )
//                                return "transparent"
//                            else {
                                switch(styleData.value) {
                                case "WR":
                                    return "#FEFBB6";
                                case "RB":
                                    return "#BCFAAD";
                                case "QB":
                                    return "#F8ADAA";
                                case "TE":
                                    return "#CCB4F0";
                                case "K":
                                    return "#FBD580";
                                case "DEF":
                                    return "#AFE1FF";
                                default:
                                    return "transparent";
                                }
//                            }
                        })
                    }
                }
//                Component.onCompleted: {
//                    width = Qt.binding(function() {
//                        return lbl.implicitWidth + 4
//                    })
//                }
            }

            TableViewColumn{
                role: "teamid"
                title: "Team"
                horizontalAlignment : Text.AlignHCenter
                delegate:
                    Material.Label{
                    //                anchors.centerIn: parent
                                    text: styleData.value
                                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    Layout.fillHeight: true
                                    Layout.fillWidth: false
//                                    color: TeamInfo.getPrimaryAt(text)
                    //                width: (parent.width * widths[1])
//                                    ColorAnimation on color { to: TeamInfo.getPrimaryAt(styleData.value); duration: 10000 }
                                    Component.onCompleted: {
                                        color = Qt.binding(function() {
                                            return TeamInfo.getPrimaryAt(text)
                                        })
                                    }
                                }
            }



            TableViewColumn {
                role: "status"
                title: "Roster"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: styleData.value === 0 ? "on" : "off"
                }
            }

            TableViewColumn{
                role: "knownProjection"
                title: "My Projection"
                horizontalAlignment : Text.AlignHCenter
                delegate: projdel
                movable: false
                width: ProtoScreen.guToPx(14)
            }

            TableViewColumn{
//                role: "knownProjection"
                title: " Avg "
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(7)
            }

        }
    }

    Component {
        id: projdel
        Rectangle {
            id: rec
            anchors.fill: parent
            border.width: 0
//                          (model.projection === model.knownProjection) ? 0 : ProtoScreen.guToPx(.125)
            border.color: themeroot.theme.accentColor
            //            width: ProtoScreen.guToPx(14)
            color: "transparent"

            Material.Label {
                anchors.left: parent.left
                anchors.leftMargin: 1
                anchors.margins: 1
//                focus: true
                id: lbl
                width: ProtoScreen.guToPx(4) - 1
                height: parent.height
                text: " "
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
//                font.bold: true
//                color: parseInt(text) > 0 ? "transparent" : themeroot.theme.accentColor
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log(" cliocked ")
                        if ( lbl.text !== "  " && parseInt(lbl.text) !== model.projection) {
                            tv.model.setData(tv.model.index(styleData.row,0),
                                             parseInt(lbl.text), 0)
                            lbl.text = Qt.binding(function(){
                                if ( model.knownProjection !==  model.projection)
                                    return model.knownProjection
                                else
                                    return "  "
                            })

                             rec.border.width = Qt.binding(function(){
                                 if ( model.knownProjection !==  model.projection)
                                     return ProtoScreen.guToPx(.25)
                                 else
                                     return 0
                             })
                        }
                    }
                }
            }

            SpinBox {
                id: sb
                anchors.left: lbl.right
                anchors.right: parent.right
                anchors.margins: 1
//                anchors.right: parent.right
                width: ProtoScreen.guToPx(10)
                height: parent.height
    //            width: parent.width * .70
                font: lbl.font
                decimals: 0
                stepSize: 1.0
                maximumValue: 40
                minimumValue:       0
                value: styleData.value
                horizontalAlignment: Text.AlignHCenter

                onEditingFinished: {
                   //                           styleData.value = value
                   console.log(" editing done " + styleData.row + " " + styleData.column + " s " + styleData.selected + "v  " + styleData.value);
                   tv.model.setData(tv.model.index(styleData.row,0),
                                    value, 0)

                    if ( model.knownProjection !==  value)
                        topw.focuscount = 1

                   lbl.text = Qt.binding(function(){
                       if ( model.knownProjection !==  value)
                           return model.knownProjection
                       else
                           return "  "
                   })

                    rec.border.width = Qt.binding(function(){
                        if ( model.knownProjection !==  value)
                            return ProtoScreen.guToPx(.25)
                        else
                            return 0
                    })

                }
            }
        }
    }

    Component {
        id: headerdel
        Rectangle {
            id: idd
            implicitWidth: textItem2.implicitWidth
            width: parent.width
            height: ProtoScreen.guToPx(6)
//            anchors.fill: parent
            color: "white"
            Rectangle {
//                enabled: styleData.column > 3
//                visible: styleData.column > 3
                id: rec
                height: parent.height * .50
                width: parent.width
                color: "transparent"
                //styleData.column < 3 ? "white" : "grey"
                anchors.top: parent.top
//                border.width: ProtoScreen.guToPx(.125)
//                border.color: light.textColor
//                radius: ProtoScreen.guToPx(.125)
                ComboBox {
                    id: cbc
                    model: ["All","QB" , "RB" , "WR" , "TE" , "K" , "DEF"]
                    enabled: styleData.column === 1
                    currentIndex: 0
                    visible: styleData.column === 1
                    anchors.fill: parent
                    onCurrentTextChanged: {
                       MiddleMan.pProjectionsViewFilterProxyModel.setPos(currentText)
                    }
                }

                Material.IconButton {
                    id: lbl
//                    anchors.fill: parent;
//                    anchors.centerIn: parent
                    anchors.left: parent.left
                    anchors.top: parent.top
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    width: ProtoScreen.guToPx(4)
                    height: parent.height
//                    color: "black"
//                    iconSource: "icon://" + "awesome/undo"// "qrc:/icons/navigation_close.png"
                    onClicked: {
                        MiddleMan.undoProj()
                        console.log("clicked icon")
                        topw.focuscount = 0
                    }

                    size: ProtoScreen.guToPx(2.5)

                    visible: styleData.column === 4
                    enabled: styleData.column === 4
                    action: Material.Action {
                        name: "undo"
                        iconName: "awesome/undo"
                        hoverAnimation: true
                    }
                }

                Material.Button {
                    focus: (topw.focuscount > 0) ? true : false
                    width: parent.width - ProtoScreen.guToPx(4)
                    anchors.left: lbl.right
                    anchors.right: parent.right
                    height: parent.height
                    visible: styleData.column === 4
                    enabled: styleData.column === 4
                    text: "Send"
                    onClicked : {
                        console.log("clicked send")
                        topw.focuscount = 0
                        MiddleMan.sendProjections()
                    }
                    backgroundColor: themeroot.theme.accentColor
                    textColor: "white"
                }

                Material.IconButton {
                    id: li
                    width: ProtoScreen.guToPx(4)//parent.width * .50
                    anchors.left: parent.left
                    height: parent.height
                    anchors.bottom: parent.bottom
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: styleData.column > 4
                    enabled: styleData.column > 4
                    onClicked : {
                        console.log("clicked send")

//                        tv.model.setData(tv.model.index(1,styleData.column),
//                                         tv.model.get(1), 0)

                    }

                    size: ProtoScreen.guToPx(2)
                    action: Material.Action {
                        name: "Copy-Merge Projection"
                        iconName: "awesome/copy"
                        hoverAnimation: true
                    }
                }

                Material.IconButton {
                    width: ProtoScreen.guToPx(4)//parent.width * .50
                    anchors.right: parent.right
                    anchors.left: li.right
                    height: parent.height
                    anchors.bottom: parent.bottom
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: styleData.column > 4
                    enabled: styleData.column > 4
                    size: ProtoScreen.guToPx(2)
                    onClicked : {
                        console.log("clicked send")
                        MiddleMan.copyProj(styleData.column, styleData.value, true)
                    }
                    action: Material.Action {
//                        name: "Copy-Clone Projection"
                        iconName: "awesome/clone"
                        text: "Copy-Clone Projection"
                        tooltip: "Copy and Replace your projection"
                        hoverAnimation: true
                    }

                }


//                Material.Button {
//                    width: parent.width * .50
//                    anchors.left: parent.left
//                    height: parent.height
//                    visible: styleData.column > 4
//                    enabled: styleData.column > 4
//                    text: "Copy"
//                    onClicked : {
//                        console.log("clicked send")
//                    }
//                    backgroundColor: themeroot.theme.accentColor
//                    textColor: "white"
//                    tooltip: "help me"
//                }

//                MouseArea {
//                    enabled: styleData.column !== 1
//                    anchors.fill: parent
//                    onClicked: {
//                        console.log(" YES FUCKER ")
//                    }
//                }
            }

            Material.Card {
                id: mcbot
                width: parent.width
                height: parent.height * .50
                backgroundColor: styleData.column === 4 ? themeroot.theme.accentColor :
                                 styleData.column < 6 ? themeroot.theme.primaryColor : "#AFE1FF"
                anchors.bottom: parent.bottom
                radius: 1
                border.color:
                    styleData.column  === tv.sortIndicatorColumn ? themeroot.theme.accentColor
                                                                 : "black"
                Material.Label {
                    id: textItem2
                    text: styleData.value
                    anchors.fill: parent
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: styleData.column < 6 ? "white" : Material.Theme.light.textColor
                    font.bold: styleData.column === 4
                }
            }

//            function tmyMethod() {
////                console.log(" col " + styleData.column + " " + widths[styleData.column] )

//                rec.color = Qt.binding(function() {
////                    console.log(" col " + styleData.column + " " + widths[styleData.column] )
//                    if ( styleData.column >= 3)
//                        return "green"
//                    else
//                        return rec.color
//                })
//            }

//            function donedropM() {
//                rec.color = Qt.binding(function() {
//                    if ( styleData.column >= 3)
//                        return "grey"
//                    else
//                        return rec.color
//                })
//            }

//            Component.onCompleted: {
////                topw.indrop.connect(tmyMethod)
////                topw.donedrop.connect(donedropM)
//            }
        }
    }

    Component {
        id: columnComponent
        TableViewColumn {
            width: 100
//            horizontalAlignment : Text.AlignHCenter
//            movable: false
//            delegate: itdel
        }

    }
}
//    Component {
//        id: itdel
//        Material.Label {
//                   anchors.centerIn: parent
//                   verticalAlignment: Text.AlignVCenter
//                   horizontalAlignment: Text.AlignHCenter
//                   font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                   text: " value" + styleData.value + " : "
//              }
//

//            resources: {
//                var roleList = MiddleMan.pProjectionsViewFilterProxyModel.userRoleNames
//                var temp = []
//                for(var i=0; i<roleList.length; i++)
//                {
//                    var role  = roleList[i]
//                    temp.push(columnComponent.createObject(tv, { "role": role, "title": role, size: 100, horizontalAlignment: Text.AlignLeft}))
//                }

////                temp.push(columnComponent.createObject(tv, { "role": "fullname", "title": "fullname"}))

////                temp.push(columnComponent.createObject(tv, { "role": "CAB", "title": "CAB"}))

//                return temp
//            }


//            itemDelegate: Material.Label {
//                anchors.centerIn: parent
//                verticalAlignment: Text.AlignVCenter
//                horizontalAlignment: Text.AlignHCenter
//                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                text: " value" + styleData.value " : "
//           }

//            rowDelegate: Rectangle {
////                    color: TeamInfo.getPosColor(model.pos)
//            }

/*
            TableViewColumn {
                role: "firstname"
                title: "First Name"
                horizontalAlignment : Text.AlignLeft
                movable: false
                delegate:
                    Material.Card {

                     backgroundColor: TeamInfo.getPosColor(model.pos)
                     Material.Label{
//                                 anchors.centerIn: parent
                                 text: styleData.value
                                 font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                                 verticalAlignment: Text.AlignVCenter
                                 horizontalAlignment: Text.AlignHCenter
                                 Layout.fillHeight: true
                                 Layout.fillWidth: false
                    }
                }
            }
            */
