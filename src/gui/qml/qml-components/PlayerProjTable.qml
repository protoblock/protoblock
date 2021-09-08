import QtQml 2.2
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Private 1.0
//import QtQuick.Controls 2.1 as Mat2
//import QtQuick.Controls.Material 2.12
//import QtQuick.Controls.Material 2.4


Item {
    id: topw
    anchors.fill: parent
    signal indrop(string fname)
    signal donedrop
    signal addcolumn(string fname)

    property alias ccount: ltv.columnCount
    property int focuscount: 0

    property int pcol: 4

    Item {
        id: i2
        anchors.top: parent.top
        width: parent.width
        height: parent.height //- ProtoScreen.guToPx(5)

        TableView {
            id: ltv
            Component.onCompleted: {
//                ltv.resizeColumnsToContents()

                //                width = Qt.binding(function(){
//                   return ProtoScreen.guToPx(6) * columnCount
//                })
                ppt.addcolumn.connect(addcolumnMethod)
//                model.sortAgain("knownProjection",Qt.DescendingOrder)
            }

            function addcolumnMethod(fname) {
//                customRoleNames[0] = fname
                console.log(" addColumn " )
                var role = MiddleMan.addFnameColumn(fname,ltv.columnCount)
                ltv.addColumn(columnComponent.createObject(ltv, {
                                                              "role": role,
                                                              "title": fname,
                                                              "horizontalAlignment": Text.AlignHCenter,
                                                              "delegate": copydel
                                                          })
                             )


//                ltv.resizeColumnsToContents()
            }
            highlightOnFocus: false
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height
            implicitWidth: parent.width
            model: MiddleMan.pProjectionsViewFilterProxyModel

            sortIndicatorVisible: true
            sortIndicatorOrder: Qt.DescendingOrder
            onSortIndicatorColumnChanged: {
                console.log( "onSortIndicatorColumnChanged tv.getColumn(tv.sortIndicatorColumn).role" + ltv.getColumn(ltv.sortIndicatorColumn))
                if ( ltv.getColumn(ltv.sortIndicatorColumn) )
                    model.sortAgain(ltv.getColumn(sortIndicatorColumn).role, ltv.sortIndicatorOrder)
                else
                    console.log( " role null")
            }

            onSortIndicatorOrderChanged: {
                console.log( "onSortIndicatorOrderChanged tv.getColumn(tv.sortIndicatorColumn).role" + ltv.getColumn(ltv.sortIndicatorColumn))
                if ( ltv.getColumn(ltv.sortIndicatorColumn) )
                    ltv.model.sortAgain(ltv.getColumn(sortIndicatorColumn).role, ltv.sortIndicatorOrder)
                else
                    console.log( " role null")
            }

            headerDelegate: headerdel
            frameVisible: false
            selectionMode: SelectionMode.NoSelection

            rowDelegate: Rectangle {
               height: ProtoScreen.guToPx(3)
               color: styleData.alternate?"#f5f5f5":"transparent"
            }

            TableViewColumn {
                role: "lastname"
                title: "Player Name"
                horizontalAlignment : Text.AlignLeft
                movable: false

                delegate: Material.Card {
                    flat: true
//                    elevation: 0
                    radius: 0
                    border.width: 0
                    anchors.fill: parent
                     backgroundColor: "transparent" //TeamInfo.getPosColor(model.pos)
                     Material.Label{
                         id: lll

                                 anchors.leftMargin: ProtoScreen.guToPx(.25)
                                 anchors.topMargin: ProtoScreen.guToPx(.25)
                                 anchors.bottomMargin: ProtoScreen.guToPx(.25)
                                 anchors.verticalCenter: parent.verticalCenter
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
                title: "Pos"
                horizontalAlignment : Text.AlignHCenter
                movable: false

//                width: parent.width
//                anchors.fill: parent
//                width: lbl.implicitWidth + 2
                width: ProtoScreen.guToPx(7)
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
                width: ProtoScreen.guToPx(6)

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
                width: ProtoScreen.guToPx(6)
                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: styleData.value === 0 ? "on" : "off"
                }
            }

            TableViewColumn{
                role: "projection"
                title: "My Projection"
                horizontalAlignment : Text.AlignHCenter
                delegate: projdel
                movable: false
                width: ProtoScreen.guToPx(14)
            }

            TableViewColumn{
                role: "avg"
                title: "Average"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: copydel
//                delegate: Material.Label {
//                    anchors.centerIn: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter

//                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
//                    text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
//                }

            }
/*
            TableViewColumn{
                role: "playerid"
                title: "playerid"
                horizontalAlignment : Text.AlignHCenter

                movable: false
                width: ProtoScreen.guToPx(16)
                delegate: Material.Label {
                    anchors.centerIn: parent
                    text: " " + styleData.value + " "
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: false
               }
            }
*/
        }
    }

    Component {
        id: projdel
        Rectangle {
            id: rec
            anchors.fill: parent
            border.width: !model ? 0 : ((model.knownProjection !==  model.projection) ?
                                ProtoScreen.guToPx(.25) : 0)
//                          (model.projection === model.knownProjection) ? 0 : ProtoScreen.guToPx(.125)
            border.color: themeroot.theme.accentColor
            width: ProtoScreen.guToPx(16)
            color: "transparent"

            Material.Label {
                anchors.right: sb.left
//                anchors.leftMargin: ProtoScreen.guToPx(.125)
                anchors.left: parent.left
                anchors.margins: ProtoScreen.guToPx(.125)

//                focus: true
                id: lbl

                width: ProtoScreen.guToPx(6) - ProtoScreen.guToPx(.125) * 2.0

                height: parent.height
                text: !model ? "" : ((model.knownProjection !==  model.projection)
                      ? model.knownProjection : "")
                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
//                font.bold: true
//                color: parseInt(text) > 0 ? "transparent" : themeroot.theme.accentColor
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log(" clicked ")
                        if ( lbl.text !== "  " && model.knownProjection !== model.projection ) {
//                                parseInt(lbl.text) !== model.projection) {
                            model.projection = model.knownProjection
//                            ltv.model.setData(ltv.model.index(styleData.row,0),
//                                             parseInt(lbl.text), 0)
                            lbl.text = Qt.binding(function(){
                                if ( !model ) return " ";
                                else if (model.knownProjection !==  model.projection)
                                    return model.knownProjection
                                else
                                    return "  "
                            })

                             rec.border.width = Qt.binding(function(){
                                 if ( !model ) return 0;
                                 else if (model.knownProjection !==  model.projection)
                                     return ProtoScreen.guToPx(.25)
                                 else
                                     return 0
                             })

                            topw.focuscount--
                        }
                    }
                }
            }

            SpinBox {
                id: sb
                enabled: (!model || MiddleMan.liveSync !== "Live") ? false : model.isopen
                anchors.right: parent.right
                anchors.margins: ProtoScreen.guToPx(.125)
                width: ProtoScreen.guToPx(8)
                height: parent.height * .86
                font: lbl.font
                anchors.verticalCenter: parent.verticalCenter
                decimals: 0
                stepSize: 1.0
                maximumValue: 40
                minimumValue:  0
                value: styleData.value
                horizontalAlignment: Text.AlignHCenter
//                onFocusChanged: {
//                    console.log(" focus change done ");
//                }

                onValueChanged: {
                    if ( model && hovered && value !== model.knownProjection && !activeFocus ) {
                        //console.log(" forcing active focus");
                        forceActiveFocus();
                    }
                }

                onEditingFinished: {
                    //console.log(" editing done " + styleData.row + " " + styleData.column + " s " + styleData.selected + " v " + value);
                    if ( !model ) {
                        //console.log(" !model ");
                        return;
                    }

                    model.projection = value
                    if ( model && (model.knownProjection !==  value)) {
                        topw.focuscount++;
                    }

                   lbl.text = Qt.binding(function(){
                       if ( !model ) return ""
                       else if ( !model.knownProjection && value > 0 )
                           return "0"
                       else if (model.knownProjection !==  value)
                           return model.knownProjection
                       else
                           return "  "
                   })

                    rec.border.width = Qt.binding(function(){
                        if ( model && (model.knownProjection !==  value))
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
            height: ProtoScreen.guToPx(8)
            color: "white"
            Rectangle {
                id: rec
                height: parent.height * .50
                width: parent.width
                color: "transparent"
                anchors.top: parent.top
                ComboBox {
                    id: cbc
//                    textRole: "display"
                    anchors.centerIn: parent
                    height: parent.height * .75
                    width: parent.width * .90

                    model: ["ALL", "QB" , "RB" , "WR" , "TE" , "K" , "DEF"] //MiddleMan.pPosFilter//
                    enabled: styleData.column === 1
                    currentIndex: 0
                    visible: styleData.column === 1
//                    anchors.fill: parent
                    onCurrentTextChanged: {
                          MiddleMan.pProjectionsViewFilterProxyModel.setPos(currentText)
                    }
                }

                Material.IconButton {
                    id: exportit
//                    anchors.fill: parent;
//                    anchors.centerIn: parent
                    anchors.left: parent.left
                    anchors.top: parent.top
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    width: ProtoScreen.guToPx(6)
                    height: parent.height
//                    color: "black"
//                    iconSource: "icon://" + "awesome/undo"// "qrc:/icons/navigation_close.png"
                    onClicked: {
                        importexportDialog.show();
//                        MiddleMan.copyFDProj()
                    }

                    size: ProtoScreen.guToPx(2.5)


                    visible: styleData.column === 0
                    enabled: styleData.column === 0 && MiddleMan.liveSync === "Live"
                    action: Material.Action {
//                        name: "cod"
                        iconName: "awesome/code"
                        hoverAnimation: true
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
                    width: ProtoScreen.guToPx(6)
                    height: parent.height
//                    color: "black"
//                    iconSource: "icon://" + "awesome/undo"// "qrc:/icons/navigation_close.png"
                    onClicked: {
                        MiddleMan.undoProj()
                        console.log("clicked icon")
                        topw.focuscount = 0
                    }

                    size: ProtoScreen.guToPx(2.5)


                    visible: styleData.column === pcol
                    enabled: styleData.column === pcol
                    action: Material.Action {
                        name: "undo"
                        iconName: "awesome/undo"
                        hoverAnimation: true
                    }
                }

                Rectangle {
                    anchors.right: parent.right
                    height: parent.height
                    width: ProtoScreen.guToPx(10)
//                    color: "#f5f5f5"
                    visible: styleData.column === pcol
                    Material.Button {
                        id: but
                        focus: (topw.focuscount > 0) ? true : false
                        anchors.centerIn: parent
//                        anchors.top: parent.top
//                        anchors.horizontalCenter: parent.horizontalCenter
                        height: parent.height * .75
                        width: parent.width * .90
                        visible: styleData.column === pcol
                        enabled: styleData.column === pcol && MiddleMan.allowAction
                        text: "Send"

                        onClicked : {
                            forceActiveFocus();
                            console.log("clicked send")
                            topw.focuscount = 0
                            if ( realRoot.uname === "" ) {
    //                            rootLoader.source = "qrc:/Account.qml"
                                pageHelper.selectedTabIndex = themeroot.accountIndex;
                            }
                            else
                                MiddleMan.sendProjections()
                            but.focus = false
                        }

                        backgroundColor: themeroot.theme.accentColor
                        textColor: themeroot.theme.primaryColor//Material.Theme.light.textColor //themeroot.theme.secondaryColor
                        elevation: 2 + ( 2 * topw.focuscount)
                    }
                }
                Material.IconButton {
                    id: li
//                    width: ProtoScreen.guToPx(5)//parent.width * .50
                    anchors.right: parent.horizontalCenter
                    anchors.rightMargin: ProtoScreen.guToPx(.50)
                    height: parent.height
                    anchors.bottom: parent.bottom
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    visible: styleData.column > pcol
                    enabled: styleData.column > pcol
                    onClicked : {
                        console.log("clicked Copy-Merge")
                        topw.focuscount = MiddleMan.copyProj(styleData.column, styleData.value, false, false)
                    }

                    size: ProtoScreen.guToPx(3)
                    action: Material.Action {
                        name: "Copy-Merge Projection"
                        iconName: "awesome/copy"
                        hoverAnimation: true
                    }
                }

                Material.IconButton {
//                    width: ProtoScreen.guToPx(5)//parent.width * .50
//                    anchors.right: parent.right
                    anchors.left: parent.horizontalCenter
                    anchors.leftMargin: ProtoScreen.guToPx(.50)

//                    anchors.left: li.right
                    height: parent.height
                    anchors.bottom: parent.bottom
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: styleData.column > pcol
                    enabled: styleData.column > pcol
                    size: ProtoScreen.guToPx(3)
                    onClicked : {
                        console.log("clicked Copy-Clone")
                        topw.focuscount = MiddleMan.copyProj(styleData.column, styleData.value, true, false)
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
                backgroundColor: styleData.column === pcol ?
                                 (topw.focuscount > 0 ? "gray" : themeroot.theme.accentColor) :
                                 styleData.column < 6 ? themeroot.theme.primaryColor : "#AFE1FF"
                anchors.bottom: parent.bottom
                radius: 1
                border.color:
                    styleData.column  === ltv.sortIndicatorColumn ?
                       Qt.darker("white",1.5) : "black"
//                        (styleData.column === pcol ? "black" :
//                          themeroot.theme.accentColor  : "black"
                Material.Label {
                    anchors.margins: ProtoScreen.guToPx(.25)
                    id: textItem2
                    text: styleData.value
                    anchors.fill: parent
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: (styleData.column === pcol && !topw.focuscount > 0 ) ?
                               themeroot.theme.primaryColor :
                               styleData.column < 6 ? "white" :
                               Material.Theme.light.textColor
//                    font.bold: styleData.column === 4
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
        id: copydel

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: (styleData.value)//.replace( /(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") + " ƑɃ"
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

//    Connections {
//        target: MiddleMan

//        onUpdateWeekData: {
//            Loader.source = themeroot.start
//            console.log(" onLiveSyncChanged changd");
////            ltv.model.sortAgain("knownProjection",Qt.DescendingOrder)
//            MiddleMan.pProjectionsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
//            ltv.sortIndicatorColumn =    MiddleMan.pProjectionsViewFilterProxyModel
//            MiddleMan.pProjectionsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
//        }

//    }
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
