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
    id: rowtt
    anchors.fill: parent

    property variant selectedModel
//    property alias ccount: tvr.columnCount
    property int focuscount: 0
    property string who: "default"
    property int rw: 8

    property string lightgreen: "#c8ffc8"
    property string lightred: "#ffc8c8"

    property int ocount: 3
    property int pcount: 3
    property int lcount: 2 //last - change
    property int qcount: 5

    property int ocol: 0
    property int pcol: ocol + ocount
    property int lcol: pcol + pcount
    property int qcol: lcol + lcount
    property int vcol: qcol + qcount


//    property int rowcol: bcol + 1
    property int poscol: pcol + 1

//    property int scount:


    //My Orders
        // My Positions
        // My PnL
        // My Average Price

    //Basic Stats
        //YTD Total
        //ROW Stats

    // name , pos, team, ros  ( 4-7)

    //Market Prices - ROW stats (x weeks)
        //bid size, bid, ask, ask size

    //YTD Stats




    Item {
        id: i2
        anchors.top: parent.top
        width: parent.width
        height: parent.height// - ProtoScreen.guToPx(5)

        TableView {
            id: tvr
            Component.onCompleted: {
//                MiddleMan.pPlayerQuoteSliceViewFilterProxyModel.sortAgain("lastprice", sortIndicatorOrder)
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
            model: MiddleMan.pPlayerQuoteSliceViewFilterProxyModel

            sortIndicatorVisible: true
            sortIndicatorOrder: Qt.DescendingOrder
            onSortIndicatorColumnChanged: {
                MiddleMan.pPlayerQuoteSliceViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            onSortIndicatorOrderChanged: {
                MiddleMan.pPlayerQuoteSliceViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
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


            //**** my orders data
            TableViewColumn {
                role: "myposition"
                title: "My Position"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: posdel
            }

            TableViewColumn {
                role: "mypnl"
                title: "My PnL"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: posdel
            }

            TableViewColumn {
                role: "myavg"
                title: "My Avg"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: avgdel
            }


            //**** Player
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



            //** green last change
            TableViewColumn {
                role: "lastprice"
                title: "Last"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: !model ? "transparent" : model.ldiff === 1 ? lightgreen : model.ldiff === -1 ? lightred :
                                         model.lsdiff === 2 ? "lightgray" : "transparent"
                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        id: ml
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: (styleData.value < 1) ? "" : styleData.value;
                        font.bold: false;
                    }

                }
            }


            TableViewColumn {
                role: "change"
                title: "Change"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: !model ? "transparent" : model.cdiff === 1 ? lightgreen : model.cdiff === -1 ? lightred :
                                            model.vdiff === 1 ? "lightgray" : "transparent"

                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: (styleData.value === 0) ? model.lastprice === 0 ? "" : "unch" : styleData.value;
                        font.bold: false;
                        color: !model ? Material.Theme.light.textColor : model.cdiff === 0 ? (styleData.value < 0 ? "red" : styleData.value > 0 ? "green" : "black") :
                                Material.Theme.light.textColor
                    }
                }
            }

            TableViewColumn {
                role: "blocknum"
                title: "Block"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(6)
                delegate: Material.Label {
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    text: styleData.value === 0 ? "" : styleData.value
                }
            }

            //**** market
            TableViewColumn{
                role: "bidsize"
                title: "Bid Qty"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: !model ? "transparent" : model.bsdiff === 1 ? lightgreen :
                                             model.bsdiff === -1 ? lightred : "transparent"

                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: (styleData.value < 1) ? "" : styleData.value;
                        font.bold: false;
                    }

                }
            }

            TableViewColumn {
                role: "bid"
                title: "Bid"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: !model ? "transparent" : model.bdiff === 1 ? lightgreen :
                                          model.bdiff === -1 ? lightred : "transparent"
                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: (styleData.value < 1) ? "" : styleData.value;
                        font.bold: false;
                    }

                }
            }

            TableViewColumn{
                role: "ask"
                title: "Ask"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: !model ? "transparent" : model.adiff === 1 ? lightgreen :
                                              model.adiff === -1 ? lightred : "transparent"
                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: (styleData.value < 1) ? "" : styleData.value;
                        font.bold: false;
                    }

                }
            }

            TableViewColumn {
                role: "asksize"
                title: "Ask Qty"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: !model ? "transparent" : model.asdiff === 1 ? lightgreen :
                                          model.asdiff === -1 ? lightred : "transparent"
                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: (styleData.value < 1) ? "" : styleData.value;
                        font.bold: false;
                    }

                }
            }

            TableViewColumn {
                role: "lastsize"
                title: "Last Qty"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: !model ? "transparent" : model.lsdiff === 2 ? "lightgray" :
                                    model.lsdiff === 1 ? lightgreen : model.lsdiff === -1 ? lightred : "transparent"
                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: (styleData.value < 1) ? "" : styleData.value;
                        font.bold: false;
                    }

                }
            }

            TableViewColumn {
                role: "hi"
                title: "High"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: !model ? "transparent" : model.hdiff === 0 ? "transparent" : lightgreen
                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: (styleData.value === 0) ? "" : styleData.value;
                        font.bold: false;
                    }

                }
            }

            TableViewColumn {
                role: "lo"
                title: "Low"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: !model ? "transparent" : model.lodiff === 0 ? "transparent" : lightred
                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: (styleData.value === 0) ? "" : styleData.value;
                        font.bold: false;
                    }

                }
            }

            TableViewColumn {
                role: "volume"
                title: "Volume"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: !model ? "transparent" : model.vdiff === 1 ? "lightgray" : "transparent"
                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: (styleData.value < 1) ? "" : styleData.value;
                        font.bold: false;
                    }

                }
            }

            TableViewColumn {
                role: "symbol"
                title: "Symbol"
                horizontalAlignment : Text.AlignHCenter
                movable: false
                width: ProtoScreen.guToPx(8)
                delegate: Material.Card {
                    anchors.centerIn: parent
                    flat: true
                    radius: 12
                    border.width: 0
                    anchors.rightMargin: ProtoScreen.guToPx(5)
                    anchors.leftMargin: ProtoScreen.guToPx(5)

                    backgroundColor: "transparent"
                    width: ProtoScreen.guToPx(9) // ml.width * 3
                    Material.Label {
                        anchors.centerIn: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                        text: styleData.value;
                        font.bold: false;
                    }

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
                    model: ["All","QB" , "RB" , "WR" , "TE" , "K" , "DEF"]
                    enabled: styleData.column === poscol
                    currentIndex: 0
                    visible: styleData.column === poscol
                    anchors.fill: parent
                    onCurrentTextChanged: {
                       MiddleMan.pResultsViewFilterProxyModel.setPos(currentText)
                    }
                }

//                Material.IconButton {
//                    id: exportit
//                    anchors.left: parent.left
//                    anchors.top: parent.top
//                    Layout.fillHeight: true
//                    Layout.fillWidth: false
//                    width: ProtoScreen.guToPx(6)
//                    height: parent.height
//                    onClicked: {
//                        importexportDialog.show();
//                    }

//                    size: ProtoScreen.guToPx(2.5)


//                    visible: false//styleData.column === 0
//                    enabled: false//styleData.column === 0
//                    action: Material.Action {
//                        iconName: "awesome/code"
//                        hoverAnimation: true
//                    }
//                }

            }

            Material.Card {
                id: mcbot
                width: parent.width
                height: parent.height * .50

                backgroundColor: (styleData.column >= ocol && styleData.column < pcol) ?
                        themeroot.theme.accentColor :
                        ( styleData.column >= pcol && styleData.column < lcol) ? themeroot.theme.primaryColor :
                         (styleData.column >= lcol &&  styleData.column < qcol ) ? "green" : themeroot.theme.primaryColor

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
                    color: (styleData.column >= ocol && styleData.column < pcol) ?
                            Material.Theme.light.textColor : "white"
//                            ( (styleData.column >= mcol && styleData.column <= scol) ||
//                               styleData.column === rowcol ) ? "white" : "white"
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
            text: styleData.value.toFixed(2)
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
        id: avgdel

        Material.Label {
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
            text: styleData.value.toFixed(2);
            font.bold: true;
        }
    }


    Component {
        id: fbdel

        Material.Card {
            anchors.centerIn: parent
            flat: true
            radius: 12
            border.width: 0
//            anchors.fill: parent
            anchors.rightMargin: ProtoScreen.guToPx(5)
            anchors.leftMargin: ProtoScreen.guToPx(5)

            backgroundColor: "transparent"
//            height: ml.height
            width: ProtoScreen.guToPx(9) // ml.width * 3
            Material.Label {
                id: ml
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                text: (styleData.value === 0) ? "" : styleData.value;
                font.bold: false;
//                color: (model.BackgroundColor === "transparent" || styleData.column !== 4) ? Material.Theme.light.textColor : "white"
            }

        }
    }

    Component {
        id: posdel
        Material.Card {
            anchors.centerIn: parent
            flat: true
    //        radius: 12
            border.width: 0
    //        anchors.rightMargin: ProtoScreen.guToPx(5)
    //        anchors.leftMargin: ProtoScreen.guToPx(5)

    //        backgroundColor: !model ? "transparent" : model.cdiff === 1 ? lightgreen : model.cdiff === -1 ? lightred :
    //                                model.vdiff === 1 ? "lightgray" : "transparent"

            width: ProtoScreen.guToPx(9) // ml.width * 3
            Material.Label {
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                text: (styleData.value === 0) ?  "" : styleData.value;
                font.bold: false;
                color: (styleData.value < 0) ? "red" : (styleData.value > 0) ? "green" : Material.Theme.light.textColor
            }
        }
    }


    Connections {
        target: tvr.selection
        onSelectionChanged: {

            rowtt.update();
            console.log("row onSelectionChanged");
        }
    }

    Connections {
        target: MiddleMan
        onThePrevWeekChanged: tvr.selectedRow = -1
    }

    function update() {
        console.log(" ROWtradng update")
        tvr.selection.forEach(function(rowIndex) {
            MiddleMan.startDepth(tvr.model.getPlayerSliceModelUid(rowIndex));
            //,tvr.model.roleForName("awardsModel"))
//            if (row && row.awardsModel) rowtt.selectedModel = row.awardsModel
        })
    }

}

