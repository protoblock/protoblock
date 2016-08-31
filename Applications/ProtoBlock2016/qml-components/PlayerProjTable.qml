import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Material 1.0 as Material
import Material.Extras 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Theme 1.0
import ProRotoQml.Protoblock 1.0

Item {
    id: topw
    anchors.fill: parent
    anchors.margins: ProtoScreen.guToPx(1)

    signal indrop(string fname)
    signal donedrop
    signal addcolumn(string fname)

    Material.Label {
        id: mm
        text: "LA LA LA"
        color: "#40000000"
        anchors.centerIn: parent
        font.pixelSize: ProtoScreen.font( ProtoScreen.XXLARGE)
        font.bold:  true
        visible: true
        function donedropM() {
            mm.visible = false;
        }

        function tmyMethod(fname) {
            mm.text = fname
            mm.visible = true;
        }

        Component.onCompleted: {
            topw.indrop.connect(tmyMethod)
            topw.donedrop.connect(donedropM)
        }
        z: i2.z-1
    }

    Item {
        id: i2
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
                console.log(" addColumn " )
                tv.addColumn(columnComponent.createObject(tv, { "role": fname, "title": fname}))
                tv.resizeColumnsToContents()
            }

            highlightOnFocus: false
            anchors.fill: parent
            model: MiddleMan.pProjectionsViewFilterProxyModel

            sortIndicatorVisible: true
            sortIndicatorOrder: Qt.AscendingOrder
            onSortIndicatorColumnChanged: {
                MiddleMan.pProjectionsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            onSortIndicatorOrderChanged: {
                MiddleMan.pProjectionsViewFilterProxyModel.sortAgain(getColumn(sortIndicatorColumn).role, sortIndicatorOrder)
            }

            headerDelegate: headerdel
            frameVisible: false
            selectionMode: SelectionMode.NoSelection
            itemDelegate:
                Material.Label {
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                text: styleData.value
           }

            TableViewColumn {
                role: "fullname"
                title: "Name"
                horizontalAlignment : Text.AlignHCenter
                movable: false
            }

            TableViewColumn{
                role: "teamid"
                title: "Team"
                horizontalAlignment : Text.AlignHCenter
            }

            TableViewColumn{
                role: "pos"
                title: "Position"
                horizontalAlignment : Text.AlignHCenter
                movable: false
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
                    text: styleData.value === 0 ? "on" : "off";
                }
            }

            TableViewColumn{
                role: "projection"
                title: " My Projection "
                horizontalAlignment : Text.AlignHCenter
                delegate: projdel
                movable: false
            }
        }
    }

    Component {
        id: projdel

        SpinBox {
            decimals: 0
            stepSize: 1.0
            maximumValue: 40
            minimumValue: 0
            value: styleData.value
            onEditingFinished: {
               //                           styleData.value = value
               console.log(" editing done " + styleData.row + " " + styleData.column + " s " + styleData.selected + "v  " + styleData.value);
               tv.model.setData(tv.model.index(styleData.row,0),
                                value, 0)
            }
        }
    }

    Component {
        id: headerdel
        Item {
            id: idd
            implicitWidth: textItem2.implicitWidth
            width: parent.width
            height: ProtoScreen.guToPx(6)

            Rectangle {
                id: rec
                height: parent.height * .50
                width: parent.width
                color: "white"
                //styleData.column < 3 ? "white" : "grey"
                anchors.top: parent.top

                ComboBox {
                    id: cbc
                    model: ["All","QB" , "RB" , "WR" , "TE" , "K" , "DEF"]
                    enabled: styleData.column === 2
                    currentIndex: 0
                    visible: styleData.column === 2
                    anchors.fill: parent
                    onCurrentTextChanged: {
                       MiddleMan.pProjectionsViewFilterProxyModel.setPos(currentText)
                    }
                }

                MouseArea {
                    enabled: styleData.column !== 2
                    anchors.fill: parent
                    onClicked: {
                        console.log(" YES FUCKER ")
                    }
                }
            }

            Material.Card {
                width: parent.width
                height: parent.height * .50
                backgroundColor: themeroot.theme.primaryColor
                anchors.bottom: parent.bottom
                radius: 0
                border.color:
                    styleData.column  === tv.sortIndicatorColumn ? themeroot.theme.accentColor
                                                                 : "black"
                Material.Label {
                    id: textItem2
                    text: " " + styleData.value + " ";
                    anchors.fill: parent
                    font.pixelSize: ProtoScreen.font(ProtoScreen.SMALL)
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }
            }

            function tmyMethod() {
//                console.log(" col " + styleData.column + " " + widths[styleData.column] )

                rec.color = Qt.binding(function() {
//                    console.log(" col " + styleData.column + " " + widths[styleData.column] )
                    if ( styleData.column >= 3)
                        return "green"
                    else
                        return rec.color
                })
            }

            function donedropM() {
                rec.color = Qt.binding(function() {
                    if ( styleData.column >= 3)
                        return "grey"
                    else
                        return rec.color
                })
            }

            Component.onCompleted: {
//                topw.indrop.connect(tmyMethod)
//                topw.donedrop.connect(donedropM)
            }
        }
    }

    Component {
        id: columnComponent
        TableViewColumn { width: 100}
    }

}
