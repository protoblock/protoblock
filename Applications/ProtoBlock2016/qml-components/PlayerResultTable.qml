import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Private 1.0

Item {
    id: topwr
    anchors.fill: parent

    property variant selectedModel
    property bool posvisible: false
//    property alias ccount: tvr.columnCount
    property int focuscount: 0
    property string who: "default"
    property int rw: 8

    property bool kicker: true
    property bool def: true
    property bool qb: true
    property bool wr: true
    property bool rb: true

    property int ocol: 5
//    property int pcol: ocol + ocount
//    property int lcol: pcol + pcount
//    property int qcol: lcol + lcount
//    property int vcol: qcol + qcount

    Item {
        id: i2
        anchors.top: parent.top
        width: parent.width
        height: parent.height - ProtoScreen.guToPx(5)

        TableView {
            id: tvr
            Component.onCompleted: {
                MiddleMan.pResultsViewFilterProxyModel.sortAgain("result", sortIndicatorOrder)
//                selection.select(0);
                console.log("tvr comleted")
            }

            onSelectionChanged: {
                console.log("changed" + currentRow);
//                console.log("selected" + currentRow + model.data(currentRow,"lastname"));
//                            prevlpv.modelPick = model.data(currentRow,"awardsModel");
                selectedRow = currentRow
            }

            onClicked: {
                console.log("clicked" + currentRow);
                tvr.selectedRow = currentRow
            }

            onCurrentRowChanged: {
                console.log("onCurrentRowChanged" + currentRow);
                tvr.selectedRow = currentRow
            }

            highlightOnFocus: true
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height
            implicitWidth: parent.width
            model: MiddleMan.pResultsViewFilterProxyModel

            sortIndicatorVisible: true
            sortIndicatorOrder: Qt.DescendingOrder
            onSortIndicatorColumnChanged: {
                MiddleMan.pResultsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            onSortIndicatorOrderChanged: {
                MiddleMan.pResultsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            headerDelegate: headerdel
            frameVisible: false
            selectionMode: SelectionMode.SingleSelection

            property int selectedRow: -1
            rowDelegate: Rectangle {
               height: ProtoScreen.guToPx(3)
//               SystemPalette {
//                  id: myPalette;
//                  colorGroup: SystemPalette.Inactive
//               }
               color: styleData.row===tvr.selectedRow ? "Light Grey" : styleData.alternate?"#f5f5f5":"transparent"
//               {
//                  var baseColor = styleData.alternate?"#f5f5f5":"transparent"
//                  return styleData.selected?myPalette.highlight:baseColor
//               }
            }


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
                    backgroundColor: "transparent"
                    Material.Label {
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
                    lll.text = Qt.binding(function() {
                        if ( !model )
                            return styleData.value
                        else
                            return model.fullname

                    })

                    backgroundColor = Qt.binding(function() {
                        if ( !model )
                            return "transparent"
                        else if (styleData.row===tvr.selectedRow )
                            return "Light Grey"
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
                width: ProtoScreen.guToPx(7)
                delegate: Material.Card {
                    flat: true
                    radius: 0
                    border.width: 0
                    anchors.fill: parent
                    anchors.centerIn: parent
                    backgroundColor: "transparent" //TeamInfo.getPosColor(styleData.value)
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
                        backgroundColor = Qt.binding(function() {
                            if (styleData.row===tvr.selectedRow )
                                return "Light Grey"
                            else
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
                        })
                    }
                }
            }

            TableViewColumn{
                id: teamcol
                role: "teamid"
                title: "Team"
                horizontalAlignment : Text.AlignHCenter
                width: ProtoScreen.guToPx(6)

                delegate: Material.Label {
                    text: styleData.value
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    Component.onCompleted: {
                        color = Qt.binding(function() {
                            return TeamInfo.getPrimaryAt(text)
                        })
                    }
                }
            }

            TableViewColumn{
                role: "result"
                title: "Result"
                horizontalAlignment : Text.AlignHCenter
                delegate: resdel
                movable: false
                width: ProtoScreen.guToPx(10)
            }

            TableViewColumn {
                role: "fb"
                title: "ƑɃ"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(10)
                delegate: fbdelFB
            }

            TableViewColumn{
                role: "myproj"
                title: "My Projection"
                horizontalAlignment : Text.AlignHCenter
                delegate: fbdel
                movable: false
                width: ProtoScreen.guToPx(10)
            }

            TableViewColumn {
                role: "myaward"
                title: "My Award"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(10)
                delegate: fbdel
                visible: posvisible
            }

            TableViewColumn {
                role: "mypos"
                title: "My Pos"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(10)
                delegate: negdel
                visible: posvisible
            }
            TableViewColumn {
                role: "myprice"
                title: "My Price"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(10)
                delegate: negdelfix
                visible: posvisible
            }


            TableViewColumn {
                role: "mypnl"
                title: "My Pnl"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(10)
                delegate: negdel
                visible: posvisible
            }


            TableViewColumn {
                role: "PassYd"
                title: "PassYd"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: qb
            }
            TableViewColumn {
                role: "PassTD"
                title: "PassTD"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: qb
            }

            TableViewColumn {
                role: "RushYd"
                title: "RushYd"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: qb || rb
            }

            TableViewColumn {
                role: "RushTD"
                title: "RushTD"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: qb || rb
            }

            TableViewColumn {
                role: "Rec"
                title: "Rec"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: wr || rb
            }

            TableViewColumn {
                role: "RecYd"
                title: "RecYd"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: wr || rb
            }
            TableViewColumn {
                role: "RecTD"
                title: "RecTD"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: wr || rb
            }

            TableViewColumn {
                role: "Int"
                title: "Int"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: qb
            }
            TableViewColumn {
                role: "Fum"
                title: "Fum"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: qb || wr || rb
            }
            TableViewColumn {
                role: "_2Pt"
                title: "2Pt"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: qb || wr || rb
            }
            TableViewColumn {
                role: "PAT"
                title: "PAT"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: topwr.kicker
            }
            TableViewColumn {
                role: "FG"
                title: "FG"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw) * 2
                delegate: fbdel
                visible: topwr.kicker
            }
            TableViewColumn {
                role: "PtsA"
                title: "PtsAlw"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: def
            }
            TableViewColumn {
                role: "Sack"
                title: "Sack"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: def
            }
            TableViewColumn {
                role: "TA"
                title: "TO"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: def
            }
            TableViewColumn {
                role: "D_TD"
                title: "DefTD"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: def
            }
            TableViewColumn {
                role: "SFTY"
                title: "SFTY"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: def
            }
            TableViewColumn {
                role: "D2pt"
                title: "Def2pt"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
                delegate: fbdel
                visible: def

            }

            TableViewColumn {
                role: "playerid"
                title: "playerid"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(rw)
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
                    model: ["All","QB" , "RB" , "WR" , "TE" , "K" , "DEF"]
                    enabled: styleData.column === 1
                    currentIndex: 0
                    visible: styleData.column === 1
                    anchors.fill: parent
                    onCurrentTextChanged: {
                       MiddleMan.pResultsViewFilterProxyModel.setPos(currentText)
                        kicker = def = qb = (currentIndex === 0)

                        if ( currentIndex === 0 )
                            kicker = def = qb = rb = wr = true
                        else {
                            kicker = def = qb = rb = wr = false

                        if ( currentIndex === 5) {
                            kicker = true;
                        }
                        else if ( currentIndex === 6) {
                            def = true
                        }
                        else if (currentIndex === 1 )
                            qb = true
                        else if ( currentIndex === 2)
                            rb = true
                        else wr = true
                        }
                    }
                }

                Material.IconButton {
                    id: exportit
                    anchors.left: parent.left
                    anchors.top: parent.top
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    width: ProtoScreen.guToPx(6)
                    height: parent.height
                    onClicked: {
                        importexportDialog.show();
                    }

                    size: ProtoScreen.guToPx(2.5)


                    visible: false//styleData.column === 0
                    enabled: false//styleData.column === 0
                    action: Material.Action {
                        iconName: "awesome/code"
                        hoverAnimation: true
                    }
                }

                Material.IconButton {
//                    anchors.horizontalCenter: parent.horizontalCenter
//                    anchors.bottom: parent.bottom
                    anchors.centerIn: parent
                    anchors.fill: parent
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    width: ProtoScreen.guToPx(6)
                    height: parent.height
                    color: Material.Theme.light.textColor
                    onClicked: {
                        posvisible = true;
                    }
                    size: ProtoScreen.guToPx(1.5)


                    visible: styleData.column === ocol && !posvisible
                    enabled: visible
                    action: Material.Action {
                        iconName: "awesome/expand"
                        hoverAnimation: true
                        name: "details"
                    }
                }

                Material.IconButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    Layout.fillHeight: false
                    Layout.fillWidth: false
                    width: ProtoScreen.guToPx(6)
                    height: parent.height
                    onClicked: {
                        posvisible = false;
                    }
                    size: ProtoScreen.guToPx(1.5)
                    color: Material.Theme.light.textColor


                    visible: styleData.column === ocol + 2 && posvisible
                    enabled: visible
                    action: Material.Action {
                        iconName: "awesome/compress"
//                        hoverAnimation: true
                        name: "less"
                    }
                }

            }

            Material.Card {
                id: mcbot
                width: parent.width
                height: parent.height * .50
                backgroundColor: (styleData.column >= 5 && styleData.column <= 9) ? themeroot.theme.accentColor :
                                 themeroot.theme.primaryColor
                anchors.bottom: parent.bottom
                radius: 1
                border.color:
                    styleData.column  === tvr.sortIndicatorColumn ? themeroot.theme.accentColor
                                                                 : "black"
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
                    color: (styleData.column >= 5 && styleData.column <= 9) ? Material.Theme.light.textColor : "white"
                    //                    font.bold: styleData.column === 4
                }
            }

        }
    }

    Component {
        id: copydel

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: (styleData.value)
        }
    }

    Component {
        id: statdel

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: (styleData.value)
        }
    }

    Component {
        id: resdel

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: !model ? "" : parseFloat(model.result).toFixed(2)
            font.bold: true;
        }
    }

    Component {
        id: bfbdel

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: styleData.value
            font.bold: true;
        }
    }

    Component {
        id: fbdel

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: (styleData.value < 1) ? "" : styleData.value;
            font.bold: false;
        }
    }

    Component {
        id: fbdelFB

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: (styleData.value < 1) ? "" : styleData.value;
            font.bold: false;
            font.family: fontfamFB
        }
    }

    Component {
        id: negdel

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: (styleData.value === 0) ? "" : styleData.value;
            font.bold: false;
        }
    }

    Component {
        id: negdelfix

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: (styleData.value === 0.0) ? "" : parseFloat(styleData.value).toFixed(2);
            font.bold: false;
        }
    }

    Component {
        id: columnComponent
        TableViewColumn {
            width: 100
        }

    }

    Connections {
        target: tvr.selection
        onSelectionChanged: topwr.update()
    }

    Connections {
        target: MiddleMan
        onThePrevWeekChanged: tvr.selectedRow = -1
    }

    function update() {
        console.log(" PlayerResultSelected update")
        tvr.selection.forEach(function(rowIndex) {
            MiddleMan.setPrevWeekResultLeaderSelection(tvr.model.getAwardsModelUid(rowIndex));
            //,tvr.model.roleForName("awardsModel"))
//            if (row && row.awardsModel) topwr.selectedModel = row.awardsModel
        })
    }

}

